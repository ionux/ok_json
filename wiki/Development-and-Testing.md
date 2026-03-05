# Development, Testing, and CI

## Repository layout

- `include/ok_json.h` — public API and constants
- `src/ok_json.c` — parser implementation
- `test/ok_json_tests.c` — unit-style test cases
- `test/ok_json_test_runner.c` — test entrypoint
- `Makefile` — local build/test targets
- `.github/workflows/ci.yml` — GitHub Actions CI

## Build flags and style bias

The Makefile uses strict warnings and C99 mode (`-Wall -Wextra -Werror -pedantic` plus additional warning flags). This keeps parser changes disciplined and visible.

`OK_JSON_DEBUG` is enabled in default CFLAGS, allowing debug-print helpers in local builds.

## Test coverage themes

Current tests validate:

- parser init behavior and null handling,
- simple object/array parsing,
- strings, numbers, booleans, null,
- type mismatch and missing key behavior in getters,
- max token handling,
- malformed/truncated input cases,
- object/array getter behavior and helper counters.

## Running checks locally

```sh
make clean
make
make test
```

The CI workflow additionally executes Valgrind over the test runner binary.

## Extending the project safely

When adding parser behavior:

1. Add failing tests first in `test/ok_json_tests.c`.
2. Keep limits explicit; avoid hidden dynamic allocation.
3. Preserve pointer-slice semantics (`start`, `length`) unless intentionally redesigning API contracts.
4. Update README and wiki pages to reflect any user-visible parsing rule changes.

## Suggested future documentation additions

- A formal compatibility matrix describing accepted vs rejected JSON constructs.
- Examples for nested objects/arrays with raw getters.
- A migration note if compile-time constants are ever made configurable via build flags.
