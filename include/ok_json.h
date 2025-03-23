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

/**
 * @brief If you prefer to use the types defined in your platform's stdint.h, uncomment
 * this define.  Otherwise, we'll define our own types per MISRA C2012 Dir 4.6.
 * #define OK_JSON_USE_STDINT_H
 **/
#ifdef OK_JSON_USE_STDINT_H
#include <stdint.h>
#else
typedef unsigned long long uint64_t;
typedef unsigned int       uint32_t;
typedef unsigned short     uint16_t;
typedef unsigned char      uint8_t;
#endif

/**
 * @brief Fixed-size token array
 **/
#define OKJ_MAX_TOKENS 128U
/* static const uint16_t OKJ_MAX_TOKENS = 128U; */

/**
 * @brief Maximum key or string length
 **/
static const uint16_t OKJ_MAX_STRING_LEN = 64U;

/**
 * @brief Maximum size of array to process
 **/
static const uint16_t OKJ_MAX_ARRAY_SIZE = 64U;

/**
 * @brief Maximum size of array to process
 **/
static const uint16_t OKJ_MAX_OBJECT_SIZE = 64U;

/**
 * @brief Array of all valid ASCII characters for string processing
 **/
static const char OKJ_VALID_CHARS[96] = {
        ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+',
        ',', '-', '.', '/', '0', '1', '2',  '3', '4', '5', '6', '7',
        '8', '9', ':', ';', '<', '=', '>',  '?', '@', 'A', 'B', 'C',
        'D', 'E', 'F', 'G', 'H', 'I', 'J',  'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T', 'U', 'V',  'W', 'X', 'Y', 'Z', '[',
        '\\',']', '^', '_', '`', 'a', 'b',  'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n',  'o', 'p', 'q', 'r', 's',
        't', 'u', 'v', 'w', 'x', 'y', 'z',  '{', '|', '}', '~', '\0'
};

/**
 * @brief OK_JSON error/return codes
 **/
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

/**
 * @brief OK_JSON token types
 **/
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

/**
 * @brief OK_JSON token structure
 **/
typedef struct
{
    OkJsonType type;
    char *start;           /* Pointer to start of token in JSON string */
    uint16_t length;       /* Token length in bytes                    */
} OkJsonToken;

/**
 * @brief OK_JSON object structure
 **/
typedef struct
{
    char *start;           /* Pointer to start of token in JSON string */
    uint16_t count;        /* Total count of object members            */
} OkJsonObject;

/**
 * @brief JSON array structure
 **/
typedef struct
{
    char *start;           /* Pointer to start of token in JSON string */
    uint16_t count;        /* Total count of array elements            */
} OkJsonArray;

/**
 * @brief OK_JSON boolean structure
 **/
typedef struct
{
    char *start;           /* Pointer to start of token in JSON string */
    uint16_t length;       /* Size of boolean in bytes                 */
} OkJsonBoolean;

/**
 * @brief OK_JSON number structure
 **/
typedef struct
{
    char *start;           /* Pointer to start of token in JSON string */
    uint16_t length;       /* Size of number in bytes                  */
} OkJsonNumber;

/**
 * @brief OK_JSON string structure
 **/
typedef struct
{
    char *start;           /* Pointer to start of token in JSON string */
    uint16_t length;       /* Size of string in bytes                  */
} OkJsonString;

/**
 * @brief OK_JSON parser structure
 **/
typedef struct
{
    OkJsonToken tokens[OKJ_MAX_TOKENS];      /* Fixed-size token storage     */
    uint16_t token_count;                    /* Number of parsed tokens      */
    char *json;                              /* Pointer to input JSON string */
    uint16_t position;                       /* Current parsing position     */
} OkJsonParser;


/**
 * @brief OK_JSON initialization routine
 *
 * @param parser Pointer to the main ok_json parser object
 * @param json_string Character stream of data you want to parse 
 **/
void okj_init(OkJsonParser *parser, char *json_string);

/**
 * @brief OK_JSON parse routine
 *
 * @param parser Pointer to the main ok_json parser object
 * @return OkjError OK_JSON error code result of the operation
 **/
OkjError okj_parse(OkJsonParser *parser);

/**
 * @brief 
 * @param parser Pointer to the main ok_json parser object
 * @param key 
 * @return OkJsonString
 **/
OkJsonString *okj_get_string(OkJsonParser *parser, const char *key);

/**
 * @brief 
 * @param parser 
 * @param key 
 * @return OkJsonNumber
 **/
OkJsonNumber *okj_get_number(OkJsonParser *parser, const char *key);

/**
 * @brief 
 * @param parser 
 * @param key 
 * @return OkJsonBoolean
 **/
OkJsonBoolean *okj_get_boolean(OkJsonParser *parser, const char *key);

/**
 * @brief 
 * @param parser 
 * @param key 
 * @return OkJsonArray
 **/
OkJsonArray *okj_get_array(OkJsonParser *parser, const char *key);

/**
 * @brief 
 * @param parser 
 * @param key 
 * @return OkJsonObject
 **/
OkJsonObject *okj_get_object(OkJsonParser *parser, const char *key);

/**
 * @brief 
 * @param parser 
 * @param key 
 * @return OkJsonToken
 **/
OkJsonToken *okj_get_token(OkJsonParser *parser, const char *key);

#endif  /* OK_JSON_H */
