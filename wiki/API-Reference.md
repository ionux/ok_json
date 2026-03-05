# API Reference

## Core data structures

### `OkJsonParser`

Holds parser state:

- `tokens[OKJ_MAX_TOKENS]`: parsed token storage
- `token_count`: number of valid tokens
- `json`: pointer to source JSON text
- `position`: parse cursor

### `OkJsonToken`

Generic token descriptor:

- `type`: one of `OkJsonType`
- `start`: pointer into source JSON buffer
- `length`: token byte length

### Typed wrappers

- `OkJsonString`
- `OkJsonNumber`
- `OkJsonBoolean`
- `OkJsonArray`
- `OkJsonObject`

These carry `start` and `length`; array/object wrappers also include `count`.

## Enumerations

### `OkJsonType`

- `OKJ_UNDEFINED`
- `OKJ_OBJECT`
- `OKJ_ARRAY`
- `OKJ_STRING`
- `OKJ_NUMBER`
- `OKJ_BOOLEAN`
- `OKJ_NULL`

### `OkjError`

Main result/failure values returned by parse routines, including:

- `OKJ_SUCCESS`
- `OKJ_ERROR_SYNTAX`
- `OKJ_ERROR_UNEXPECTED_END`
- `OKJ_ERROR_MAX_TOKENS_EXCEEDED`
- `OKJ_ERROR_MAX_STR_LEN_EXCEEDED`
- `OKJ_ERROR_MAX_JSON_LEN_EXCEEDED`
- plus validation errors for pointers/types/content.

## Initialization and parse

### `void okj_init(OkJsonParser *parser, char *json_string)`

Initializes parser state and binds it to the caller-provided mutable JSON buffer.

### `OkjError okj_parse(OkJsonParser *parser)`

Tokenizes the current JSON text and returns status code.

## Key-based getters

Each getter searches for a key token (`OKJ_STRING`) and returns the immediate next token if type-compatible.

- `OkJsonString *okj_get_string(OkJsonParser *parser, const char *key)`
- `OkJsonNumber *okj_get_number(OkJsonParser *parser, const char *key)`
- `OkJsonBoolean *okj_get_boolean(OkJsonParser *parser, const char *key)`
- `OkJsonArray *okj_get_array(OkJsonParser *parser, const char *key)`
- `OkJsonObject *okj_get_object(OkJsonParser *parser, const char *key)`
- `OkJsonToken *okj_get_token(OkJsonParser *parser, const char *key)`

### Raw container getters

These return full container span metadata and do not enforce the standard array/object member count limits used by the non-raw getters:

- `OkJsonArray *okj_get_array_raw(OkJsonParser *parser, const char *key)`
- `OkJsonObject *okj_get_object_raw(OkJsonParser *parser, const char *key)`

## Counting helpers

- `uint16_t okj_count_objects(OkJsonParser *parser)`
- `uint16_t okj_count_arrays(OkJsonParser *parser)`
- `uint16_t okj_count_elements(OkJsonParser *parser)`

## Debug support

When compiled with `OK_JSON_DEBUG`:

- `void okj_debug_print(OkJsonParser *parser)` prints token diagnostics.

## Important usage notes

1. Returned pointers in getter structs refer to the original input buffer.
2. Getter return structs are static internal storage; copy values immediately if you need to retain multiple results robustly across calls.
3. Missing key or type mismatch returns `NULL`.
