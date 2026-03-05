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

static int okj_is_whitespace(char c)
{
    return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

static int okj_is_digit(char c)
{
    return (c >= '0') && (c <= '9');
}

/* Returns 1 if the first `len` bytes of `src` equal `lit`, 0 otherwise.
 * Stops early on a NUL byte in `src` to avoid overreads. */
static int okj_match(const char *src, const char *lit, uint16_t len)
{
    uint16_t i = 0U;

    for (i = 0U; i < len; i++)
    {
        if ((src[i] == '\0') || (src[i] != lit[i]))
        {
            return 0U;
        }
    }

    return 1U;
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

/* ---------------------------------------------------------------------------
 * File-scope result structs returned by getter functions.
 * Avoids dynamic allocation (suitable for embedded targets).
 * ---------------------------------------------------------------------------*/

static OkJsonString  s_string_result;
static OkJsonNumber  s_number_result;
static OkJsonBoolean s_boolean_result;
static OkJsonArray   s_array_result;
static OkJsonObject  s_object_result;

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
                parser->position++;     /* skip backslash */

                if (parser->json[parser->position] == '\0')
                {
                    break;  /* truncated input: backslash at end of stream */
                }
            }

            parser->position++;
        }

        if ((parser->json[parser->position] != '"') &&
            (parser->json[parser->position] != '\0'))
        {
            /* Loop exited due to the length limit, not a closing quote. */
            result = OKJ_ERROR_MAX_STR_LEN_EXCEEDED;
        }
        else if (parser->json[parser->position] != '"')
        {
            result = OKJ_ERROR_SYNTAX;
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
        tok        = &parser->tokens[parser->token_count];
        tok->type  = OKJ_NUMBER;
        tok->start = &parser->json[parser->position];

        start_pos  = parser->position;

        parser->position++;

        while ((okj_is_digit(parser->json[parser->position])) ||
               (parser->json[parser->position] == '.'))
        {
            parser->position++;
        }

        if (parser->json[parser->position - 1U] == '.')
        {
            result = OKJ_ERROR_BAD_NUMBER;
        }
        else
        {
            tok->length = parser->position - start_pos;

            parser->token_count++;
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
    OkjError result = OKJ_SUCCESS;

    if (parser == NULL)
    {
        return OKJ_ERROR_BAD_POINTER;
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

    while (key[key_len] != '\0')
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

    s_array_result.start = parser->tokens[idx].start;
    s_array_result.count = okj_count_array_elements(parser->tokens[idx].start);

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

    s_object_result.start = parser->tokens[idx].start;
    s_object_result.count = okj_count_object_members(parser->tokens[idx].start);

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
