# Usage Guide

## Build

```sh
make
make test
```

Artifacts:

- `ok_json.a` static library
- `test/ok_json_test_runner` test binary

For coverage reporting (requires `gcovr`):

```sh
make coverage
```

## Basic integration pattern

```c
#include "ok_json.h"

OkJsonParser parser;
char json[] = "{\"temp\":42,\"unit\":\"C\",\"valid\":true}";

okj_init(&parser, json, (uint16_t)(sizeof(json) - 1U));
if (okj_parse(&parser) == OKJ_SUCCESS) {
    OkJsonNumber  temp;
    OkJsonString  unit;
    OkJsonBoolean valid;

    if (okj_get_number (&parser, "temp",  4U, &temp)  == OKJ_SUCCESS) {
        /* temp.start points to "42", temp.length == 2 */
    }
    if (okj_get_string (&parser, "unit",  4U, &unit)  == OKJ_SUCCESS) {
        /* unit.start points to "C",  unit.length == 1 */
    }
    if (okj_get_boolean(&parser, "valid", 5U, &valid) == OKJ_SUCCESS) {
        /* valid.start points to "true", valid.length == 4 */
    }
}
```

## Working with token slices

Values are not copied into separate buffers. Instead, each result is a
`(start, length)` view into the input JSON text.  The `start` pointer is
**not** null-terminated.

To obtain a null-terminated C string, use `okj_copy_string()`:

```c
OkJsonString unit;
if (okj_get_string(&parser, "unit", 4U, &unit) == OKJ_SUCCESS) {
    char buf[65];
    okj_copy_string(&unit, buf, sizeof(buf));
    /* buf now contains a null-terminated copy of the string value */
}
```

`okj_copy_string` copies at most `buf_size - 1` bytes and always appends a
null terminator when `buf_size >= 1`.  It returns the number of bytes copied
(excluding the null terminator), or 0 on error.

## Error handling pattern

Use `okj_parse()` result values to differentiate classes of failures:

- syntax/format issues: `OKJ_ERROR_SYNTAX`, `OKJ_ERROR_BAD_NUMBER`,
  `OKJ_ERROR_BAD_STRING`, `OKJ_ERROR_BAD_BOOLEAN`, `OKJ_ERROR_INVALID_CHARACTER`
- structural issues: `OKJ_ERROR_BRACKET_MISMATCH`, `OKJ_ERROR_UNEXPECTED_END`
- resource/limit issues: `OKJ_ERROR_MAX_TOKENS_EXCEEDED`,
  `OKJ_ERROR_MAX_JSON_LEN_EXCEEDED`, `OKJ_ERROR_MAX_DEPTH_EXCEEDED`,
  `OKJ_ERROR_MAX_STR_LEN_EXCEEDED`
- pointer or object misuse: `OKJ_ERROR_NULL_PARSER_OBJ`, `OKJ_ERROR_BAD_POINTER`

See the full list of error codes in [API Reference](./API-Reference.md).

## Arrays and objects

- `okj_get_array` / `okj_get_object` enforce size limits via `OKJ_MAX_ARRAY_SIZE`
  (64) and `OKJ_MAX_OBJECT_SIZE` (32) and return `OKJ_ERROR_BAD_ARRAY` /
  `OKJ_ERROR_BAD_OBJECT` when the container exceeds the limit.
- `okj_get_array_raw` / `okj_get_object_raw` populate the full span + counted
  members without those limit checks.

Use raw variants when you need the exact source span and can handle larger
containers explicitly.

## Nesting depth limit

The parser enforces a maximum nesting depth of `OKJ_MAX_DEPTH` (16) for
combined object and array containers.  Inputs that would exceed this are
rejected with `OKJ_ERROR_MAX_DEPTH_EXCEEDED`.  This protects embedded targets
from unbounded stack growth.

## Pitfalls to avoid

1. Passing string literals directly to APIs expecting mutable JSON buffers in
   systems that place literals in read-only memory.
2. Freeing or overwriting the input buffer before consuming returned token
   pointers.
3. Assuming getters perform type coercion; they require an exact type match.
4. Directly null-terminating `start`; use `okj_copy_string()` instead.
5. Assuming all JSON escape sequences beyond the supported set are accepted;
   see [Design Philosophy](./Design-Philosophy.md) for current scope.
