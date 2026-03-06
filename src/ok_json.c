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

#include "../include/ok_json.h"


/* ---------------------------------------------------------------------------
 * Internal helpers — no external library dependencies
 * ---------------------------------------------------------------------------*/

static uint8_t okj_is_whitespace(char c)
{
    return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

static uint8_t okj_is_digit(char c)
{
    return (c >= '0') && (c <= '9');
}

static uint8_t okj_is_hex_digit(char c)
{
    return ((c >= '0') && (c <= '9')) ||
           ((c >= 'a') && (c <= 'f')) ||
           ((c >= 'A') && (c <= 'F'));
}

/* Returns 1 if the first `len` bytes of `src` equal `lit`, 0 otherwise.
 * Stops early on a NUL byte in `src` to avoid overreads. */
static uint8_t okj_match(const char *src, const char *lit, uint16_t len)
{
    uint16_t i = 0U;
    uint8_t result = 1U;

    for (i = 0U; i < len; i++)
    {
        if ((src[i] == '\0') || (src[i] != lit[i]))
        {
            result = 0U;
        }
    }

    return result;
}

/* ---------------------------------------------------------------------------
 * Helpers for counting array elements and object members in the raw JSON
 * string.  These walk the source text so the counts are accurate even when
 * multiple values appear at the same nesting level.
 * ---------------------------------------------------------------------------*/

/* Advance `p` past a JSON string (including both surrounding quotes).
 * `p` must point at the opening '"'.  Returns a pointer to the character
 * immediately following the closing '"', or to '\0' on unterminated input.
 * Recognises backslash escapes so that \" inside a string does not
 * prematurely terminate the scan. */
static const char *okj_skip_string(const char *p)
{
    p++;    /* skip opening '"' */

    while ((*p != '"') && (*p != '\0'))
    {
        if (*p == '\\')
        {
            p++;    /* skip the backslash */

            if (*p == '\0')
            {
                break;  /* truncated input: backslash at end of stream */
            }
        }

        p++;
    }

    if (*p == '"')
    {
        p++;    /* skip closing '"' */
    }

    return p;
}

/* Count the number of elements in a JSON array whose text begins at `start`
 * (which must point to the '[' character).  Handles nested arrays and objects
 * correctly by tracking bracket depth, and skips string content to avoid
 * counting structural characters that appear inside quoted values. */
static uint16_t okj_count_array_elements(const char *start)
{
    uint16_t    depth = 0U;
    uint16_t    count = 0U;
    uint8_t     seen  = 0U;    /* set when first non-whitespace element is found */
    const char *p     = start;

    if ((p == NULL) || (*p != '['))
    {
        return 0U;
    }

    p++;        /* skip '[' */
    depth = 1U;

    while ((*p != '\0') && (depth > 0U))
    {
        char c = *p;

        if (c == '"')
        {
            /* Skip string content so commas inside strings are not counted. */
            p = okj_skip_string(p);

            if ((depth == 1U) && (seen == 0U))
            {
                seen = 1U;
            }
        }
        else
        {
            if ((c == '[') || (c == '{'))
            {
                if ((depth == 1U) && (seen == 0U))
                {
                    seen = 1U;
                }
                depth++;
            }
            else if ((c == ']') || (c == '}'))
            {
                depth--;
            }
            else if ((c == ',') && (depth == 1U))
            {
                count++;
            }
            else if ((!okj_is_whitespace(c)) && (depth == 1U) && (seen == 0U))
            {
                seen = 1U;
            }
            else
            {
                /* Other characters (digits, letters, etc.) need no action. */
            }

            p++;
        }
    }

    /* The comma count equals elements-minus-one for a non-empty array. */
    if (seen != 0U)
    {
        count++;
    }

    return count;
}

/* Count the number of key-value members in a JSON object whose text begins
 * at `start` (which must point to the '{' character).  Each colon at depth 1
 * corresponds to exactly one member. */
static uint16_t okj_count_object_members(const char *start)
{
    uint16_t    depth = 0U;
    uint16_t    count = 0U;
    const char *p     = start;

    if ((p == NULL) || (*p != '{'))
    {
        return 0U;
    }

    p++;        /* skip '{' */
    depth = 1U;

    while ((*p != '\0') && (depth > 0U))
    {
        char c = *p;

        if (c == '"')
        {
            /* Skip string content so colons inside keys/values are ignored. */
            p = okj_skip_string(p);
        }
        else
        {
            if ((c == '[') || (c == '{'))
            {
                depth++;
            }
            else if ((c == ']') || (c == '}'))
            {
                depth--;
            }
            else if ((c == ':') && (depth == 1U))
            {
                count++;
            }
            else
            {
                /* Other characters need no action. */
            }

            p++;
        }
    }

    return count;
}

/* Measure the full byte length of a JSON array or object starting at `start`
 * (which must point to '[' or '{').  Returns the count of bytes from the
 * opening bracket to the matching closing bracket, inclusive.  String content
 * is skipped so that structural characters inside quoted values are ignored.
 * Returns 0 if `start` is NULL or does not begin with '[' or '{'. */
static uint16_t okj_measure_container(const char *start)
{
    uint16_t    depth  = 0U;
    uint16_t    length = 0U;
    const char *p      = start;

    if ((p == NULL) || ((*p != '[') && (*p != '{')))
    {
        return 0U;
    }

    while (*p != '\0')
    {
        char c = *p;
        length++;

        if (c == '"')
        {
            /* Skip past the opening quote (already counted above). */
            p++;

            while ((*p != '\0') && (*p != '"'))
            {
                if (*p == '\\')
                {
                    p++;
                    if (*p != '\0')
                    {
                        length++;
                        p++;
                    }
                }
                else
                {
                    p++;
                }
                length++;
            }

            /* Count the closing quote if present, then continue. */
            if (*p == '"')
            {
                length++;
                p++;
            }
        }
        else
        {
            if ((c == '[') || (c == '{'))
            {
                depth++;
            }
            else if ((c == ']') || (c == '}'))
            {
                depth--;
            }
            else
            {
                /* whitespace, digits, colons, commas, letters, etc. */
            }

            p++;

            if (depth == 0U)
            {
                break;
            }
        }
    }

    return length;
}

/* ---------------------------------------------------------------------------
 * File-scope result structs returned by getter functions.
 * Avoids dynamic allocation (suitable for embedded targets).
 * ---------------------------------------------------------------------------*/

static OkJsonString  s_string_result;
static OkJsonNumber  s_number_result;
static OkJsonBoolean s_boolean_result;
static OkJsonArray   s_array_result;
static OkJsonObject  s_object_result;
static OkJsonArray   s_full_array_result;
static OkJsonObject  s_full_object_result;

/* ---------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------------*/

void okj_init(OkJsonParser *parser, char *json_string)
{
    if ((parser != NULL) && (json_string != NULL))
    {
        uint16_t i = 0U;

        for (i = 0U; i < OKJ_MAX_TOKENS; i++)
        {
            parser->tokens[i].type   = OKJ_UNDEFINED;
            parser->tokens[i].start  = NULL;
            parser->tokens[i].length = 0U;
        }

        parser->json        = json_string;
        parser->position    = 0U;
        parser->token_count = 0U;
    }
}

static void okj_skip_whitespace(OkJsonParser *parser)
{
    if (parser != NULL)
    {
        while (okj_is_whitespace(parser->json[parser->position]))
        {
            parser->position++;
        }
    }
}

static OkjError okj_parse_value(OkJsonParser *parser)
{
    OkjError    result    = OKJ_SUCCESS;
    char        c         = '\0';
    OkJsonToken *tok      = NULL;
    uint16_t    start_pos = 0U;

    if (parser == NULL)
    {
        return OKJ_ERROR_BAD_POINTER;
    }

    okj_skip_whitespace(parser);

    c = parser->json[parser->position];

    if (c == '\0')
    {
        /* End of input — nothing to do. */
    }
    else if (c == '{')
    {
        tok         = &parser->tokens[parser->token_count];
        tok->type   = OKJ_OBJECT;
        tok->start  = &parser->json[parser->position];
        tok->length = 1U;
        parser->position++;
        parser->token_count++;
    }
    else if (c == '[')
    {
        tok         = &parser->tokens[parser->token_count];
        tok->type   = OKJ_ARRAY;
        tok->start  = &parser->json[parser->position];
        tok->length = 1U;

        parser->position++;
        parser->token_count++;
    }
    else if ((c == '}') || (c == ']') || (c == ',') || (c == ':'))
    {
        /* Structural punctuation — advance past it, emit no token. */
        parser->position++;
    }
    else if (c == '"')
    {
        tok        = &parser->tokens[parser->token_count];
        tok->type  = OKJ_STRING;
        tok->start = &parser->json[parser->position + 1U];  /* skip opening '"' */

        start_pos  = parser->position + 1U;

        parser->position++;

        while ((parser->json[parser->position] != '"') &&
               (parser->json[parser->position] != '\0'))
        {
            if ((parser->position - start_pos) >= OKJ_MAX_STRING_LEN)
            {
                break;
            }

            if (parser->json[parser->position] == '\\')
            {
                char esc_char;

                parser->position++;     /* consume backslash */

                esc_char = parser->json[parser->position];

                if (esc_char == '\0')
                {
                    break;  /* truncated input: backslash at end of stream */
                }
                else if ((esc_char == '"')  || (esc_char == '\\') ||
                         (esc_char == '/')  || (esc_char == 'b')  ||
                         (esc_char == 'f')  || (esc_char == 'n')  ||
                         (esc_char == 'r')  || (esc_char == 't'))
                {
                    parser->position++;     /* consume the escape character */
                }
                else if (esc_char == 'u')
                {
                    uint16_t h;

                    parser->position++;     /* consume 'u' */

                    for (h = 0U; h < 4U; h++)
                    {
                        if (okj_is_hex_digit(parser->json[parser->position]) == 0)
                        {
                            result = OKJ_ERROR_BAD_STRING;
                            break;
                        }

                        parser->position++;
                    }
                }
                else
                {
                    result = OKJ_ERROR_BAD_STRING;
                }

                if (result != OKJ_SUCCESS)
                {
                    break;
                }
            }
            else
            {
                parser->position++;
            }
        }

        if (result != OKJ_SUCCESS)
        {
            /* Error set inside loop (e.g. invalid escape sequence). */
        }
        else if ((parser->json[parser->position] != '"') &&
                 (parser->json[parser->position] != '\0'))
        {
            /* Loop exited due to the length limit, not a closing quote. */
            result = OKJ_ERROR_MAX_STR_LEN_EXCEEDED;
        }
        else if (parser->json[parser->position] != '"')
        {
            /* Position is at '\0': input ended before the closing quote. */
            result = OKJ_ERROR_UNEXPECTED_END;
        }
        else
        {
            tok->length = parser->position - start_pos;

            parser->position++;   /* advance past closing '"' */
            parser->token_count++;
        }
    }
    else if ((okj_is_digit(c)) || (c == '-'))
    {
        uint8_t number_ok = 1U;

        tok        = &parser->tokens[parser->token_count];
        tok->type  = OKJ_NUMBER;
        tok->start = &parser->json[parser->position];

        start_pos  = parser->position;

        /* Step 1: optional leading minus */
        if (c == '-')
        {
            parser->position++;     /* consume '-' */

            if (okj_is_digit(parser->json[parser->position]) == 0)
            {
                number_ok = 0U;     /* bare minus is not a valid number */
            }
        }

        /* Step 2: integer part — zero OR digit1-9 *DIGIT */
        if (number_ok != 0U)
        {
            if (parser->json[parser->position] == '0')
            {
                parser->position++;     /* consume '0' */

                if (okj_is_digit(parser->json[parser->position]) != 0)
                {
                    number_ok = 0U;     /* leading zero: "012" is invalid */
                }
            }
            else
            {
                while (okj_is_digit(parser->json[parser->position]) != 0)
                {
                    parser->position++;
                }
            }
        }

        /* Step 3: optional fractional part — '.' 1*DIGIT */
        if ((number_ok != 0U) && (parser->json[parser->position] == '.'))
        {
            parser->position++;     /* consume '.' */

            if (okj_is_digit(parser->json[parser->position]) == 0)
            {
                number_ok = 0U;     /* decimal point must be followed by a digit */
            }
            else
            {
                while (okj_is_digit(parser->json[parser->position]) != 0)
                {
                    parser->position++;
                }
            }
        }

        /* Step 4: optional exponent part — ('e'/'E') [sign] 1*DIGIT */
        if ((number_ok != 0U) &&
            ((parser->json[parser->position] == 'e') ||
             (parser->json[parser->position] == 'E')))
        {
            parser->position++;     /* consume 'e' or 'E' */

            if ((parser->json[parser->position] == '+') ||
                (parser->json[parser->position] == '-'))
            {
                parser->position++;     /* consume optional sign */
            }

            if (okj_is_digit(parser->json[parser->position]) == 0)
            {
                number_ok = 0U;     /* exponent requires at least one digit */
            }
            else
            {
                while (okj_is_digit(parser->json[parser->position]) != 0)
                {
                    parser->position++;
                }
            }
        }

        if (number_ok != 0U)
        {
            tok->length = parser->position - start_pos;

            parser->token_count++;
        }
        else
        {
            result = OKJ_ERROR_BAD_NUMBER;
        }
    }
    else if (okj_match(&parser->json[parser->position], "true", 4U))
    {
        /* TODO: Should we check for other forms like TRUE and True? */

        tok         = &parser->tokens[parser->token_count];
        tok->type   = OKJ_BOOLEAN;
        tok->start  = &parser->json[parser->position];
        tok->length = 4U;

        parser->position += 4U;
        parser->token_count++;
    }
    else if (okj_match(&parser->json[parser->position], "false", 5U))
    {
        /* TODO: Should we check for other forms like FALSE and False? */

        tok         = &parser->tokens[parser->token_count];
        tok->type   = OKJ_BOOLEAN;
        tok->start  = &parser->json[parser->position];
        tok->length = 5U;

        parser->position += 5U;
        parser->token_count++;
    }
    else if (okj_match(&parser->json[parser->position], "null", 4U))
    {
        /* TODO: Should we check for other forms like NULL and Null? */
        
        tok         = &parser->tokens[parser->token_count];
        tok->type   = OKJ_NULL;
        tok->start  = &parser->json[parser->position];
        tok->length = 4U;

        parser->position += 4U;
        parser->token_count++;
    }
    else
    {
        result = OKJ_ERROR_SYNTAX;
    }

    return result;
}

OkjError okj_parse(OkJsonParser *parser)
{
    OkjError result  = OKJ_SUCCESS;
    uint16_t json_len = 0U;

    if (parser == NULL)
    {
        return OKJ_ERROR_BAD_POINTER;
    }

    while (parser->json[json_len] != '\0')
    {
        json_len++;

        if (json_len > OKJ_MAX_JSON_LEN)
        {
            return OKJ_ERROR_MAX_JSON_LEN_EXCEEDED;
        }
    }

    while ((parser->json[parser->position] != '\0') &&
           (parser->token_count < OKJ_MAX_TOKENS))
    {
        result = okj_parse_value(parser);

        if (result != OKJ_SUCCESS)
        {
            break;
        }
    }

    if ((result == OKJ_SUCCESS)                      &&
        (parser->token_count >= OKJ_MAX_TOKENS)      &&
        (parser->json[parser->position] != '\0'))
    {
        result = OKJ_ERROR_MAX_TOKENS_EXCEEDED;
    }

    return result;
}

/* ---------------------------------------------------------------------------
 * Internal lookup helper
 * ---------------------------------------------------------------------------*/

/* Scans the token array for a STRING token whose content equals `key`.
 * Returns the index of the NEXT token (the value), or OKJ_MAX_TOKENS if
 * not found. */
static uint16_t okj_find_value_index(OkJsonParser *parser, const char *key)
{
    uint16_t i       = 0U;
    uint16_t key_len = 0U;

    while ((key[key_len] != '\0') && (key_len <= OKJ_MAX_STRING_LEN))
    {
        key_len++;
    }

    for (i = 0U; (i + 1U) < parser->token_count; i++)
    {
        OkJsonToken *t = &parser->tokens[i];

        if ((t->type   == OKJ_STRING) &&
            (t->length == key_len)    &&
            (okj_match(t->start, key, key_len)))
        {
            return i + 1U;
        }
    }

    return OKJ_MAX_TOKENS;
}

/* ---------------------------------------------------------------------------
 * Getter functions
 * ---------------------------------------------------------------------------*/

OkJsonString *okj_get_string(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_STRING))
    {
        return NULL;
    }

    s_string_result.start  = parser->tokens[idx].start;
    s_string_result.length = parser->tokens[idx].length;

    return &s_string_result;
}

OkJsonNumber *okj_get_number(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_NUMBER))
    {
        return NULL;
    }

    s_number_result.start  = parser->tokens[idx].start;
    s_number_result.length = parser->tokens[idx].length;

    return &s_number_result;
}

OkJsonBoolean *okj_get_boolean(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_BOOLEAN))
    {
        return NULL;
    }

    s_boolean_result.start  = parser->tokens[idx].start;
    s_boolean_result.length = parser->tokens[idx].length;

    return &s_boolean_result;
}

OkJsonArray *okj_get_array(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_ARRAY))
    {
        return NULL;
    }

    s_array_result.start  = parser->tokens[idx].start;
    s_array_result.count  = okj_count_array_elements(parser->tokens[idx].start);
    s_array_result.length = okj_measure_container(parser->tokens[idx].start);

    if (s_array_result.count > OKJ_MAX_ARRAY_SIZE)
    {
        return NULL;
    }

    return &s_array_result;
}

OkJsonObject *okj_get_object(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_OBJECT))
    {
        return NULL;
    }

    s_object_result.start  = parser->tokens[idx].start;
    s_object_result.count  = okj_count_object_members(parser->tokens[idx].start);
    s_object_result.length = okj_measure_container(parser->tokens[idx].start);

    if (s_object_result.count > OKJ_MAX_OBJECT_SIZE)
    {
        return NULL;
    }

    return &s_object_result;
}

OkJsonToken *okj_get_token(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if (idx == OKJ_MAX_TOKENS)
    {
        return NULL;
    }

    return &parser->tokens[idx];
}

OkJsonArray *okj_get_array_raw(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_ARRAY))
    {
        return NULL;
    }

    s_full_array_result.start  = parser->tokens[idx].start;
    s_full_array_result.count  = okj_count_array_elements(parser->tokens[idx].start);
    s_full_array_result.length = okj_measure_container(parser->tokens[idx].start);

    return &s_full_array_result;
}

OkJsonObject *okj_get_object_raw(OkJsonParser *parser, const char *key)
{
    uint16_t idx = 0U;

    if ((parser == NULL) || (key == NULL))
    {
        return NULL;
    }

    idx = okj_find_value_index(parser, key);

    if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_OBJECT))
    {
        return NULL;
    }

    s_full_object_result.start  = parser->tokens[idx].start;
    s_full_object_result.count  = okj_count_object_members(parser->tokens[idx].start);
    s_full_object_result.length = okj_measure_container(parser->tokens[idx].start);

    return &s_full_object_result;
}

uint16_t okj_copy_string(const OkJsonString *str, char *buf, uint16_t buf_size)
{
    uint16_t copy_len = 0U;
    uint16_t i        = 0U;

    if ((str == NULL) || (buf == NULL) || (buf_size == 0U))
    {
        return 0U;
    }

    /* Copy at most (buf_size - 1) bytes to leave room for the null terminator. */
    copy_len = str->length;

    if (copy_len >= buf_size)
    {
        copy_len = (uint16_t)(buf_size - 1U);
    }

    for (i = 0U; i < copy_len; i++)
    {
        buf[i] = str->start[i];
    }

    buf[copy_len] = '\0';

    return copy_len;
}

uint16_t okj_count_objects(OkJsonParser *parser)
{
    uint16_t count = 0U;
    uint16_t i;

    if (parser == NULL)
    {
        return 0U;
    }

    for (i = 0U; i < parser->token_count; i++)
    {
        if (parser->tokens[i].type == OKJ_OBJECT)
        {
            count++;
        }
    }

    return count;
}

uint16_t okj_count_arrays(OkJsonParser *parser)
{
    uint16_t count = 0U;
    uint16_t i;

    if (parser == NULL)
    {
        return 0U;
    }

    for (i = 0U; i < parser->token_count; i++)
    {
        if (parser->tokens[i].type == OKJ_ARRAY)
        {
            count++;
        }
    }

    return count;
}

uint16_t okj_count_elements(OkJsonParser *parser)
{
    if (parser == NULL)
    {
        return 0U;
    }

    return parser->token_count;
}

/* ---------------------------------------------------------------------------
 * Debug print — only compiled when OK_JSON_DEBUG is defined
 * ---------------------------------------------------------------------------*/

#ifdef OK_JSON_DEBUG

#include <stdio.h>

static const char *okj_type_name(OkJsonType t)
{
    const char *name;

    switch (t)
    {
        case OKJ_UNDEFINED: name = "UNDEFINED"; break;
        case OKJ_OBJECT:    name = "OBJECT";    break;
        case OKJ_ARRAY:     name = "ARRAY";     break;
        case OKJ_STRING:    name = "STRING";    break;
        case OKJ_NUMBER:    name = "NUMBER";    break;
        case OKJ_BOOLEAN:   name = "BOOLEAN";   break;
        case OKJ_NULL:      name = "NULL";      break;
        default:            name = "UNKNOWN";   break;
    }

    return name;
}

void okj_debug_print(OkJsonParser *parser)
{
    uint16_t i;

    if (parser == NULL)
    {
        (void)printf("okj_debug_print: NULL parser\n");
        return;
    }

    (void)printf("=== OK_JSON Debug Dump: %u token(s) ===\n",
                 (unsigned int)parser->token_count);

    for (i = 0U; i < parser->token_count; i++)
    {
        OkJsonToken *t    = &parser->tokens[i];
        uint16_t     dlen = t->length;
        uint16_t     j;

        if ((t->type == OKJ_OBJECT) || (t->type == OKJ_ARRAY))
        {
            dlen = okj_measure_container(t->start);
        }

        (void)printf("[%3u] type=%-9s len=%3u  val='",
                     (unsigned int)i,
                     okj_type_name(t->type),
                     (unsigned int)dlen);

        if (t->start != NULL)
        {
            for (j = 0U; j < dlen; j++)
            {
                if (t->start[j] == '\0')
                {
                    break;
                }

                (void)putchar((int)(unsigned char)t->start[j]);
            }
        }

        (void)printf("'\n");
    }
}

#endif /* OK_JSON_DEBUG */
