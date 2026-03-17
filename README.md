# ok_json

## Description

The goals for this project are to be a simple, light, clean, fully tested, RFC 8259-compliant and MISRA C:2012-compliant JSON parser suitable for embedded, memory-constrained, and safety-critical projects.  It is written in pure C99 for maximum portability with no external library dependencies.

**Compliance Progress:**
- Test coverage currently sits at 100% which meets the goal of 100% coverage by version 1.0.0 release.  Analysis indicates robust happy path, edge case, security and safety corner case handling. 
- RFC 8259 and MISRA compliance progress and issues tracked in respective `*_COMPLIANCE` markdown files.  Informal analysis shows strong & significant alignment with both RFC 8259 and MISRA C:2012.
- Current `cppcheck` MISRA analysis reports all checked rules passing except Rules 8.9, 15.4, 15.5, which are currently tracked for refactoring before 1.0.0 release.

## Quick Start

```c
#include "ok_json.h"

/* 1. Declare a parser and a mutable JSON string */
OkJsonParser parser;
char json[] = "{\"temp\": 42, \"unit\": \"C\", \"valid\": true}";

/* 2. Initialise and parse */
okj_init(&parser, json);
if (okj_parse(&parser) != OKJ_SUCCESS) {
    /* handle parse error */
}

/* 3. Retrieve scalar values by key */
OkJsonNumber  *temp  = okj_get_number(&parser,  "temp");
OkJsonString  *unit  = okj_get_string(&parser,  "unit");
OkJsonBoolean *valid = okj_get_boolean(&parser, "valid");

if (temp  != NULL) { /* temp->start points to "42", temp->length == 2  */ }
if (unit  != NULL) { /* unit->start points to "C",  unit->length == 1  */ }
if (valid != NULL) { /* valid->start points to "true", valid->length == 4 */ }

/* 4. Copy a string value into a caller-supplied buffer */
char buf[32];
if (unit != NULL) {
    okj_copy_string(unit, buf, sizeof(buf)); /* buf == "C\0" */
}
```

All getter functions return `NULL` when the key is not found or when the value type does not match the requested type.

## API Reference

### Lifecycle

| Function | Description |
|----------|-------------|
| `okj_init(parser, json_string)` | Initialise the parser with a mutable JSON string |
| `okj_parse(parser)` | Tokenise the JSON string; returns `OkjError` |

### Value Getters

| Function | Returns |
|----------|---------|
| `okj_get_string(parser, key)` | `OkJsonString *` — quotes excluded |
| `okj_get_number(parser, key)` | `OkJsonNumber *` — raw numeric text |
| `okj_get_boolean(parser, key)` | `OkJsonBoolean *` — `"true"` or `"false"` literal |
| `okj_get_array(parser, key)` | `OkJsonArray *` — enforces `OKJ_MAX_ARRAY_SIZE` |
| `okj_get_object(parser, key)` | `OkJsonObject *` — enforces `OKJ_MAX_OBJECT_SIZE` |
| `okj_get_array_raw(parser, key)` | `OkJsonArray *` — full raw array text, no size limit |
| `okj_get_object_raw(parser, key)` | `OkJsonObject *` — full raw object text, no size limit |
| `okj_get_token(parser, key)` | `OkJsonToken *` — raw token from the parser's token array |

### Utilities

| Function | Description |
|----------|-------------|
| `okj_copy_string(str, buf, buf_size)` | Copy string content into a caller buffer with NUL termination |
| `okj_count_objects(parser)` | Count all `OKJ_OBJECT` tokens in the parsed result |
| `okj_count_arrays(parser)` | Count all `OKJ_ARRAY` tokens in the parsed result |
| `okj_count_elements(parser)` | Return the total token count (`parser->token_count`) |

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

`OKJ_MAX_TOKENS` and `OKJ_MAX_DEPTH` are preprocessor macros (used as array
dimensions in struct definitions). The remaining limits are `static const`
values defined in the header. All can be overridden at compile time.

## Wiki

A full local wiki is available in the [`wiki/`](wiki/) directory:

- [Home](wiki/Home.md)
- [Project Overview](wiki/Project-Overview.md)
- [Design Philosophy](wiki/Design-Philosophy.md)
- [API Reference](wiki/API-Reference.md)
- [Usage Guide](wiki/Usage-Guide.md)
- [Development and Testing](wiki/Development-and-Testing.md)

## Known Issues

- Not passing all MISRA C:2012 rule checks yet (using `cppcheck`).  All checks will be passing by the 1.0.0 release.

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
