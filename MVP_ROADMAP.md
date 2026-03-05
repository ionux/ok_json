# ok_json MVP Roadmap

This document outlines the work needed to bring ok_json to a functional MVP
state: a C99 JSON parser that builds cleanly, passes its tests, and is usable
by embedded developers as a minimal, dependency-free library.

---

## Current State Summary (updated 2026-03-05)

**MVP is complete.** All 13 tests pass. The build is clean (zero warnings,
zero errors). All six getter functions are implemented. The library has zero
stdlib dependencies in `ok_json.c`. The CI Valgrind path is correct.

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
| 13 | Refactor `okj_parse()` to single-return (MISRA) | ✅ Done for `okj_parse()`; `okj_parse_value()` retains one early-return on NULL — see TODO_LIST |

### Phase 3 — Implement getter functions ✅ COMPLETE

| # | Function | Status |
|---|----------|--------|
| 14 | `okj_get_token()` | ✅ Implemented |
| 15 | `okj_get_string()` | ✅ Implemented |
| 16 | `okj_get_number()` | ✅ Implemented |
| 17 | `okj_get_boolean()` | ✅ Implemented |
| 18 | `okj_get_array()` | ✅ Implemented (count field not yet populated — see TODO_LIST) |
| 19 | `okj_get_object()` | ✅ Implemented (count field not yet populated — see TODO_LIST) |

### Phase 4 — Test suite and CI ✅ COMPLETE

| # | Task | Status |
|---|------|--------|
| 20 | Fix CI Valgrind path: `./tests/` → `./test/` in `ci.yml` | ✅ Already correct |
| 21 | Add tests for each getter function once implemented | ✅ Done (13 tests total) |
| 22 | Add test for `OKJ_ERROR_MAX_TOKENS_EXCEEDED` | ✅ `test_max_tokens_exceeded` |
| 23 | Add test for deeply nested JSON rejection | ⬜ Not yet added — see TODO_LIST |
| 24 | Add test for `OKJ_ERROR_UNEXPECTED_END` (truncated JSON) | ✅ `test_truncated_string` |

### Phase 5 — Documentation and polish ✅ COMPLETE

| # | Task | Status |
|---|------|--------|
| 25 | Update README: remove WIP warning once tests pass | ✅ Done |
| 26 | Add a minimal usage example to README | ✅ Done |
| 27 | Fill in `@brief` docs on getter declarations in `ok_json.h` | ✅ Done |
| 28 | Resolve `OKJ_MAX_TOKENS` macro vs const inconsistency in header | ✅ Resolved — `OKJ_MAX_TOKENS` is a macro (required for array dimension); `OKJ_MAX_STRING_LEN`, `OKJ_MAX_ARRAY_SIZE`, `OKJ_MAX_OBJECT_SIZE` are `static const` |

---

## Out of Scope for MVP (unchanged — deferred)

- **Full MISRA C2012 compliance** — requires a MISRA-capable toolchain and
  formal analysis.
- **Nested structure depth tracking** — the current flat token model handles
  one level of nesting adequately for many embedded use cases.
- **Unicode / escape sequence handling in strings** — `\uXXXX`, `\\`, `\"` etc.
- **Floating-point exponent notation** — `1e10`, `1.5E-3` not yet handled.
- **Configurable size limits** — `OKJ_MAX_TOKENS` etc. are compile-time only.

---

## Definition of Done (MVP) — ✅ ALL MET

1. ✅ `make` completes with zero warnings and zero errors.
2. ✅ `make test` runs and all 13 tests pass.
3. ✅ The Valgrind step in CI references the correct binary path.
4. ✅ No `printf` debug statements remain in `ok_json.c`.
5. ✅ `ok_json.c` does not `#include <ctype.h>`, `<string.h>`, or `<stdio.h>`.
6. ✅ `okj_get_string()`, `okj_get_number()`, and `okj_get_boolean()` return
      correct results for a simple flat JSON object.
7. ✅ The CI pipeline is configured and passes on a clean push.
