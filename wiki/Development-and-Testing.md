# Development, Testing, and CI

## Repository layout

- `include/ok_json.h` — public API and constants
- `src/ok_json.c` — parser implementation
- `test/ok_json_tests.c` — unit-style test cases (compiled to produce the `test/ok_json_test_runner` binary)
- `test/fuzz_target.c` — libFuzzer entry point for fuzz testing
- `test/compliance_harness.c` — file-based harness for JSON test-suite compliance testing
- `Makefile` — local build/test/coverage/fuzz targets
- `.github/workflows/ci.yml` — GitHub Actions CI

## Build flags and style bias

The Makefile uses strict warnings and C99 mode:

```
-Wall -Wextra -Werror -std=c99 -pedantic
-Wconversion -Wsign-conversion -Wfloat-equal -Wcast-qual
-Wcast-align -Wpointer-arith -Wshadow -Wlogical-op -Wundef
-Wswitch-default -Wswitch-enum -Wunreachable-code
```

This keeps parser changes disciplined and visible.

`-DOK_JSON_DEBUG` is enabled in default CFLAGS, enabling `okj_debug_print()`
in local builds.

## Makefile targets

| Target | Description |
|---|---|
| `make` (or `make all`) | Builds `ok_json.a` and the test runner, then runs tests |
| `make ok_json.a` | Builds the static library only |
| `make test` | Builds and runs the test binary |
| `make coverage` | Rebuilds with `--coverage`, runs tests, and generates `coverage.xml` via `gcovr` |
| `make fuzz` | Compiles `test/fuzz_target.c` with Clang libFuzzer + ASan/UBSan and runs the fuzzer for 30 seconds |
| `make clean` | Removes all build artifacts including coverage data |

## Test coverage themes

Current tests validate:

- parser init behavior and null/bad-pointer handling
- simple object and array parsing
- strings, numbers (integer, negative, float, exponent), booleans, null
- all supported escape sequences: `\"`, `\\`, `\/`, `\b`, `\f`, `\n`, `\r`, `\t`
- `\uXXXX` Unicode escapes including invalid hex, truncated sequences, and surrogate pairs
- raw multi-byte UTF-8 sequence validation (2-, 3-, 4-byte) with MC/DC boundary coverage
- type mismatch and missing key behavior in getters
- max token exhaustion
- malformed/truncated input cases
- trailing commas, trailing garbage, and multiple top-level values
- non-string object keys (number, boolean, null as key)
- bracket mismatch and depth stack unwinding
- object/array getter behavior (size limits, raw variants)
- `okj_copy_string` behavior (basic copy, null termination, truncation, null inputs)
- `okj_count_objects`, `okj_count_arrays`, `okj_count_elements` helper counters
- key length boundary conditions (exactly 64 bytes vs. 65 bytes)
- nesting depth limit (`OKJ_MAX_DEPTH`) and maximum JSON length limit
- `okj_debug_print` output under `OK_JSON_DEBUG`
- empty objects and arrays, nested objects and arrays

## Running checks locally

```sh
make clean
make
make test
```

For line/branch coverage (requires `gcovr`):

```sh
make coverage
```

The CI workflow (`build-and-test` job) additionally:

- Runs Valgrind (`memcheck` with full leak checking and origin tracking) over the test runner binary.
- Runs Cppcheck with the MISRA add-on against the test source, publishing results to the GitHub step summary.
- Publishes a gcovr line/branch coverage summary to the GitHub step summary.

A separate `fuzz` CI job compiles `test/fuzz_target.c` with Clang libFuzzer + ASan + UBSan and runs the fuzzer for up to 10 seconds per push/PR.

## Extending the project safely

When adding parser behavior:

1. Add failing tests first in `test/ok_json_tests.c`.
2. Keep limits explicit; avoid hidden dynamic allocation.
3. Preserve pointer-slice semantics (`start`, `length`) unless intentionally
   redesigning API contracts.
4. Update README and wiki pages to reflect any user-visible parsing rule changes.
