# ok_json MVP Roadmap

This document outlines the work needed to bring ok_json to a functional MVP
state: a C99 JSON parser that builds cleanly, passes its tests, and is usable
by embedded developers as a minimal, dependency-free library.

---

## Current State Summary (updated 2026-03-17)

**MVP is complete and post-MVP work is ongoing.** All 191 tests pass. The build
is clean (zero warnings, zero errors). The library has zero stdlib dependencies
in `ok_json.c`. The CI Valgrind path is correct. All cppcheck MISRA C:2012
analysis checks pass with no suppressed rule exceptions.

Post-MVP additions since the original MVP completion:
- RFC 8259 number exponent notation (`1e10`, `2.5E-3`)
- Full string escape sequence handling (`\"`, `\\`, `\/`, `\b`, `\f`, `\n`,
  `\r`, `\t`, `\uXXXX`)
- `OKJ_MAX_STRING_LEN` enforcement during string scanning
- `count` field population in `okj_get_array()` and `okj_get_object()`
- Raw container getters: `okj_get_array_raw()`, `okj_get_object_raw()`
- `okj_copy_string()` safe-copy helper
- Counting helpers: `okj_count_objects()`, `okj_count_arrays()`, `okj_count_elements()`
- `OKJ_MAX_JSON_LEN` input-length check in `okj_parse()`
- Fixed-size depth/state stack: `depth_stack[OKJ_MAX_DEPTH]` and `depth` in
  `OkJsonParser`; bracket matching and depth ceiling (`OKJ_MAX_DEPTH=16`);
  two new error codes (`OKJ_ERROR_MAX_DEPTH_EXCEEDED`, `OKJ_ERROR_BRACKET_MISMATCH`)
- Full RFC 3629 UTF-8 validation via `okj_validate_utf8_sequence()`: handles
  2-, 3-, and 4-byte sequences; rejects overlong encodings, surrogates, and
  truncated sequences; invoked from the string scanner in `okj_parse_value()`
- `okj_debug_print()` utility function for token-by-token parser state dump
- `OkjParseContext` enum for grammar context tracking: detects structural errors
  (trailing commas, missing colons, non-string object keys) during parsing
- All getter functions refactored to output-parameter pattern: callers supply
  a pointer to the result struct; no struct-by-value returns
- All getter key parameters require an explicit `uint16_t key_len` argument;
  no implicit `strlen()` calls inside the library
- Single-return rule (MISRA Rule 15.5) applied to `okj_parse_value()` and all
  getter functions; final-else added to all `if...else if` chains (Rule 15.4)
- All previously suppressed MISRA C:2012 rules resolved: Rules 8.9, 15.4,
  15.5, 19.1, 19.2; all cppcheck MISRA checks now pass with no suppressions
- 100% branch coverage confirmed via gcov/gcovr (okj_debug_print excluded)
- MISRA C:2012 compliance work: `MISRA_C2012_COMPLIANCE.md` created and updated;
  all cppcheck-checked rules pass with no known outstanding rule failures
- Test file changed to `#include "../src/ok_json.c"` directly, giving full
  visibility into static helpers for accurate branch coverage (gcov/gcovr)
- Security and robustness tests: backslash flood at/over limit, escape
  straddle at boundary, UTF-8 overlong NUL (0xC0 0x80), control character
  injection (bare tab/LF in string values), modified UTF-8 NULL byte,
  quoted-string structural-character spoofing, multi-byte UTF-8 boundary
  cases, and many more
- Test suite grown from 13 to 191 tests

The sections below are preserved for historical reference, with status
annotations added. Open items have been moved to the TODO_LIST.

---

## Bug Inventory (historical — all resolved)

### BUG-1: Structural characters cause a false syntax error ✅ FIXED

`}`, `]`, `,`, and `:` now advance the position and emit no token.

### BUG-2: Boolean and null values do not advance `parser->position` ✅ FIXED

`parser->position` is now advanced by 4 for `true`/`null` and 5 for `false`.

### BUG-3: String `token.start` points to the closing quote ✅ FIXED

`tok->start` is set to `&parser->json[parser->position + 1U]` (character after
the opening quote) before the scan loop.

### BUG-4: String `token.length` is off by one ✅ FIXED

Length is now `parser->position - start_pos` where `start_pos` begins after
the opening quote, giving the correct content length.

### BUG-5: String scan does not advance past the closing quote ✅ VERIFIED

The explicit `parser->position++` after the scan loop correctly advances past
the closing quote.

### BUG-6: `token.start` shared assignment is wrong for non-string types ✅ FIXED

`tok->start` is set per-type before the scan loop for NUMBER, STRING, and all
literal types.

### BUG-7: Test expectation mismatch for `test_parse_simple_object` ✅ RESOLVED

Resolved via Option A: test now asserts `token_count == 3` with tokens
`[OKJ_OBJECT, OKJ_STRING, OKJ_NUMBER]`, accurately reflecting the flat scanner.

---

## MVP Task List (status annotations added)

### Phase 1 — Make tests pass ✅ COMPLETE

| # | Task | Status |
|---|------|--------|
| 1 | Fix BUG-1: skip structural characters in parse loop | ✅ Done |
| 2 | Fix BUG-2: advance position for booleans and null | ✅ Done |
| 3 | Fix BUG-3 & BUG-4: correct string `start` pointer and `length` | ✅ Done |
| 4 | Fix BUG-6: set `token.start` correctly for NUMBER tokens | ✅ Done |
| 5 | Resolve BUG-7: decide key-token model and align test to match | ✅ Done (Option A) |
| 6 | Initialize `OkJsonParser` fields to zero in `okj_init()` | ✅ Done |

### Phase 2 — Clean up the implementation ✅ COMPLETE

| # | Task | Status |
|---|------|--------|
| 7 | Remove all debug `printf` calls from `ok_json.c` | ✅ Done |
| 8 | Remove `#include <stdio.h>` from `ok_json.c` | ✅ Done |
| 9 | Replace `isspace()` with an inline whitespace check | ✅ Done (`okj_is_whitespace`) |
| 10 | Replace `isdigit()` with a range check | ✅ Done (`okj_is_digit`) |
| 11 | Replace `strncmp()` for boolean/null detection with `okj_match()` | ✅ Done |
| 12 | Remove `#include <ctype.h>` and `#include <string.h>` | ✅ Done |
| 13 | Refactor `okj_parse()` to single-return (MISRA) | ✅ Done for `okj_parse()` and `okj_parse_value()` (post-MVP); all getter functions also refactored to single-return |

### Phase 3 — Implement getter functions ✅ COMPLETE

| # | Function | Status |
|---|----------|--------|
| 14 | `okj_get_token(parser, key, key_len, out_tok)` | ✅ Implemented (output-parameter signature — post-MVP) |
| 15 | `okj_get_string(parser, key, key_len, out_str)` | ✅ Implemented (output-parameter signature — post-MVP) |
| 16 | `okj_get_number(parser, key, key_len, out_num)` | ✅ Implemented (output-parameter signature — post-MVP) |
| 17 | `okj_get_boolean(parser, key, key_len, out_bool)` | ✅ Implemented (output-parameter signature — post-MVP) |
| 18 | `okj_get_array(parser, key, key_len, out_arr)` | ✅ Implemented (count field + output-parameter signature — post-MVP) |
| 19 | `okj_get_object(parser, key, key_len, out_obj)` | ✅ Implemented (count field + output-parameter signature — post-MVP) |

### Phase 4 — Test suite and CI ✅ COMPLETE

| # | Task | Status |
|---|------|--------|
| 20 | Fix CI Valgrind path: `./tests/` → `./test/` in `ci.yml` | ✅ Already correct |
| 21 | Add tests for each getter function once implemented | ✅ Done (191 tests total) |
| 22 | Add test for `OKJ_ERROR_MAX_TOKENS_EXCEEDED` | ✅ `test_max_tokens_exceeded` |
| 23 | Add test for deeply nested JSON rejection | ✅ `test_deeply_nested_at_limit` (updated post-MVP to exercise depth ceiling) |
| 24 | Add test for `OKJ_ERROR_UNEXPECTED_END` (truncated JSON) | ✅ `test_truncated_string` |

### Phase 5 — Documentation and polish ✅ COMPLETE

| # | Task | Status |
|---|------|--------|
| 25 | Update README: remove WIP warning once tests pass | ✅ Done |
| 26 | Add a minimal usage example to README | ✅ Done |
| 27 | Fill in `@brief` docs on getter declarations in `ok_json.h` | ✅ Done |
| 28 | Resolve `OKJ_MAX_TOKENS` macro vs const inconsistency in header | ✅ Resolved — all size limits (`OKJ_MAX_TOKENS`, `OKJ_MAX_DEPTH`, `OKJ_MAX_STRING_LEN`, `OKJ_MAX_ARRAY_SIZE`, `OKJ_MAX_OBJECT_SIZE`, `OKJ_MAX_JSON_LEN`) are preprocessor macros, required for array dimensions and MISRA scope compliance |

---

## Out of Scope for MVP (status updated)

- **Full MISRA C2012 compliance** — all cppcheck MISRA C:2012 checks now pass
  with no suppressed rule exceptions.  Previously suppressed rules (8.9, 15.4,
  15.5, 19.1, 19.2) have been resolved post-MVP.  Formal compliance still
  requires additional process evidence (deviation records, tool configuration
  control, manual review) beyond static-analysis output alone.
- **Nested structure depth tracking** — ✅ Implemented post-MVP.  A fixed-size
  16-slot depth/state stack now lives in `OkJsonParser`; bracket matching and
  depth ceiling enforced during parsing.
- **Unicode / escape sequence handling in strings** — ✅ Implemented post-MVP.
  All RFC 8259 escape sequences (`\"`, `\\`, `\/`, `\b`, `\f`, `\n`, `\r`,
  `\t`, `\uXXXX`) are recognised; `\uXXXX` requires exactly 4 hex digits.
- **Floating-point exponent notation** — ✅ Implemented post-MVP.  `e`/`E`
  with optional sign and 1+ digits handled per RFC 8259 §6.
- **Configurable size limits** — still compile-time only (`OKJ_MAX_TOKENS`,
  `OKJ_MAX_DEPTH`, `OKJ_MAX_STRING_LEN`, etc.).

---

## Definition of Done (MVP) — ✅ ALL MET

1. ✅ `make` completes with zero warnings and zero errors.
2. ✅ `make test` runs and all tests pass (13 at MVP; 191 as of 2026-03-17).
3. ✅ The Valgrind step in CI references the correct binary path.
4. ✅ No `printf` debug statements remain in `ok_json.c`.
5. ✅ `ok_json.c` does not `#include <ctype.h>`, `<string.h>`, or `<stdio.h>`.
6. ✅ `okj_get_string()`, `okj_get_number()`, and `okj_get_boolean()` return
      correct results for a simple flat JSON object.
7. ✅ The CI pipeline is configured and passes on a clean push.
