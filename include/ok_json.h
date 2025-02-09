/**********************************************************************************
 * This file is part of the OK_JSON project. You can always find the latest
 * version of this file and project at: https://github.com/ionux/ok_json/
 *
 * Copyright (c) 2025 Rich Morgan
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/

#ifndef OK_JSON_H
#define OK_JSON_H

#include <stdint.h>

#define OKJ_MAX_TOKENS      32      /* Fixed-size token array       */
#define OKJ_MAX_STRING_LEN  64      /* Maximum key or string length */

/* JSON Token Types */
typedef enum
{
    OKJ_UNDEFINED,
    OKJ_OBJECT,
    OKJ_ARRAY,
    OKJ_STRING,
    OKJ_NUMBER,
    OKJ_BOOLEAN,
    OKJ_NULL
} OkJsonType;

/* JSON Token Structure */
typedef struct
{
    OkJsonType type;
    char *start;      /* Pointer to start of token in JSON string */
    int length;       /* Token length in bytes                    */
} OkJsonToken;

/* JSON Parser Structure */
typedef struct
{
    OkJsonToken tokens[OKJ_MAX_TOKENS];  /* Fixed-size token storage     */
    int token_count;                     /* Number of parsed tokens      */
    const char *json;                    /* Pointer to input JSON string */
    int position;                        /* Current parsing position     */
} OkJsonParser;


void okj_init(OkJsonParser *parser, const char *json_string);
int okj_parse(OkJsonParser *parser);
const char *okj_get_string(OkJsonParser *parser, const char *key);
int okj_get_int(OkJsonParser *parser, const char *key);
int okj_get_boolean(OkJsonParser *parser, const char *key);

#endif  /* OK_JSON_H */
