# Project Overview

## What OK_JSON is

OK_JSON is a compact JSON tokenizer/parser library in C99 that parses an input JSON string into a fixed array of tokens stored inside `OkJsonParser`. It is designed for environments where predictability and low complexity are more important than complete JSON feature coverage.

Core files:

- Public API and types: `include/ok_json.h`
- Implementation: `src/ok_json.c`
- Tests: `test/ok_json_tests.c`
- Fuzzer: `test/fuzz_target.c`
- Compliance harness: `test/compliance_harness.c`
- Build/test automation: `Makefile`, `.github/workflows/ci.yml`

## Core capabilities

- Parse JSON containing object, array, string, number, boolean, and null token types.
- Store parser output in fixed-size token memory (`OKJ_MAX_TOKENS`) with no heap allocation.
- Retrieve values by key from parsed objects via typed getters:
  - `okj_get_string`
  - `okj_get_number`
  - `okj_get_boolean`
  - `okj_get_array` / `okj_get_array_raw`
  - `okj_get_object` / `okj_get_object_raw`
  - `okj_get_token`
- Copy parsed string values into null-terminated buffers via `okj_copy_string`.
- Count objects, arrays, and total tokens with helper functions.

## Parser model at a glance

OK_JSON is token-centric, not tree-centric:

1. `okj_init()` binds parser state to a mutable JSON buffer.
2. `okj_parse()` walks the input and appends tokens into `parser->tokens`.
3. Getter functions scan token stream for key/value pairs and expose slices (`start`, `length`) into the original JSON string.

Because tokens point into input memory, **the input buffer must remain valid for as long as retrieved values are used**.

## Constraints and compile-time limits

The parser intentionally relies on fixed limits for deterministic memory usage:

- `OKJ_MAX_TOKENS` (128) — maximum number of tokens
- `OKJ_MAX_DEPTH` (16) — maximum container nesting depth
- `OKJ_MAX_STRING_LEN` (64) — maximum key/string byte length
- `OKJ_MAX_ARRAY_SIZE` (64) — maximum array elements (non-raw getter)
- `OKJ_MAX_OBJECT_SIZE` (32) — maximum object members (non-raw getter)
- `OKJ_MAX_JSON_LEN` (4096) — maximum input length in bytes

These controls are documented in the header and enforced by parsing/getter behavior.

## Typical use case

A common flow is:

- parse a small telemetry or config object,
- extract a few known keys,
- avoid dynamic allocation and external library dependencies.

Example (simplified):

```c
OkJsonParser parser;
char json[] = "{\"temp\":42,\"ok\":true}";

okj_init(&parser, json, (uint16_t)(sizeof(json) - 1U));
if (okj_parse(&parser) == OKJ_SUCCESS) {
    OkJsonNumber  temp;
    OkJsonBoolean ok;
    if (okj_get_number (&parser, "temp", 4U, &temp) == OKJ_SUCCESS) {
        /* Use temp.start / temp.length */
    }
    if (okj_get_boolean(&parser, "ok",   2U, &ok)   == OKJ_SUCCESS) {
        /* Use ok.start / ok.length */
    }
}
```
