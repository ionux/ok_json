# API Reference

## Core data structures

### `OkJsonParser`

Holds parser state:

- `tokens[OKJ_MAX_TOKENS]`: fixed-size parsed token storage
- `depth_stack[OKJ_MAX_DEPTH]`: container type at each nesting depth (object or array)
- `context`: current grammar expectation (`OkjParseContext`)
- `token_count`: number of valid tokens
- `depth`: current nesting depth
- `json`: pointer to source JSON text
- `json_len`: byte length of the JSON string (excluding any null terminator)
- `position`: parse cursor

### `OkJsonToken`

Generic token descriptor:

- `type`: one of `OkJsonType`
- `start`: pointer into source JSON buffer
- `length`: token byte length

### Typed wrappers

- `OkJsonString` — `start`, `length`
- `OkJsonNumber` — `start`, `length`
- `OkJsonBoolean` — `start`, `length`
- `OkJsonArray` — `start`, `count`, `length`
- `OkJsonObject` — `start`, `count`, `length`

Array and object wrappers include a `count` of their members and a `length`
covering the full raw text (including surrounding brackets/braces).

## Enumerations

### `OkJsonType`

- `OKJ_UNDEFINED`
- `OKJ_OBJECT`
- `OKJ_ARRAY`
- `OKJ_STRING`
- `OKJ_NUMBER`
- `OKJ_BOOLEAN`
- `OKJ_NULL`

### `OkjParseContext`

Internal grammar state used during tokenisation (not normally inspected by
callers, but visible in the struct):

- `OKJ_CTX_WANT_VALUE` — expecting any JSON value
- `OKJ_CTX_WANT_VALUE_OR_CLOSE` — expecting a value or `]` (just opened array)
- `OKJ_CTX_WANT_KEY` — expecting an object key string
- `OKJ_CTX_WANT_KEY_OR_CLOSE` — expecting a key string or `}` (just opened `{}`)
- `OKJ_CTX_WANT_COLON` — expecting `:` after a key
- `OKJ_CTX_WANT_SEP_OR_CLOSE` — expecting `,` or the matching close bracket

### `OkjError`

All result/failure codes returned by parse and getter routines:

| Code | Value | Meaning |
|---|---|---|
| `OKJ_SUCCESS` | 0 | Successful operation |
| `OKJ_ERROR_INVALID_CHARACTER` | 1 | Unexpected character encountered |
| `OKJ_ERROR_SYNTAX` | 2 | Structural syntax error |
| `OKJ_ERROR_OVERFLOW` | 3 | Internal numeric overflow |
| `OKJ_ERROR_UNEXPECTED_END` | 4 | Input ended prematurely |
| `OKJ_ERROR_MAX_TOKENS_EXCEEDED` | 5 | Token array full (`OKJ_MAX_TOKENS`) |
| `OKJ_ERROR_MAX_STR_LEN_EXCEEDED` | 6 | String exceeds `OKJ_MAX_STRING_LEN` |
| `OKJ_ERROR_BAD_POINTER` | 7 | Unexpected NULL pointer argument |
| `OKJ_ERROR_BAD_NUMBER` | 8 | Malformed numeric literal |
| `OKJ_ERROR_BAD_OBJECT` | 9 | Malformed object |
| `OKJ_ERROR_BAD_STRING` | 10 | Malformed string |
| `OKJ_ERROR_BAD_ARRAY` | 11 | Malformed array |
| `OKJ_ERROR_BAD_BOOLEAN` | 12 | Malformed boolean literal |
| `OKJ_ERROR_NULL_PARSER_OBJ` | 13 | Parser pointer is NULL |
| `OKJ_ERROR_INVALID_TYPE_ENUM` | 14 | Unrecognised `OkJsonType` value |
| `OKJ_ERROR_NO_FREE_SPACE` | 15 | No space left in token array |
| `OKJ_ERROR_PARSING_FAILED` | 16 | General parse failure |
| `OKJ_ERROR_MAX_JSON_LEN_EXCEEDED` | 17 | Input exceeds `OKJ_MAX_JSON_LEN` |
| `OKJ_ERROR_MAX_DEPTH_EXCEEDED` | 18 | Nesting exceeds `OKJ_MAX_DEPTH` |
| `OKJ_ERROR_BRACKET_MISMATCH` | 19 | Mismatched opening/closing brackets |

## Compile-time and const limits

| Constant | Default | Type | Description |
|---|---|---|---|
| `OKJ_MAX_TOKENS` | 128 | `#define` | Maximum number of tokens |
| `OKJ_MAX_DEPTH` | 16 | `#define` | Maximum container nesting depth |
| `OKJ_MAX_STRING_LEN` | 64 | `#define` | Maximum key/string length in bytes |
| `OKJ_MAX_ARRAY_SIZE` | 64 | `#define` | Maximum array elements (non-raw getter) |
| `OKJ_MAX_OBJECT_SIZE` | 32 | `#define` | Maximum object members (non-raw getter) |
| `OKJ_MAX_JSON_LEN` | 4096 | `#define` | Maximum input JSON length in bytes |

## Initialization and parse

### `void okj_init(OkJsonParser *parser, char *json_string, uint16_t json_len)`

Initializes parser state and binds it to the caller-provided mutable JSON
buffer.  `json_len` is the byte length of `json_string` excluding any null
terminator.  The buffer must remain valid for the lifetime of any token
pointers retrieved from the parser.

### `OkjError okj_parse(OkJsonParser *parser)`

Tokenizes the bound JSON text and returns a status code.  On success,
`parser->tokens` and `parser->token_count` are populated.

## Key-based getters

Each getter scans for an `OKJ_STRING` token whose content matches `key`, then
writes the immediately following token into a caller-supplied output struct.
All getters return `OkjError` — `OKJ_SUCCESS` on success, or the appropriate
error code on a missing key, type mismatch, or bad argument.

```c
OkjError okj_get_string (OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonString  *out_str);
OkjError okj_get_number (OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonNumber  *out_num);
OkjError okj_get_boolean(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonBoolean *out_bool);
OkjError okj_get_array  (OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonArray   *out_arr);
OkjError okj_get_object (OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonObject  *out_obj);
OkjError okj_get_token  (OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonToken   *out_tok);
```

`key_len` is the byte length of the key string (excluding any null terminator).

`okj_get_array` and `okj_get_object` enforce `OKJ_MAX_ARRAY_SIZE` and
`OKJ_MAX_OBJECT_SIZE` respectively and return `OKJ_ERROR_BAD_ARRAY` /
`OKJ_ERROR_BAD_OBJECT` when the container exceeds the limit.

### Raw container getters

These bypass the size limit checks and always populate the full `length` field:

```c
OkjError okj_get_array_raw (OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonArray  *out_arr);
OkjError okj_get_object_raw(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonObject *out_obj);
```

Use raw variants when you need the exact source span of a large container.

### String copy helper

```c
uint16_t okj_copy_string(const OkJsonString *str, char *buf, uint16_t buf_size);
```

Copies the parsed string value into `buf` with guaranteed null-termination.
At most `buf_size - 1` bytes of content are copied.  Returns the number of
characters copied (excluding the null terminator), or 0 on error.

Because token `start` pointers are not null-terminated, use this helper
whenever you need a C-string from a parsed string value.

## Counting helpers

```c
uint16_t okj_count_objects (const OkJsonParser *parser);
uint16_t okj_count_arrays  (const OkJsonParser *parser);
uint16_t okj_count_elements(const OkJsonParser *parser);
```

- `okj_count_objects` — total `OKJ_OBJECT` tokens (including nested)
- `okj_count_arrays` — total `OKJ_ARRAY` tokens (including nested)
- `okj_count_elements` — total token count; equivalent to `parser->token_count`

All return 0 when `parser` is `NULL`.

## Debug support

When compiled with `-DOK_JSON_DEBUG`:

```c
void okj_debug_print(OkJsonParser *parser);
```

Prints a human-readable dump of every token in `parser` to stdout.

## Important usage notes

1. The `start` pointer in every populated output struct points into the
   original input buffer.  Do not free or overwrite the buffer while using
   those pointers.
2. Declare one output struct per value; getter functions write into the struct
   you supply, so multiple results can coexist as long as you keep them in
   separate variables.
3. A missing key or type mismatch returns an error code (not `OKJ_SUCCESS`);
   always check the return value before reading the output struct.
4. Getters do not perform type coercion; the token immediately following the
   key must be the exact requested type.
5. Use `okj_copy_string()` to obtain null-terminated string values.
