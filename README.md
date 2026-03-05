# ok_json

## Description

Simple, light, and clean JSON parser suitable for embedded, memory-constrained,
and safety-critical projects. Written in pure C99 for maximum portability.
Zero external library dependencies (`stdio.h`, `string.h`, and `ctype.h` are
not used). Designed with safety-critical coding standards (MISRA C2012) in mind.

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

/* 3. Retrieve values by key */
OkJsonNumber  *temp  = okj_get_number(&parser,  "temp");
OkJsonString  *unit  = okj_get_string(&parser,  "unit");
OkJsonBoolean *valid = okj_get_boolean(&parser, "valid");

if (temp  != NULL) { /* temp->start points to "42", temp->length == 2  */ }
if (unit  != NULL) { /* unit->start points to "C",  unit->length == 1  */ }
if (valid != NULL) { /* valid->start points to "true", valid->length == 4 */ }
```

All getter functions return `NULL` when the key is not found or when the value
type does not match the requested type.

## Building

```sh
make        # builds ok_json.a and runs the test suite
make clean  # removes build artifacts
```

Requires a C99-capable compiler. Tested with GCC using
`-Wall -Wextra -Werror -pedantic`.

## Limits

| Constant            | Default | Description                  |
|---------------------|---------|------------------------------|
| `OKJ_MAX_TOKENS`    | 128     | Maximum tokens per parse     |
| `OKJ_MAX_STRING_LEN`| 64      | Maximum key/string length    |
| `OKJ_MAX_ARRAY_SIZE`| 64      | Maximum array element count  |
| `OKJ_MAX_OBJECT_SIZE`| 64     | Maximum object member count  |

All limits are compile-time constants.

## Wiki

A full local wiki is available in the [`wiki/`](wiki/) directory:

- [Home](wiki/Home.md)
- [Project Overview](wiki/Project-Overview.md)
- [Design Philosophy](wiki/Design-Philosophy.md)
- [API Reference](wiki/API-Reference.md)
- [Usage Guide](wiki/Usage-Guide.md)
- [Development and Testing](wiki/Development-and-Testing.md)

## Contributing

If you've found a bug or improvement, feel free to submit an update! Here's how:

1. Create a new branch off master for your changes.
2. Clone the repository to your local development machine.
3. Make your updates and save the files.
4. Add, commit, and push your changes back to the repository.
5. Create a new merge request so I can review your code.

Once everything looks good with no errors or formatting issues, I'll approve
and merge your code.

## License

Copyright (c) 2026 Rich Morgan

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
