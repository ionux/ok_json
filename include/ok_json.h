/**********************************************************************************
 * This file is part of the OK_JSON project. You can always find the latest
 * version of this file and project at: https://github.com/ionux/ok_json/
 *
 * Copyright (c) 2026 Rich Morgan
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
/**
 * Only using shorts and chars for now.  If you need larger data types, define them here.
 * For example: "typedef unsigned long long uint64_t" or "typedef unsigned int uint32_t".
 */
typedef unsigned short  uint16_t;
typedef unsigned char   uint8_t;
#endif

/**
 * @brief NULL is part of the C standard but lives in stddef.h which we avoid
 * pulling in.  Define it here if the platform header hasn't provided it.
 **/
#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * @brief Fixed-size token array limit. Defined as a preprocessor macro so it
 * can be used as an array dimension in struct definitions.
 **/
#define OKJ_MAX_TOKENS 128U

/**
 * @brief Maximum nesting depth for JSON containers (objects and arrays).
 * Defined as a preprocessor macro so it can be used as an array dimension in
 * struct definitions.  The parser rejects input that would exceed this many
 * concurrently open containers (RFC 8259 imposes no limit, but this bound
 * protects embedded targets from unbounded stack growth).
 **/
#define OKJ_MAX_DEPTH 16U

/**
 * @brief Maximum key or string length
 **/
static const uint16_t OKJ_MAX_STRING_LEN = 64U;

/**
 * @brief Maximum size of array to process
 **/
static const uint16_t OKJ_MAX_ARRAY_SIZE = 64U;

/**
 * @brief Maximum number of key-value members in an object to process.
 * Note: must stay below (OKJ_MAX_TOKENS - 1) / 2 so that a fully-parsed
 * object never exhausts the token budget before the member limit fires.
 **/
static const uint16_t OKJ_MAX_OBJECT_SIZE = 32U;

/**
 * @brief Maximum length of the raw JSON input string in bytes (excluding the
 * terminating NUL character).  Inputs longer than this are rejected by
 * okj_parse() before any tokenisation begins.
 **/
static const uint16_t OKJ_MAX_JSON_LEN = 4096U;

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
 * @brief Grammar context used internally to validate token sequence during
 * parsing.  Tracks what the parser currently expects next so that structural
 * errors (trailing commas, missing colons, non-string object keys, etc.) are
 * detected immediately rather than after the fact.
 **/
typedef enum
{
    OKJ_CTX_WANT_VALUE,          /* Expecting any JSON value (no close yet)        */
    OKJ_CTX_WANT_VALUE_OR_CLOSE, /* Expecting a value OR ']' (just opened array)   */
    OKJ_CTX_WANT_KEY,            /* Expecting an object key string (no close)      */
    OKJ_CTX_WANT_KEY_OR_CLOSE,   /* Expecting a key string OR '}' (just opened {}) */
    OKJ_CTX_WANT_COLON,          /* Expecting ':' after an object key              */
    OKJ_CTX_WANT_SEP_OR_CLOSE    /* Expecting ',' or the matching close bracket    */
} OkjParseContext;

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
    OKJ_ERROR_PARSING_FAILED       = 16,
    OKJ_ERROR_MAX_JSON_LEN_EXCEEDED = 17,
    OKJ_ERROR_MAX_DEPTH_EXCEEDED   = 18,
    OKJ_ERROR_BRACKET_MISMATCH     = 19
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
    uint16_t length;       /* Full raw text length including braces    */
} OkJsonObject;

/**
 * @brief JSON array structure
 **/
typedef struct
{
    char *start;           /* Pointer to start of token in JSON string */
    uint16_t count;        /* Total count of array elements            */
    uint16_t length;       /* Full raw text length including brackets  */
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
    OkJsonToken    tokens[OKJ_MAX_TOKENS];      /* Fixed-size token storage     */
    OkJsonType     depth_stack[OKJ_MAX_DEPTH];  /* Container-type at each depth */
    OkjParseContext context;                    /* Current grammar expectation  */
    uint16_t       token_count;                 /* Number of parsed tokens      */
    uint16_t       depth;                       /* Current nesting depth        */
    char          *json;                        /* Pointer to input JSON string */
    uint16_t       position;                    /* Current parsing position     */
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
 * @brief Retrieve the string value associated with a key in a parsed object.
 *        Scans the token array for a STRING token whose content matches @p key,
 *        then returns the immediately following token cast to OkJsonString.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to a static OkJsonString containing start and length of the
 *         string content (quotes excluded), or NULL if not found or type mismatch
 **/
OkJsonString *okj_get_string(OkJsonParser *parser, const char *key);

/**
 * @brief Retrieve the number value associated with a key in a parsed object.
 *        Scans the token array for a STRING token whose content matches @p key,
 *        then returns the immediately following token cast to OkJsonNumber.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to a static OkJsonNumber containing start and length of the
 *         numeric text, or NULL if not found or type mismatch
 **/
OkJsonNumber *okj_get_number(OkJsonParser *parser, const char *key);

/**
 * @brief Retrieve the boolean value associated with a key in a parsed object.
 *        Scans the token array for a STRING token whose content matches @p key,
 *        then returns the immediately following token cast to OkJsonBoolean.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to a static OkJsonBoolean containing start and length of the
 *         boolean literal ("true" or "false"), or NULL if not found or type mismatch
 **/
OkJsonBoolean *okj_get_boolean(OkJsonParser *parser, const char *key);

/**
 * @brief Retrieve the array value associated with a key in a parsed object.
 *        Scans the token array for a STRING token whose content matches @p key,
 *        then returns the immediately following token cast to OkJsonArray.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to a static OkJsonArray containing start of the array, or
 *         NULL if not found or type mismatch
 **/
OkJsonArray *okj_get_array(OkJsonParser *parser, const char *key);

/**
 * @brief Retrieve the object value associated with a key in a parsed object.
 *        Scans the token array for a STRING token whose content matches @p key,
 *        then returns the immediately following token cast to OkJsonObject.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to a static OkJsonObject containing start of the object, or
 *         NULL if not found or type mismatch
 **/
OkJsonObject *okj_get_object(OkJsonParser *parser, const char *key);

/**
 * @brief Retrieve the raw token for the value associated with a key.
 *        Scans the token array for a STRING token whose content matches @p key,
 *        then returns a pointer directly into the parser's token array for the
 *        immediately following token.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to the OkJsonToken in the parser's token array, or NULL if
 *         not found
 **/
OkJsonToken *okj_get_token(OkJsonParser *parser, const char *key);

/**
 * @brief Retrieve the entire raw array value associated with a key.
 *        Like okj_get_array() but also populates the @c length field with the
 *        full byte count of the array text (including surrounding brackets) and
 *        does NOT enforce OKJ_MAX_ARRAY_SIZE.  Use this when you need the
 *        complete raw text of an array regardless of its element count.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to a static OkJsonArray with start, count, and length
 *         populated, or NULL if not found or type mismatch
 **/
OkJsonArray *okj_get_array_raw(OkJsonParser *parser, const char *key);

/**
 * @brief Retrieve the entire raw object value associated with a key.
 *        Like okj_get_object() but also populates the @c length field with the
 *        full byte count of the object text (including surrounding braces) and
 *        does NOT enforce OKJ_MAX_OBJECT_SIZE.  Use this when you need the
 *        complete raw text of an object regardless of its member count.
 * @param parser Pointer to the main ok_json parser object
 * @param key    Null-terminated key name to look up
 * @return Pointer to a static OkJsonObject with start, count, and length
 *         populated, or NULL if not found or type mismatch
 **/
OkJsonObject *okj_get_object_raw(OkJsonParser *parser, const char *key);

/**
 * @brief Copy a parsed string value into a caller-supplied buffer with
 *        guaranteed null-termination.  At most @p buf_size - 1 bytes of
 *        string content are copied; the buffer is always null-terminated
 *        when @p buf_size >= 1.
 * @param str      Pointer to an OkJsonString returned by okj_get_string()
 * @param buf      Destination buffer supplied by the caller
 * @param buf_size Total size of @p buf in bytes (must be >= 1)
 * @return Number of characters copied (excluding the null terminator), or 0
 *         on error (NULL inputs or buf_size == 0)
 **/
uint16_t okj_copy_string(const OkJsonString *str, char *buf, uint16_t buf_size);

/**
 * @brief Return the total number of OKJ_OBJECT tokens in the parsed result.
 *        Counts every object opening brace that was tokenised, including
 *        nested objects.
 * @param parser Pointer to the main ok_json parser object
 * @return Count of OKJ_OBJECT tokens, or 0 if @p parser is NULL
 **/
uint16_t okj_count_objects(const OkJsonParser *parser);

/**
 * @brief Return the total number of OKJ_ARRAY tokens in the parsed result.
 *        Counts every array opening bracket that was tokenised, including
 *        nested arrays.
 * @param parser Pointer to the main ok_json parser object
 * @return Count of OKJ_ARRAY tokens, or 0 if @p parser is NULL
 **/
uint16_t okj_count_arrays(const OkJsonParser *parser);

/**
 * @brief Return the total number of tokens (elements of any type) produced
 *        by a successful parse.  Equivalent to @c parser->token_count.
 * @param parser Pointer to the main ok_json parser object
 * @return Total token count, or 0 if @p parser is NULL
 **/
uint16_t okj_count_elements(const OkJsonParser *parser);

/**
 * @brief Print a human-readable debug dump of every token in @p parser to
 *        stdout.  Only available when compiled with -DOK_JSON_DEBUG.
 * @param parser Pointer to the main ok_json parser object
 **/
#ifdef OK_JSON_DEBUG
void okj_debug_print(OkJsonParser *parser);
#endif /* OK_JSON_DEBUG */

#endif  /* OK_JSON_H */
