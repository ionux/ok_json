# ok_json MVP Roadmap

This document outlines the work needed to bring ok_json to a functional MVP
state: a C99 JSON parser that builds cleanly, passes its tests, and is usable
by embedded developers as a minimal, dependency-free library.

---

## Current State Summary

The project has a solid structural foundation (API design, error codes, type
definitions, token model, build system, CI pipeline) but the core parsing
implementation has several bugs that prevent the existing tests from passing.
All six data-retrieval functions are stubs. Debug `printf` calls and standard
library dependencies (`ctype.h`, `string.h`, `stdio.h`) are scattered through
the source.

---

## Bug Inventory (blocking test passage)

These are concrete, specific bugs in `src/ok_json.c` that must be fixed before
the three existing tests can pass.

### BUG-1: Structural characters cause a false syntax error

In `okj_parse_value()`, the characters `:`, `,`, `}`, and `]` hit the `else`
branch and return `OKJ_ERROR_SYNTAX`. This means any realistic JSON object or
array immediately fails.

**Fix:** At the top of `okj_parse_value()`, after `okj_skip_whitespace()`,
skip structural characters before type-dispatch, or handle them as their own
case and return `OKJ_SUCCESS` without creating a token.

### BUG-2: Boolean and null values do not advance `parser->position`

After detecting `true`, `false`, or `null`, the code sets the token type but
does not advance the position past the keyword. The shared `position++` at line
177 only advances by 1. On the next call the parser re-enters the middle of the
same keyword, returns `OKJ_ERROR_SYNTAX`, and the whole parse fails.

**Fix:** Advance `parser->position` by 4 for `true` and `null`, and by 5 for
`false`, before falling through to the token-finalization code.

### BUG-3: String `token.start` points to the closing quote

`token.start` is set at line 166 (`&parser->json[parser->position]`) **after**
the scan loop has already moved `parser->position` to the closing `"`. The
token therefore points to `"` instead of the first character of the string
content.

**Fix:** Capture `&parser->json[start + 1]` (the character after the opening
quote) into `token.start` for STRING tokens, independently of the shared
assignment at line 166.

### BUG-4: String `token.length` is off by one

`parser->position - start` counts from the opening `"` to the closing `"`,
which is one character too long. The actual content length is
`parser->position - start - 1`.

**Fix:** Use `parser->position - start - 1` for the STRING token length.

### BUG-5: String scan does not advance past the closing quote

After the scan loop exits with `parser->position` on the closing `"`, the
shared `position++` at line 177 advances to the character immediately after the
closing quote. This is correct, but it relies on the shared increment being in
the right place. Review carefully when refactoring to ensure strings consume
both quotes.

### BUG-6: `token.start` shared assignment is wrong for non-string types

Line 166 sets `token.start = &parser->json[parser->position]` for all types.
For OBJECT (`{`) and ARRAY (`[`), this is fine—position still points to the
opening character. But for NUMBER, the scan loop has already moved position
past the digits, so `token.start` ends up pointing to the character after the
number. NUMBER tokens need `token.start` set to `&parser->json[start]` before
the scan loop runs, just as STRING tokens do.

**Fix:** Set `token.start` per type (before the scan loop), then remove or
conditionally apply the shared assignment at line 166.

### BUG-7: Test expectation mismatch for `test_parse_simple_object`

`test_parse_simple_object` parses `{"key": 42}` and asserts
`parser.token_count == 2` with tokens `[OKJ_OBJECT, OKJ_NUMBER]`. A flat
token-per-value scanner would produce three tokens: `OKJ_OBJECT`, `OKJ_STRING`
("key"), `OKJ_NUMBER`. The test implicitly assumes that object keys are **not**
emitted as independent tokens.

Two valid resolutions:

- **Option A (minimal):** Update the test to expect 3 tokens and adjust
  `tokens[1]` to `OKJ_STRING`, `tokens[2]` to `OKJ_NUMBER`. This is the
  simpler fix and honest about what the flat scanner produces.
- **Option B (design-aligned):** Extend the parser to track whether the current
  string is a key (odd-depth string inside an object) and skip emitting a token
  for it. This matches the test as written but requires depth tracking.

A decision needs to be made before the implementation is locked in. Option A is
recommended for MVP; Option B can be deferred to a post-MVP iteration.

---

## MVP Task List

Tasks are ordered by priority. Items marked **[BLOCKER]** must be done for the
build and tests to succeed.

### Phase 1 — Make tests pass

| # | Task | Notes |
|---|------|-------|
| 1 | Fix BUG-1: skip structural characters in parse loop | **[BLOCKER]** |
| 2 | Fix BUG-2: advance position for booleans and null | **[BLOCKER]** |
| 3 | Fix BUG-3 & BUG-4: correct string `start` pointer and `length` | **[BLOCKER]** |
| 4 | Fix BUG-6: set `token.start` correctly for NUMBER tokens | **[BLOCKER]** |
| 5 | Resolve BUG-7: decide key-token model and align test to match | **[BLOCKER]** |
| 6 | Initialize `OkJsonParser` fields to zero in `okj_init()` (tokens array) | Prevents undefined behavior on stack-allocated structs |

### Phase 2 — Clean up the implementation

| # | Task | Notes |
|---|------|-------|
| 7 | Remove all 13 debug `printf` calls from `ok_json.c` | Marked `REMOVE BEFORE COMMIT` |
| 8 | Remove `#include <stdio.h>` from `ok_json.c` | Follows from item 7 |
| 9 | Replace `isspace()` with an inline whitespace check | Removes `ctype.h` dependency |
| 10 | Replace `isdigit()` with a range check (`c >= '0' && c <= '9'`) | Removes `ctype.h` dependency |
| 11 | Replace `strncmp()` for boolean/null detection with character-by-character checks | Removes `string.h` dependency |
| 12 | Remove `#include <ctype.h>` and `#include <string.h>` once replaced | Achieves zero-dependency goal |
| 13 | Refactor `okj_parse_value()` and `okj_parse()` to single-return (MISRA) | TODOs at lines 60, 192 |

### Phase 3 — Implement getter functions

The six getter stubs are required for the library to be useful. At minimum,
implement:

| # | Function | Priority |
|---|----------|----------|
| 14 | `okj_get_token()` | Implement first; others can build on it |
| 15 | `okj_get_string()` | Core retrieval |
| 16 | `okj_get_number()` | Core retrieval |
| 17 | `okj_get_boolean()` | Core retrieval |
| 18 | `okj_get_array()` | Can be deferred to post-MVP if time is tight |
| 19 | `okj_get_object()` | Can be deferred to post-MVP if time is tight |

All getters need a key-lookup strategy. Since keys and values are interleaved in
the token array (once key-token emission is settled in Phase 1), the lookup
pattern is: scan tokens for a STRING whose content matches `key`, then return
the immediately following token cast to the appropriate type.

### Phase 4 — Test suite and CI

| # | Task | Notes |
|---|------|-------|
| 20 | Fix CI Valgrind path: `./tests/` → `./test/` in `ci.yml` line 22 | CI Valgrind step currently references wrong path |
| 21 | Add tests for each getter function once implemented | Cover success and NULL/not-found cases |
| 22 | Add test for `OKJ_ERROR_MAX_TOKENS_EXCEEDED` | Boundary condition |
| 23 | Add test for deeply nested JSON rejection (or correct handling) | Define and test the nesting limit behavior |
| 24 | Add test for `OKJ_ERROR_UNEXPECTED_END` (truncated JSON) | e.g., `{"key": ` with no closing brace |

### Phase 5 — Documentation and polish

| # | Task | Notes |
|---|------|-------|
| 25 | Update README: remove WIP warning once tests pass | Replace with accurate usage example |
| 26 | Add a minimal usage example to README | Show `okj_init` → `okj_parse` → `okj_get_*` |
| 27 | Fill in `@brief` docs on getter declarations in `ok_json.h` | Currently blank |
| 28 | Resolve `OKJ_MAX_TOKENS` macro vs const inconsistency in header (line 48) | Stale comment left over from a refactor |

---

## Out of Scope for MVP

The following items from the TODO_LIST and codebase are valuable but should be
deferred until after MVP is working:

- **Full MISRA C2012 compliance** — requires a MISRA-capable toolchain and
  formal analysis. Can be pursued incrementally after the parser is correct.
- **Nested structure depth tracking** — the current flat token model handles
  one level of nesting adequately for many embedded use cases. True recursive
  nesting requires a depth stack, which complicates the design.
- **Unicode / escape sequence handling in strings** — `\uXXXX`, `\\`, `\"` etc.
  are not parsed. Fine for MVP if inputs are ASCII-only.
- **Floating-point number support** — the number scanner handles `.` already,
  but exponent notation (`1e10`, `1.5E-3`) is not handled. Acceptable for MVP.
- **Configurable size limits** — `OKJ_MAX_TOKENS`, `OKJ_MAX_STRING_LEN`, etc.
  are compile-time constants. Runtime configuration is a future enhancement.

---

## Definition of Done (MVP)

MVP is reached when all of the following are true:

1. `make` completes with zero warnings and zero errors.
2. `make test` runs and all three existing tests pass (green output).
3. The Valgrind step in CI reports zero memory errors on the test binary.
4. No `printf` debug statements remain in `ok_json.c`.
5. `ok_json.c` does not `#include <ctype.h>`, `<string.h>`, or `<stdio.h>`.
6. At minimum `okj_get_string()`, `okj_get_number()`, and `okj_get_boolean()`
   return correct results for a simple flat JSON object.
7. The CI pipeline passes on a clean push.
