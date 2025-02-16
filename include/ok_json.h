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

/* Fixed-size token array       */
#define OKJ_MAX_TOKENS      128

/* Maximum key or string length */
#define OKJ_MAX_STRING_LEN  64

/* Maximum size of array to process */
#define OKJ_MAX_ARRAY_SIZE  64

/* Maximum size of array to process */
#define OKJ_MAX_OBJECT_SIZE  64

/* JSON Error Codes */
typedef enum
{
    OKJ_SUCCESS                    = 0,
    OKJ_ERROR_INVALID_CHARACTER    = 1,
    OKJ_ERROR_SYNTAX               = 2,
    OKJ_ERROR_OVERFLOW             = 3,
    OKJ_ERROR_UNEXPECTED_END       = 4,
    OKJ_ERROR_MAX_TOKENS_EXCEEDED  = 5,
    OKJ_ERROR_MAX_STR_LEN_EXCEEDED = 6,
    OKJ_ERROR_BAD_POINTER          = 7,
    OKJ_ERROR_BAD_NUMBER           = 8,
    OKJ_ERROR_BAD_OBJECT           = 9,
    OKJ_ERROR_BAD_STRING           = 10,
    OKJ_ERROR_BAD_ARRAY            = 11,
    OKJ_ERROR_BAD_BOOLEAN          = 12,
    OKJ_ERROR_NULL_PARSER_OBJ      = 13,
    OKJ_ERROR_INVALID_TYPE_ENUM    = 14,
    OKJ_ERROR_NO_FREE_SPACE        = 15,
    OKJ_ERROR_PARSING_FAILED       = 16
} OkjError;

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

/* JSON Object Structure */
typedef struct
{
    char *start;      /* Pointer to start of token in JSON string */
    int count;        /* Total count of object members            */
} OkJsonObject;

/* JSON Array Structure */
typedef struct
{
    char *start;      /* Pointer to start of token in JSON string */
    int count;        /* Total count of array elements            */
} OkJsonArray;

/* JSON Boolean Structure */
typedef struct
{
    char *start;      /* Pointer to start of token in JSON string */
    int length;       /* Size of boolean in bytes                 */
} OkJsonBoolean;

/* JSON Number Structure */
typedef struct
{
    char *start;      /* Pointer to start of token in JSON string */
    int length;       /* Size of boolean in bytes                 */
} OkJsonNumber;

/* JSON String Structure */
typedef struct
{
    char *start;      /* Pointer to start of token in JSON string */
    int length;       /* Size of string in bytes                  */
} OkJsonString;

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
int okj_get_number(OkJsonParser *parser, const char *key);
int okj_get_boolean(OkJsonParser *parser, const char *key);
int okj_get_array(OkJsonParser *parser, const char *key);
int okj_get_object(OkJsonParser *parser, const char *key);
int okj_get_token(OkJsonParser *parser, const char *key);

#endif  /* OK_JSON_H */
