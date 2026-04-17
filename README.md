# ok_json

## Description

The goals for this project are to be a simple, light, clean, fully tested, RFC 8259-compliant and MISRA C:2012-compliant JSON parser suitable for embedded, memory-constrained, and safety-critical projects.  It is written in pure C99 for maximum portability with no external library dependencies.

**Compliance Progress:**
- Test coverage currently sits at 100% which meets the goal of 100% coverage by version 1.0.0 release.  Analysis indicates robust happy path, edge case, security and safety corner case handling. 
- RFC 8259 and MISRA compliance progress and issues tracked in respective `*_COMPLIANCE` markdown files.  Informal analysis shows strong & significant alignment with both RFC 8259 and MISRA C:2012.
- Current `cppcheck` MISRA analysis reports all checked rules passing.

## Validation and Quality Checks

`ok_json` is continuously checked in CI with:

- unit tests
- `cppcheck` static analysis
- `valgrind`
- `libFuzzer` with AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan)

The fuzzing target is compiled with Clang and exercised in CI using coverage-guided fuzzing against the parser input surface. Current CI fuzz runs complete without sanitizer-detected crashes or undefined-behavior findings within the configured time budget.

In addition to the project’s internal test suite, `ok_json` is validated against the external **JSONTestSuite** corpus. The current recorded compliance run passed all parser checks with:

- **Passed:** 283
- **Failed:** 0

The current external validation results are recorded in `json_compliance_report.txt`.

## Quick Start

```c
#include "ok_json.h"

/* 1. Declare a parser and a mutable JSON string */
OkJsonParser parser;
char json[] = "{"
    "\"temp\": 42, \"unit\": \"C\", \"valid\": true,"
    "\"tags\": [\"sensor\", \"indoor\"],"
    "\"location\": {\"floor\": 3}"
"}";

/* 2. Initialise and parse */
okj_init(&parser, json, (uint16_t)(sizeof(json) - 1U));
if (okj_parse(&parser) != OKJ_SUCCESS) {
    /* handle parse error */
}

/* 3. Retrieve scalar values by key into caller-supplied structs */
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

/* 4. Copy a string value into a caller-supplied buffer */
char buf[32];
if (okj_get_string(&parser, "unit", 4U, &unit) == OKJ_SUCCESS) {
    okj_copy_string(&unit, buf, (uint16_t)sizeof(buf)); /* buf == "C\0" */
}

/* 5. Retrieve array and object values */
OkJsonArray  tags;
OkJsonObject location;

if (okj_get_array (&parser, "tags",     4U, &tags)     == OKJ_SUCCESS) {
    /* tags.count == 2, tags.start points to '[' */
}
if (okj_get_object(&parser, "location", 8U, &location) == OKJ_SUCCESS) {
    /* location.count == 1, location.start points to '{' */
}

/* 6. Count containers and total tokens */
uint16_t num_objects  = okj_count_objects(&parser);   /* 2 — root + location */
uint16_t num_arrays   = okj_count_arrays(&parser);    /* 1 — tags             */
uint16_t num_elements = okj_count_elements(&parser);  /* total token count    */
```

All getter functions return an `OkjError` code and write their result into a caller-supplied struct.  They return an error code (not `OKJ_SUCCESS`) when the key is not found or the value type does not match.

## API Reference

### Lifecycle

| Function | Returns | Description |
|----------|---------|-------------|
| `okj_init(parser, json_string, json_len)` | `void` | Initialise the parser with a mutable JSON string and its byte length |
| `okj_parse(parser)` | `OkjError` | Tokenise the JSON string |

### Value Getters

Each getter returns `OkjError` and writes its result into a caller-supplied output struct.  All getters return `OKJ_ERROR_BAD_POINTER` when any pointer argument is `NULL`.

| Function | Output struct | On success | On failure (non-NULL args) |
|----------|--------------|------------|---------------------------|
| `okj_get_string(parser, key, key_len, out_str)` | `OkJsonString *` | `OKJ_SUCCESS`; content excludes surrounding quotes | `OKJ_ERROR_BAD_STRING` — key not found or value is not a string |
| `okj_get_number(parser, key, key_len, out_num)` | `OkJsonNumber *` | `OKJ_SUCCESS`; raw numeric text | `OKJ_ERROR_BAD_NUMBER` — key not found or value is not a number |
| `okj_get_boolean(parser, key, key_len, out_bool)` | `OkJsonBoolean *` | `OKJ_SUCCESS`; `"true"` or `"false"` literal | `OKJ_ERROR_BAD_BOOLEAN` — key not found or value is not a boolean |
| `okj_get_array(parser, key, key_len, out_arr)` | `OkJsonArray *` | `OKJ_SUCCESS`; enforces `OKJ_MAX_ARRAY_SIZE` | `OKJ_ERROR_BAD_ARRAY` — key not found, not an array, or element count exceeds `OKJ_MAX_ARRAY_SIZE` |
| `okj_get_object(parser, key, key_len, out_obj)` | `OkJsonObject *` | `OKJ_SUCCESS`; enforces `OKJ_MAX_OBJECT_SIZE` | `OKJ_ERROR_BAD_OBJECT` — key not found, not an object, or member count exceeds `OKJ_MAX_OBJECT_SIZE` |
| `okj_get_array_raw(parser, key, key_len, out_arr)` | `OkJsonArray *` | `OKJ_SUCCESS`; full raw array span, no size limit | `OKJ_ERROR_BAD_ARRAY` — key not found or value is not an array |
| `okj_get_object_raw(parser, key, key_len, out_obj)` | `OkJsonObject *` | `OKJ_SUCCESS`; full raw object span, no size limit | `OKJ_ERROR_BAD_OBJECT` — key not found or value is not an object |
| `okj_get_token(parser, key, key_len, out_tok)` | `OkJsonToken *` | `OKJ_SUCCESS`; raw token copied from parser token array | `OKJ_ERROR_BAD_POINTER` — key not found (no type-specific code) |

### Utilities

| Function | Returns | Description |
|----------|---------|-------------|
| `okj_copy_string(str, buf, buf_size)` | `uint16_t` | Copy string content into a caller-supplied buffer with NUL termination; returns bytes copied (excluding NUL), or `0` on error |
| `okj_count_objects(parser)` | `uint16_t` | Count all `OKJ_OBJECT` tokens in the parsed result, including nested objects; returns `0` if `parser` is `NULL` |
| `okj_count_arrays(parser)` | `uint16_t` | Count all `OKJ_ARRAY` tokens in the parsed result, including nested arrays; returns `0` if `parser` is `NULL` |
| `okj_count_elements(parser)` | `uint16_t` | Return the total token count (equivalent to `parser->token_count`); returns `0` if `parser` is `NULL` |

### Error Codes

All functions returning `OkjError` use the following values:

| Code | Value | Meaning |
|------|-------|---------|
| `OKJ_SUCCESS` | 0 | Operation succeeded |
| `OKJ_ERROR_INVALID_CHARACTER` | 1 | Unexpected character in input |
| `OKJ_ERROR_SYNTAX` | 2 | Structural syntax error (trailing comma, missing colon, etc.) |
| `OKJ_ERROR_OVERFLOW` | 3 | Internal numeric overflow |
| `OKJ_ERROR_UNEXPECTED_END` | 4 | Input ended before all containers were closed |
| `OKJ_ERROR_MAX_TOKENS_EXCEEDED` | 5 | Token array full; input exceeds `OKJ_MAX_TOKENS` |
| `OKJ_ERROR_MAX_STR_LEN_EXCEEDED` | 6 | String or key exceeds `OKJ_MAX_STRING_LEN` bytes |
| `OKJ_ERROR_BAD_POINTER` | 7 | `NULL` pointer passed where a non-`NULL` argument is required |
| `OKJ_ERROR_BAD_NUMBER` | 8 | Key not found or value is not a number |
| `OKJ_ERROR_BAD_OBJECT` | 9 | Key not found, value is not an object, or member count exceeds `OKJ_MAX_OBJECT_SIZE` |
| `OKJ_ERROR_BAD_STRING` | 10 | Key not found or value is not a string |
| `OKJ_ERROR_BAD_ARRAY` | 11 | Key not found, value is not an array, or element count exceeds `OKJ_MAX_ARRAY_SIZE` |
| `OKJ_ERROR_BAD_BOOLEAN` | 12 | Key not found or value is not a boolean |
| `OKJ_ERROR_NULL_PARSER_OBJ` | 13 | Parser pointer is `NULL` |
| `OKJ_ERROR_INVALID_TYPE_ENUM` | 14 | Unrecognised `OkJsonType` value encountered |
| `OKJ_ERROR_NO_FREE_SPACE` | 15 | No free slot remaining in the token array |
| `OKJ_ERROR_PARSING_FAILED` | 16 | General parse failure |
| `OKJ_ERROR_MAX_JSON_LEN_EXCEEDED` | 17 | Input exceeds `OKJ_MAX_JSON_LEN` bytes |
| `OKJ_ERROR_MAX_DEPTH_EXCEEDED` | 18 | Nesting depth exceeds `OKJ_MAX_DEPTH` |
| `OKJ_ERROR_BRACKET_MISMATCH` | 19 | Mismatched opening and closing brackets |

### Debug (compile with `-DOK_JSON_DEBUG`)

| Function | Description |
|----------|-------------|
| `okj_debug_print(parser)` | Print a human-readable dump of every token to stdout |

## Building

```sh
make        # builds ok_json.a and runs the test suite
make clean  # removes build artifacts
```

Requires a C99-capable compiler. Tested with GCC using
`-Wall -Wextra -Werror -std=c99 -pedantic -Iinclude -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-qual -Wcast-align -Wpointer-arith -Wshadow -Wlogical-op -Wundef -Wswitch-default -Wswitch-enum -Wunreachable-code -O0.`

## Limits

| Constant              | Default | Description                                  |
|-----------------------|---------|----------------------------------------------|
| `OKJ_MAX_TOKENS`      | 128     | Maximum tokens per parse                     |
| `OKJ_MAX_DEPTH`       | 16      | Maximum nesting depth for objects and arrays |
| `OKJ_MAX_STRING_LEN`  | 64      | Maximum key/string length in bytes           |
| `OKJ_MAX_ARRAY_SIZE`  | 64      | Maximum array element count                  |
| `OKJ_MAX_OBJECT_SIZE` | 32      | Maximum object member count                  |
| `OKJ_MAX_JSON_LEN`    | 4096    | Maximum raw JSON input length in bytes       |

All limits are preprocessor macros defined in the header and can be overridden
at compile time by defining them before including `ok_json.h`.

## Wiki

A full local wiki is available in the [`wiki/`](wiki/) directory:

- [Home](wiki/Home.md)
- [Project Overview](wiki/Project-Overview.md)
- [Design Philosophy](wiki/Design-Philosophy.md)
- [API Reference](wiki/API-Reference.md)
- [Usage Guide](wiki/Usage-Guide.md)
- [Development and Testing](wiki/Development-and-Testing.md)

## Known Issues
- None.

## Security

If you believe you have found a security vulnerability, please do *not* open a public issue.  Instead, follow the private reporting process described in SECURITY.md, including responsible disclosure expectations, reporting guidance, and coordination procedures.

## AI Assistance Disclosure

Portions of this project were developed with AI-assisted tooling.  AI was used as an engineering support tool for tasks such as brainstorming, code review, debugging, test-cases, documentation, refactoring, etc...  However, all code, tests, and documentation in this repository were reviewed and intentionally selected by me.

## Contributing

If you've found a bug or improvement, feel free to submit an update! Here's how:

1. Create a new branch off master for your changes.
2. Clone the repository to your local development machine.
3. Make your updates and save the files.
4. Add, commit, and push your changes back to the repository.
5. Create a new merge request so I can review your code.

Once everything looks good with no errors or formatting issues and all tests are passing, I'll approve and merge your code.

## License

Copyright (c) 2026 Rich Morgan

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
