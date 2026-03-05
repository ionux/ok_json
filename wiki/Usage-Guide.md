# Usage Guide

## Build

```sh
make
make test
```

Artifacts:

- `ok_json.a` static library
- `test/ok_json_test_runner` test binary

## Basic integration pattern

```c
#include "ok_json.h"

OkJsonParser parser;
char json[] = "{\"temp\":42,\"unit\":\"C\",\"valid\":true}";

okj_init(&parser, json);
if (okj_parse(&parser) == OKJ_SUCCESS) {
    OkJsonNumber  *temp  = okj_get_number(&parser,  "temp");
    OkJsonString  *unit  = okj_get_string(&parser,  "unit");
    OkJsonBoolean *valid = okj_get_boolean(&parser, "valid");

    if (temp != NULL) {
        /* temp->start points to "42", temp->length == 2 */
    }
}
```

## Working with token slices

Values are not copied into separate buffers. Instead, each result is a `(start, length)` view into the input JSON text.

If you need null-terminated strings, copy them into your own destination buffer and append `\0`.

## Error handling pattern

Use `okj_parse()` result values to differentiate classes of failures:

- syntax/format issue (`OKJ_ERROR_SYNTAX`, `OKJ_ERROR_BAD_NUMBER`, etc.),
- resource/limit issue (`OKJ_ERROR_MAX_TOKENS_EXCEEDED`, `OKJ_ERROR_MAX_JSON_LEN_EXCEEDED`),
- pointer or object misuse (`OKJ_ERROR_NULL_PARSER_OBJ`, `OKJ_ERROR_BAD_POINTER`).

## Arrays and objects

- `okj_get_array` / `okj_get_object` enforce size limits via `OKJ_MAX_ARRAY_SIZE` and `OKJ_MAX_OBJECT_SIZE`.
- `okj_get_array_raw` / `okj_get_object_raw` return full span + counted members without those limit checks.

Use raw variants when you need exact source span and can handle larger containers explicitly.

## Pitfalls to avoid

1. Passing string literals directly to APIs expecting mutable JSON buffers in systems that place literals in read-only memory.
2. Freeing or overwriting the input buffer before consuming returned token pointers.
3. Assuming getters perform type coercion; they require exact type match.
4. Assuming full JSON escape/unicode semantics beyond current implementation scope.
