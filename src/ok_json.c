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

/*@
  // 1. Frame Condition
  // This function modifies absolutely no external memory.
  assigns \nothing;

  // 2. Behaviors
  // We define exactly what constitutes a "success" (is whitespace) 
  // and "failure" (is not whitespace).
  behavior is_space:
    assumes c == ' ' || c == '\t' || c == '\n' || c == '\r';
    ensures \result == 1;

  behavior not_space:
    assumes !(c == ' ' || c == '\t' || c == '\n' || c == '\r');
    ensures \result == 0;

  // 3. Completeness Guarantees
  // This forces the WP prover to verify mathematically that our two 
  // behaviors cover 100% of all possible inputs (complete) and that 
  // they never overlap (disjoint).
  complete behaviors;
  disjoint behaviors;
*/
static uint8_t okj_is_whitespace(char c)
{
    return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

/*@
  // 1. Frame Condition
  // This function modifies absolutely no external memory.
  assigns \nothing;

  // 2. Behaviors
  // We define exactly what constitutes a "success" (is digit) 
  // and "failure" (is not digit).
  behavior is_digit:
    assumes c >= '0' && c <= '9';
    ensures \result == 1;

  behavior not_digit:
    assumes !(c >= '0' && c <= '9');
    ensures \result == 0;

  // 3. Completeness Guarantees
  // This forces the WP prover to verify mathematically that our two 
  // behaviors cover 100% of all possible inputs (complete) and that 
  // they never overlap (disjoint).
  complete behaviors;
  disjoint behaviors;
*/
static uint8_t okj_is_digit(char c)
{
    return (c >= '0') && (c <= '9');
}

/*@
  // 1. Frame Condition
  // This function modifies absolutely no external memory.
  assigns \nothing;

  // 2. Behaviors
  // We define exactly what constitutes a "success" (is hex digit) 
  // and "failure" (is not hex digit).
  behavior is_hex:
    assumes (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    ensures \result == 1;

  behavior not_hex:
    assumes !((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
    ensures \result == 0;

  // 3. Completeness Guarantees
  // This forces the WP prover to verify mathematically that our two 
  // behaviors cover 100% of all possible inputs (complete) and that 
  // they never overlap (disjoint).
  complete behaviors;
  disjoint behaviors;
*/
static uint8_t okj_is_hex_digit(char c)
{
    return ((c >= '0') && (c <= '9')) ||
           ((c >= 'a') && (c <= 'f')) ||
           ((c >= 'A') && (c <= 'F'));
}

/*@
  // 1. Frame Condition
  // The function modifies absolutely no external memory.
  assigns \nothing;

  // 2. Behaviors
  // A valid UTF-8 continuation byte always has the top two bits set to 10.
  // In hex, that means masking with 0xC0 (11000000) must equal 0x80 (10000000).
  behavior is_continuation:
    assumes (byte & 0xC0U) == 0x80U;
    ensures \result == 1;

  behavior not_continuation:
    assumes (byte & 0xC0U) != 0x80U;
    ensures \result == 0;

  // 3. Completeness Guarantees
  complete behaviors;
  disjoint behaviors;
*/
static uint8_t okj_is_utf8_continuation(uint8_t byte)
{
    uint8_t result = 0U;
    
    if ((byte & 0xC0U) == 0x80U)
    {
        result = 1U;
    }

    return result;
}

/*@
  // 1. Preconditions
  requires src != \null;
  requires advance != \null;
  
  // The caller MUST guarantee that the advance pointer is writable.
  requires \valid(advance);
  
  // The caller MUST guarantee that it is safe to read up to 4 bytes 
  // starting at the current position. If they pass a string that ends 
  // 2 bytes from 'pos', Frama-C will fail the build at the call site!
  // We also ensure pos doesn't mathematically overflow when adding 3.
  requires pos <= 65531; 
  requires \valid_read(src + (pos .. pos + 3));

  // 2. Frame Condition
  // We only modify the value pointed to by 'advance'
  assigns *advance;

  // 3. Behaviors
  behavior success:
    assumes src != \null && advance != \null;
    // The function must return a boolean 0 or 1
    ensures \result == 0 || \result == 1;
    // If it succeeds, the advance pointer must be set between 1 and 4
    ensures \result == 1 ==> (*advance >= 1 && *advance <= 4);

  complete behaviors;
  disjoint behaviors;
*/
static uint8_t okj_validate_utf8_sequence(const char *src, uint16_t pos, uint16_t *advance)
{
    /* Validate one UTF-8 scalar-value sequence starting at src[pos].
     * On success sets *advance (1..4) and returns 1, else returns 0.
     * TODO: Refactoring this to comply with single return rule has
     * really made the cognitive complexity of this function worse.
     * Need to refactor this again to reduce it. -RLM
     */
    uint8_t result = 0U;

    if ((src != NULL) && (advance != NULL))
    {
        uint8_t b0 = (uint8_t)src[pos];

        if (b0 <= 0x7FU)
        {
            *advance = 1U;
            result = 1U;
        }
        else
        {
            uint8_t b1 = (uint8_t)src[pos + 1U];

            if ((b0 >= 0xC2U) && (b0 <= 0xDFU))
            {
                if (okj_is_utf8_continuation(b1) != 0U)
                {
                    *advance = 2U;
                    result = 1U;
                }
                else
                {
                    result = 0U;
                }
            }
            else
            {
                uint8_t b2 = (uint8_t)src[pos + 2U];

                if (b0 == 0xE0U)
                {
                    if ((b1 >= 0xA0U) && (b1 <= 0xBFU) &&
                        (okj_is_utf8_continuation(b2) != 0U))
                    {
                        *advance = 3U;
                        result = 1U;
                    }
                    else
                    {
                        result = 0U;
                    }
                }
                else
                {
                    if (((b0 >= 0xE1U) && (b0 <= 0xECU)) ||
                        ((b0 >= 0xEEU) && (b0 <= 0xEFU)))
                    {
                        if ((okj_is_utf8_continuation(b1) != 0U) &&
                            (okj_is_utf8_continuation(b2) != 0U))
                        {
                            *advance = 3U;
                            result = 1U;
                        }
                        else
                        {
                            result = 0U;
                        }
                    }
                    else
                    {
                        if (b0 == 0xEDU)
                        {
                            if ((b1 >= 0x80U) && (b1 <= 0x9FU) &&
                                (okj_is_utf8_continuation(b2) != 0U))
                            {
                                *advance = 3U;
                                result = 1U;
                            }
                            else
                            {
                                result = 0U;
                            }
                        }
                        else
                        {
                            uint8_t b3 = (uint8_t)src[pos + 3U];

                            if (b0 == 0xF0U)
                            {
                                if ((b1 >= 0x90U) && (b1 <= 0xBFU) &&
                                    (okj_is_utf8_continuation(b2) != 0U) &&
                                    (okj_is_utf8_continuation(b3) != 0U))
                                {
                                    *advance = 4U;
                                    result = 1U;
                                }
                                else
                                {
                                    result = 0U;
                                }
                            }
                            else
                            {
                                if ((b0 >= 0xF1U) && (b0 <= 0xF3U))
                                {
                                    if ((okj_is_utf8_continuation(b1) != 0U) &&
                                        (okj_is_utf8_continuation(b2) != 0U) &&
                                        (okj_is_utf8_continuation(b3) != 0U))
                                    {
                                        *advance = 4U;
                                        result = 1U;
                                    }
                                    else
                                    {
                                        result = 0U;
                                    }
                                }
                                else
                                {
                                    if (b0 == 0xF4U)
                                    {
                                        if ((b1 >= 0x80U) && (b1 <= 0x8FU) &&
                                            (okj_is_utf8_continuation(b2) != 0U) &&
                                            (okj_is_utf8_continuation(b3) != 0U))
                                        {
                                            *advance = 4U;
                                            result = 1U;
                                        }
                                        else
                                        {
                                            result = 0U;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

/*@
  // 1. Preconditions
  // If the pointers are not NULL, they MUST be readable up to 'len' bytes.
  // This precondition acts as a contract for whoever calls okj_match.
  requires src != \null ==> \valid_read(src + (0 .. len - 1));
  requires lit != \null ==> \valid_read(lit + (0 .. len - 1));

  // 2. Frame Condition
  assigns \nothing;

  // 3. Behaviors
  behavior invalid_ptrs:
    assumes src == \null || lit == \null;
    ensures \result == 0;

  behavior valid_ptrs:
    assumes src != \null && lit != \null;
    ensures \result == 0 || \result == 1; // It must return a boolean equivalent

  // 3. Completeness Guarantees
  complete behaviors;
  disjoint behaviors;
*/
static uint8_t okj_match(const char *src, const char *lit, uint16_t len)
{
  /* Returns 1 if the first `len` bytes of `src` equal `lit`, 0 otherwise.
   * Stops early on a NUL byte in `src` to avoid overreads. */
    uint8_t result = 1U;

    if ((src == NULL) || (lit == NULL))
    {
        result = 0U;
    }
    else
    {
        uint16_t i;

        /*@
          // LOOP INVARIANTS
          // Prove 'i' never exceeds 'len' to guarantee no out-of-bounds access.
          loop invariant 0 <= i <= len;
          
          // The result state can only ever be 0 or 1.
          loop invariant result == 0 || result == 1;
          
          // Explicitly state what variables the loop modifies.
          loop assigns i, result;
          
          // Prove termination.
          loop variant len - i;
        */
        for (i = 0U; (i < len) && (result != 0U); i++)
        {
            if ((src[i] == '\0') || (src[i] != lit[i]))
            {
                result = 0U;
            }
        }
    }

    return result;
}

/*@
  // 1. Frame Condition
  assigns \nothing;

  // 2. Behaviors
  // We explicitly list the 4 structural terminators AND the 4 whitespace 
  // characters for the theorem prover's pure logic evaluation.
  behavior is_terminator:
    assumes c == '\0' || c == ',' || c == ']' || c == '}' || 
            c == ' '  || c == '\t' || c == '\n' || c == '\r';
    ensures \result == 1;

  behavior not_terminator:
    assumes !(c == '\0' || c == ',' || c == ']' || c == '}' || 
              c == ' '  || c == '\t' || c == '\n' || c == '\r');
    ensures \result == 0;

  // 3. Completeness
  complete behaviors;
  disjoint behaviors;
*/
static uint8_t okj_is_value_terminator(char c)
{
    /* Returns 1 if `c` is a valid character that may immediately follow a JSON
     * value token (keyword literal, number, or closing bracket).  Valid
     * terminators are: end of input, whitespace, comma, or a closing bracket. */
    
    return (c == '\0') || (c == ',') || (c == ']') || (c == '}') ||
           okj_is_whitespace(c);
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
    const char *scan = p;

    scan++;   /* skip opening '"' */

    while ((*scan != '"') && (*scan != '\0'))
    {
        if (*scan == '\\')
        {
            scan++;    /* skip the backslash */

            if (*scan == '\0')
            {
                break;  /* truncated input: backslash at end of stream */
            }
        }

        scan++;
    }

    if (*scan == '"')
    {
        scan++;    /* skip closing '"' */
    }

    return scan;
}

/* Count the number of elements in a JSON array whose text begins at `start`
 * (which must point to the '[' character).  Handles nested arrays and objects
 * correctly by tracking bracket depth, and skips string content to avoid
 * counting structural characters that appear inside quoted values. */
static uint16_t okj_count_array_elements(const char *start)
{
    const char *p = start;

    uint16_t count = 0U;

    if ((p != NULL) && (*p == '['))
    {
        p++;        /* skip '[' */

        uint16_t depth = 1U;

        uint8_t  seen  = 0U;    /* set when first non-whitespace element is found */

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
    }

    return count;
}

/* Count the number of key-value members in a JSON object whose text begins
 * at `start` (which must point to the '{' character).  Each colon at depth 1
 * corresponds to exactly one member. */
static uint16_t okj_count_object_members(const char *start)
{
    const char *p = start;

    uint16_t count = 0U;

    if ((p != NULL) && (*p == '{'))
    {
        p++;        /* skip '{' */

        uint16_t depth = 1U;

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
    const char *p = start;

    uint16_t length = 0U;

    if ((p != NULL) && ((*p == '[') || (*p == '{')))
    {
        uint16_t depth  = 0U;

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
    }

    return length;
}


/* ---------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------------*/

/*@
  // 1. Preconditions
  // The parser can be NULL, or it must be a valid pointer to the struct.
  requires parser == \null || \valid(parser);

  // 2. Behaviors
  behavior null_ptrs:
    assumes parser == \null || json_string == \null;
    assigns \nothing;

  behavior valid_ptrs:
    assumes parser != \null && json_string != \null;
    
    // We are modifying the entire parser struct
    assigns *parser;
    
    // Verify standard field initializations
    ensures parser->json == json_string;
    ensures parser->json_len == json_len;
    ensures parser->position == 0;
    ensures parser->token_count == 0;
    ensures parser->depth == 0;
    ensures parser->context == OKJ_CTX_WANT_VALUE;
    
    // Verify array initializations using universal quantification
    ensures \forall integer k; 0 <= k < OKJ_MAX_TOKENS ==> parser->tokens[k].type == OKJ_UNDEFINED;
    ensures \forall integer k; 0 <= k < OKJ_MAX_TOKENS ==> parser->tokens[k].start == \null;
    ensures \forall integer k; 0 <= k < OKJ_MAX_TOKENS ==> parser->tokens[k].length == 0;
    ensures \forall integer k; 0 <= k < OKJ_MAX_DEPTH ==> parser->depth_stack[k] == OKJ_UNDEFINED;

  complete behaviors;
  disjoint behaviors;
*/
void okj_init(OkJsonParser *parser, char *json_string, uint16_t json_len)
{
    if ((parser != NULL) && (json_string != NULL))
    {
        uint16_t i;

        /*@
          loop invariant 0 <= i <= OKJ_MAX_TOKENS;
          
          // Prove that all elements BEFORE the current index 'i' are correctly initialized
          loop invariant \forall integer k; 0 <= k < i ==> parser->tokens[k].type == OKJ_UNDEFINED;
          loop invariant \forall integer k; 0 <= k < i ==> parser->tokens[k].start == \null;
          loop invariant \forall integer k; 0 <= k < i ==> parser->tokens[k].length == 0;
          
          // Explicitly state which part of the array this loop is allowed to modify
          loop assigns i, parser->tokens[0 .. OKJ_MAX_TOKENS - 1];
          loop variant OKJ_MAX_TOKENS - i;
        */
        for (i = 0U; i < OKJ_MAX_TOKENS; i++)
        {
            parser->tokens[i].type   = OKJ_UNDEFINED;
            parser->tokens[i].start  = NULL;
            parser->tokens[i].length = 0U;
        }

        /*@
          loop invariant 0 <= i <= OKJ_MAX_DEPTH;
          loop invariant \forall integer k; 0 <= k < i ==> parser->depth_stack[k] == OKJ_UNDEFINED;
          loop assigns i, parser->depth_stack[0 .. OKJ_MAX_DEPTH - 1];
          loop variant OKJ_MAX_DEPTH - i;
        */
        for (i = 0U; i < (uint16_t)OKJ_MAX_DEPTH; i++)
        {
            parser->depth_stack[i] = OKJ_UNDEFINED;
        }

        parser->json        = json_string;
        parser->json_len    = json_len;
        parser->position    = 0U;
        parser->token_count = 0U;
        parser->depth       = 0U;
        parser->context     = OKJ_CTX_WANT_VALUE;
    }
}

/*@
  // 1. Preconditions
  // The parser pointer can be NULL, or it must point to a valid struct.
  requires parser == \null || \valid(parser);
  
  // IF the parser is not NULL, the JSON payload it points to must be 
  // safely readable from index 0 up to json_len - 1.
  requires parser != \null ==> \valid_read(parser->json + (0 .. parser->json_len - 1));
  
  // IF the parser is not NULL, the current position must not already 
  // exceed the payload length.
  requires parser != \null ==> parser->position <= parser->json_len;

  // 2. Behaviors (These handle the 'assigns' automatically)
  behavior is_null:
    assumes parser == \null;
    assigns \nothing;

  behavior valid_parser:
    assumes parser != \null;
    assigns parser->position;
    ensures parser->position >= \old(parser->position); // Position never goes backwards
    ensures parser->position <= parser->json_len;       // Position never exceeds buffer

  complete behaviors;
  disjoint behaviors;
*/
static void okj_skip_whitespace(OkJsonParser *parser)
{
    if (parser != NULL)
    {
        /*@
          // LOOP INVARIANTS: These are mathematically proven before the loop 
          // starts, at the end of every iteration, and after the loop exits.
          // Split into individual goals for the prover.

          loop invariant \at(parser->position, Pre) <= parser->position;
          loop invariant parser->position <= parser->json_len;
          
          loop assigns parser->position;
          loop variant parser->json_len - parser->position;
        */
        while ((parser->position < parser->json_len) &&
               (okj_is_whitespace(parser->json[parser->position]) == 1U))
        {
            parser->position++;
        }
    }
}

/*
 * TODO: Refactor to reduce complexity. -RLM
 */
static OkjError okj_parse_value(OkJsonParser *parser)
{
    OkjError    result  = OKJ_SUCCESS;
    OkJsonToken *tok    = NULL;

    if (parser == NULL)
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        okj_skip_whitespace(parser);

        if (parser->position >= parser->json_len)
        {
            /* End of input — nothing to do. */
        }
        else
        {

        char c = parser->json[parser->position];

        if (c == '{')
        {
            /* Object open is only valid in a value position. */
            if ((parser->context != OKJ_CTX_WANT_VALUE) &&
                (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                if (parser->depth >= (uint16_t)OKJ_MAX_DEPTH)
                {
                    result = OKJ_ERROR_MAX_DEPTH_EXCEEDED;
                }
                else
                {
                    tok         = &parser->tokens[parser->token_count];
                    tok->type   = OKJ_OBJECT;
                    tok->start  = &parser->json[parser->position];
                    tok->length = 1U;

                    parser->depth_stack[parser->depth] = OKJ_OBJECT;
                    parser->depth++;
                    parser->position++;
                    parser->token_count++;

                    /* After '{' we expect a key string or immediate '}' (empty object). */
                    parser->context = OKJ_CTX_WANT_KEY_OR_CLOSE;
                }
            }
        }
        else if (c == '[')
        {
            /* Array open is only valid in a value position. */
            if ((parser->context != OKJ_CTX_WANT_VALUE) &&
                (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                if (parser->depth >= (uint16_t)OKJ_MAX_DEPTH)
                {
                    result = OKJ_ERROR_MAX_DEPTH_EXCEEDED;
                }
                else
                {
                    tok         = &parser->tokens[parser->token_count];
                    tok->type   = OKJ_ARRAY;
                    tok->start  = &parser->json[parser->position];
                    tok->length = 1U;

                    parser->depth_stack[parser->depth] = OKJ_ARRAY;
                    parser->depth++;
                    parser->position++;
                    parser->token_count++;

                    /* After '[' we expect a value or immediate ']' (empty array). */
                    parser->context = OKJ_CTX_WANT_VALUE_OR_CLOSE;
                }
            }
        }
        else if (c == '}')
        {
            /* '}' is valid:
            *  - after a value in a non-empty object (WANT_SEP_OR_CLOSE), or
            *  - immediately after '{' to close an empty object (WANT_KEY_OR_CLOSE).
            * It is NOT valid after a trailing comma (WANT_KEY). */
            if ((parser->context != OKJ_CTX_WANT_SEP_OR_CLOSE) &&
                (parser->context != OKJ_CTX_WANT_KEY_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                /* Validate the closing bracket against the depth stack. */
                if (parser->depth == 0U)
                {
                    result = OKJ_ERROR_BRACKET_MISMATCH;
                }
                else
                {
                    parser->depth--;

                    if (parser->depth_stack[parser->depth] != OKJ_OBJECT)
                    {
                        result = OKJ_ERROR_BRACKET_MISMATCH;
                    }
                    else
                    {
                        parser->position++;

                        /* Update context for the enclosing container (if any). */
                        if (parser->depth > 0U)
                        {
                            parser->context = OKJ_CTX_WANT_SEP_OR_CLOSE;
                        }
                    }
                }
            }
        }
        else if (c == ']')
        {
            /* ']' is valid:
            *  - after a value in a non-empty array (WANT_SEP_OR_CLOSE), or
            *  - immediately after '[' to close an empty array (WANT_VALUE_OR_CLOSE).
            * It is NOT valid after a trailing comma (WANT_VALUE). */
            if ((parser->context != OKJ_CTX_WANT_SEP_OR_CLOSE) &&
                (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                if (parser->depth == 0U)
                {
                    result = OKJ_ERROR_BRACKET_MISMATCH;
                }
                else
                {
                    parser->depth--;

                    if (parser->depth_stack[parser->depth] != OKJ_ARRAY)
                    {
                        result = OKJ_ERROR_BRACKET_MISMATCH;
                    }
                    else
                    {
                        parser->position++;

                        if (parser->depth > 0U)
                        {
                            parser->context = OKJ_CTX_WANT_SEP_OR_CLOSE;
                        }
                    }
                }
            }
        }
        else if (c == ',')
        {
            /* Comma is only valid after a completed value.  Trailing commas
            * (comma followed immediately by a closing bracket) are forbidden
            * by RFC 8259. */
            if (parser->context != OKJ_CTX_WANT_SEP_OR_CLOSE)
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                if (parser->depth == 0U)
                {
                    result = OKJ_ERROR_SYNTAX;    /* comma outside any container */
                }
                else
                {
                    parser->position++;

                    /* What comes next depends on whether we are inside an object or array.
                    * Note: WANT_KEY (not WANT_KEY_OR_CLOSE) prevents trailing commas. */
                    if (parser->depth_stack[parser->depth - 1U] == OKJ_OBJECT)
                    {
                        parser->context = OKJ_CTX_WANT_KEY;
                    }
                    else
                    {
                        parser->context = OKJ_CTX_WANT_VALUE;
                    }
                }
            }
        }
        else if (c == ':')
        {
            /* Colon is only valid immediately after an object key string. */
            if (parser->context != OKJ_CTX_WANT_COLON)
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                parser->position++;
                parser->context = OKJ_CTX_WANT_VALUE;
            }
        }
        else if (c == '"')
        {
            uint8_t is_key = 0U;    /* 1 when this string is an object key */

            /* A string is valid as an object key or as a value. */
            if ((parser->context == OKJ_CTX_WANT_KEY) ||
                (parser->context == OKJ_CTX_WANT_KEY_OR_CLOSE))
            {
                is_key = 1U;
            }
            else if ((parser->context != OKJ_CTX_WANT_VALUE) &&
                    (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                /* is_key stays 0 — string is a value */
            }

            if (result == OKJ_SUCCESS)
            {
                tok        = &parser->tokens[parser->token_count];
                tok->type  = OKJ_STRING;
                tok->start = &parser->json[parser->position + 1U];  /* skip opening '"' */

                uint16_t start_pos  = parser->position + 1U;

                parser->position++;

                uint8_t loop_break = 0U;

                while ((parser->position < parser->json_len)    &&
                       (parser->json[parser->position] != '"')  &&
                       (loop_break != 1U))
                {
                    if ((parser->position - start_pos) >= (uint16_t)OKJ_MAX_STRING_LEN)
                    {
                        loop_break = 1U;
                    }
                    else
                    {
                        if (parser->json[parser->position] == '\\')
                        {
                            parser->position++;     /* consume backslash */

                            if (parser->position >= parser->json_len)
                            {
                                loop_break = 1U;  /* truncated input: backslash at end of stream */
                            }
                            else
                            {
                                char esc_char = parser->json[parser->position];

                                if ((esc_char == '"')  || (esc_char == '\\') ||
                                    (esc_char == '/')  || (esc_char == 'b')  ||
                                    (esc_char == 'f')  || (esc_char == 'n')  ||
                                    (esc_char == 'r')  || (esc_char == 't'))
                                {
                                    parser->position++;     /* consume the escape character */
                                }
                                else if (esc_char == 'u')
                                {
                                    parser->position++;     /* consume 'u' */

                                    uint16_t h;

                                    for (h = 0U; h < 4U; h++)
                                    {
                                        if ((okj_is_hex_digit(parser->json[parser->position]) == 0U) &&
                                            (loop_break != 1U))
                                        {
                                            result = OKJ_ERROR_BAD_STRING;
                                            loop_break = 1U;
                                        }
                                        else
                                        {
                                            parser->position++;
                                        }
                                    }
                                }
                                else
                                {
                                    result = OKJ_ERROR_BAD_STRING;
                                }
                            }

                            if (result != OKJ_SUCCESS)
                            {
                                loop_break = 1U;
                            }
                        }
                        else
                        {
                            uint16_t utf8_advance = 0U;

                            /* RFC 8259 §7: bare control characters (U+0000–U+001F) are
                            * forbidden inside strings; they must be represented as
                            * escape sequences (e.g. \n, \t, \uXXXX). */
                            if ((unsigned char)parser->json[parser->position] < 0x20U)
                            {
                                result = OKJ_ERROR_BAD_STRING;
                                loop_break = 1U;
                            }
                            else
                            {
                                if (okj_validate_utf8_sequence(parser->json,
                                                            parser->position,
                                                            &utf8_advance) == 0U)
                                {
                                    result = OKJ_ERROR_BAD_STRING;
                                    loop_break = 1U;
                                }
                                else
                                {
                                    parser->position += utf8_advance;
                                }
                            }
                        }
                    }
                }

                if (result != OKJ_SUCCESS)
                {
                    /* Error set inside loop (e.g. invalid escape sequence). */
                }
                else if (parser->position >= parser->json_len)
                {
                    /* Reached end of input before the closing quote. */
                    result = OKJ_ERROR_UNEXPECTED_END;
                }
                else if (parser->json[parser->position] != '"')
                {
                    /* Loop exited due to the length limit, not a closing quote. */
                    result = OKJ_ERROR_MAX_STR_LEN_EXCEEDED;
                }
                else if ((parser->position - start_pos) > (uint16_t)OKJ_MAX_STRING_LEN)
                {
                    /* An escape sequence whose first byte landed just below the
                    * OKJ_MAX_STRING_LEN ceiling advanced position one past the
                    * limit in a single loop iteration before the top-of-loop
                    * length check could fire.  The raw string content therefore
                    * exceeds the declared maximum; reject it. */
                    result = OKJ_ERROR_MAX_STR_LEN_EXCEEDED;
                }
                else
                {
                    tok->length = parser->position - start_pos;

                    parser->position++;   /* advance past closing '"' */
                    parser->token_count++;

                    /* Update grammar context based on whether this string was a key
                    * (next: colon) or a value (next: separator or closing bracket). */
                    if (is_key != 0U)
                    {
                        parser->context = OKJ_CTX_WANT_COLON;
                    }
                    else if (parser->depth > 0U)
                    {
                        parser->context = OKJ_CTX_WANT_SEP_OR_CLOSE;
                    }
                    else
                    {
                        /* Top-level string value: the main loop handles the exit. */
                    }
                }
            }
        }
        else if ((okj_is_digit(c)) || (c == '-'))
        {
            /* Numbers are only valid in value positions, never as object keys. */
            if ((parser->context != OKJ_CTX_WANT_VALUE) &&
                (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                tok        = &parser->tokens[parser->token_count];
                tok->type  = OKJ_NUMBER;
                tok->start = &parser->json[parser->position];

                uint16_t start_pos  = parser->position;

                uint8_t number_ok = 1U;

                /* Step 1: optional leading minus */
                if (c == '-')
                {
                    parser->position++;     /* consume '-' */

                    if ((parser->position >= parser->json_len) ||
                        (okj_is_digit(parser->json[parser->position]) == 0U))
                    {
                        number_ok = 0U;     /* bare minus is not a valid number */
                    }
                }

                /* Step 2: integer part — zero OR digit1-9 *DIGIT */
                if (number_ok != 0U)
                {
                    if ((parser->position < parser->json_len) &&
                        (parser->json[parser->position] == '0'))
                    {
                        parser->position++;     /* consume '0' */

                        if ((parser->position < parser->json_len) &&
                            (okj_is_digit(parser->json[parser->position]) != 0U))
                        {
                            number_ok = 0U;     /* leading zero: "012" is invalid */
                        }
                    }
                    else
                    {
                        while ((parser->position < parser->json_len) &&
                               (okj_is_digit(parser->json[parser->position]) != 0U))
                        {
                            parser->position++;
                        }
                    }
                }

                /* Step 3: optional fractional part — '.' 1*DIGIT */
                if ((number_ok != 0U) &&
                    (parser->position < parser->json_len) &&
                    (parser->json[parser->position] == '.'))
                {
                    parser->position++;     /* consume '.' */

                    if ((parser->position >= parser->json_len) ||
                        (okj_is_digit(parser->json[parser->position]) == 0U))
                    {
                        number_ok = 0U;     /* decimal point must be followed by a digit */
                    }
                    else
                    {
                        while ((parser->position < parser->json_len) &&
                               (okj_is_digit(parser->json[parser->position]) != 0U))
                        {
                            parser->position++;
                        }
                    }
                }

                /* Step 4: optional exponent part — ('e'/'E') [sign] 1*DIGIT */
                if ((number_ok != 0U) &&
                    (parser->position < parser->json_len) &&
                    ((parser->json[parser->position] == 'e') ||
                     (parser->json[parser->position] == 'E')))
                {
                    parser->position++;     /* consume 'e' or 'E' */

                    if ((parser->position < parser->json_len) &&
                        ((parser->json[parser->position] == '+') ||
                         (parser->json[parser->position] == '-')))
                    {
                        parser->position++;     /* consume optional sign */
                    }

                    if ((parser->position >= parser->json_len) ||
                        (okj_is_digit(parser->json[parser->position]) == 0U))
                    {
                        number_ok = 0U;     /* exponent requires at least one digit */
                    }
                    else
                    {
                        while ((parser->position < parser->json_len) &&
                               (okj_is_digit(parser->json[parser->position]) != 0U))
                        {
                            parser->position++;
                        }
                    }
                }

                if (number_ok != 0U)
                {
                    tok->length = parser->position - start_pos;

                    parser->token_count++;

                    if (parser->depth > 0U)
                    {
                        parser->context = OKJ_CTX_WANT_SEP_OR_CLOSE;
                    }
                }
                else
                {
                    result = OKJ_ERROR_BAD_NUMBER;
                }
            }
        }
        else if (okj_match(&parser->json[parser->position], "true", 4U) == 1U)
        {
            /* Keyword literals are only valid in value positions. */
            if ((parser->context != OKJ_CTX_WANT_VALUE) &&
                (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                tok         = &parser->tokens[parser->token_count];
                tok->type   = OKJ_BOOLEAN;
                tok->start  = &parser->json[parser->position];
                tok->length = 4U;

                parser->position += 4U;

                /* RFC 8259: keyword must end at a value boundary (no "truetrue").
                 * End of input is always a valid boundary. */
                if ((parser->position < parser->json_len) &&
                    (okj_is_value_terminator(parser->json[parser->position]) == 0U))
                {
                    result = OKJ_ERROR_SYNTAX;
                }
                else
                {
                    parser->token_count++;

                    if (parser->depth > 0U)
                    {
                        parser->context = OKJ_CTX_WANT_SEP_OR_CLOSE;
                    }
                }
            }
        }
        else if (okj_match(&parser->json[parser->position], "false", 5U) == 1U)
        {
            if ((parser->context != OKJ_CTX_WANT_VALUE) &&
                (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                tok         = &parser->tokens[parser->token_count];
                tok->type   = OKJ_BOOLEAN;
                tok->start  = &parser->json[parser->position];
                tok->length = 5U;

                parser->position += 5U;

                if ((parser->position < parser->json_len) &&
                    (okj_is_value_terminator(parser->json[parser->position]) == 0U))
                {
                    result = OKJ_ERROR_SYNTAX;
                }
                else
                {
                    parser->token_count++;

                    if (parser->depth > 0U)
                    {
                        parser->context = OKJ_CTX_WANT_SEP_OR_CLOSE;
                    }
                }
            }
        }
        else if (okj_match(&parser->json[parser->position], "null", 4U) == 1U)
        {
            if ((parser->context != OKJ_CTX_WANT_VALUE) &&
                (parser->context != OKJ_CTX_WANT_VALUE_OR_CLOSE))
            {
                result = OKJ_ERROR_SYNTAX;
            }
            else
            {
                tok         = &parser->tokens[parser->token_count];
                tok->type   = OKJ_NULL;
                tok->start  = &parser->json[parser->position];
                tok->length = 4U;

                parser->position += 4U;

                if ((parser->position < parser->json_len) &&
                    (okj_is_value_terminator(parser->json[parser->position]) == 0U))
                {
                    result = OKJ_ERROR_SYNTAX;
                }
                else
                {
                    parser->token_count++;

                    if (parser->depth > 0U)
                    {
                        parser->context = OKJ_CTX_WANT_SEP_OR_CLOSE;
                    }
                }
            }
        }
        else
        {
            result = OKJ_ERROR_SYNTAX;
        }

        } /* end else (position < json_len) */
    }

    return result;
}

OkjError okj_parse(OkJsonParser *parser)
{
    OkjError result = OKJ_SUCCESS;

    if (parser == NULL)
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        if (parser->json_len > OKJ_MAX_JSON_LEN)
        {
            result = OKJ_ERROR_MAX_JSON_LEN_EXCEEDED;
        }

        if (result != OKJ_ERROR_MAX_JSON_LEN_EXCEEDED)
        {
            uint8_t not_success = 0U;

            while ((parser->position < parser->json_len)  &&
                   (parser->token_count < OKJ_MAX_TOKENS) &&
                   (not_success == 0U))
            {
                uint16_t prev_tokens = parser->token_count;
                uint16_t prev_depth  = parser->depth;

                result = okj_parse_value(parser);

                if (result != OKJ_SUCCESS)
                {
                    not_success = 1U;
                }
                else
                {
                    /* Detect completion of the single top-level value:
                    *  - A primitive was emitted at depth 0 (depth stayed 0, new token added), or
                    *  - A container was fully closed back to depth 0 (depth dropped from > 0).
                    * RFC 8259 §2 permits exactly one top-level value; anything other than
                    * optional whitespace that follows it is a syntax error. */
                    if (parser->depth == 0U)
                    {
                        if ((prev_depth > 0U) || (parser->token_count > prev_tokens))
                        {
                            okj_skip_whitespace(parser);

                            if (parser->position < parser->json_len)
                            {
                                result = OKJ_ERROR_SYNTAX;
                            }

                            break;
                        }
                    }
                }
            }

            if ((result == OKJ_SUCCESS)                      &&
                (parser->token_count >= OKJ_MAX_TOKENS)      &&
                (parser->position < parser->json_len))
            {
                result = OKJ_ERROR_MAX_TOKENS_EXCEEDED;
            }

            /* Any containers still open at end-of-input indicate truncated input. */
            if ((result == OKJ_SUCCESS) && (parser->depth != 0U))
            {
                result = OKJ_ERROR_UNEXPECTED_END;
            }

            /* RFC 8259 §2: a JSON text must contain exactly one value.  An empty
            * or whitespace-only input has no value and is therefore invalid. */
            if ((result == OKJ_SUCCESS) && (parser->token_count == 0U))
            {
                result = OKJ_ERROR_UNEXPECTED_END;
            }
        }
    }

    return result;
}

/* ---------------------------------------------------------------------------
 * Internal lookup helper
 * ---------------------------------------------------------------------------*/

/*@
  // 1. Preconditions
  requires parser == \null || \valid_read(parser);
  
  // The search key must be safely readable up to key_len.
  requires key != \null ==> \valid_read(key + (0 .. key_len - 1));

  // The caller must guarantee that the parser state is well-formed.
  requires parser != \null ==> parser->token_count <= OKJ_MAX_TOKENS;

  // The caller must guarantee that EVERY token in the parser array that 
  // has a non-null start pointer is safely readable up to its specified length.
  // This is the mathematical key that allows okj_match to be proven safe.
  requires parser != \null ==> 
    (\forall integer k; 0 <= k < parser->token_count ==> 
      parser->tokens[k].start != \null ==> 
        \valid_read(parser->tokens[k].start + (0 .. parser->tokens[k].length - 1)));

  // 2. Frame Condition
  // This is a pure search function; it modifies absolutely nothing.
  assigns \nothing;

  // 3. Behaviors
  behavior invalid_args:
    assumes parser == \null || key == \null;
    ensures \result == OKJ_MAX_TOKENS;

  behavior valid_args:
    assumes parser != \null && key != \null;
    // The result is either OKJ_MAX_TOKENS (not found) or a valid index (1 to token_count)
    ensures \result == OKJ_MAX_TOKENS || (1 <= \result && \result <= parser->token_count);

  complete behaviors;
  disjoint behaviors;
*/
static uint16_t okj_find_value_index(OkJsonParser *parser, const char *key, uint16_t key_len)
{
    /* Scans the token array for a STRING token whose content equals `key`
    * (of length `key_len` bytes).  The key need not be null-terminated.
    * Returns the index of the NEXT token (the value), or OKJ_MAX_TOKENS if
    * not found.  Keys longer than OKJ_MAX_STRING_LEN are never found because
    * the parser enforces that limit on stored tokens. */

    uint16_t result = OKJ_MAX_TOKENS;

    if ((parser != NULL) && (key != NULL))
    {
        uint16_t i;

        /*@
          // LOOP INVARIANTS
          // i will never exceed the token count
          loop invariant 0 <= i <= parser->token_count;
          
          // Result stays at OKJ_MAX_TOKENS unless a match is found
          loop invariant result == OKJ_MAX_TOKENS || (1 <= result && result <= parser->token_count);
          
          loop assigns i, result;
          loop variant parser->token_count - i;
        */
        for (i = 0U; (i + 1U) < parser->token_count; i++)
        {
            const OkJsonToken *t = &parser->tokens[i];

            if ((t->type   == OKJ_STRING) &&
                (t->length == key_len)    &&
                (okj_match(t->start, key, key_len)))
            {
                result = i + 1U;
                break;
            }
        }
    }

    return result;
}

/* ---------------------------------------------------------------------------
 * Getter functions
 * ---------------------------------------------------------------------------*/

/*@
  // 1. Preconditions
  // Pointers can be null (we handle that gracefully), but if they aren't, 
  // they must be valid for reading/writing.
  requires parser == \null || \valid_read(parser);
  requires key != \null ==> \valid_read(key + (0 .. key_len - 1));
  requires out_str == \null || \valid(out_str);

  // Inherit the token validity requirements so okj_find_value_index is satisfied
  requires parser != \null ==> parser->token_count <= OKJ_MAX_TOKENS;
  requires parser != \null ==> 
    (\forall integer k; 0 <= k < parser->token_count ==> 
      parser->tokens[k].start != \null ==> 
        \valid_read(parser->tokens[k].start + (0 .. parser->tokens[k].length - 1)));

  // 2. Behaviors
  behavior invalid_args:
    assumes parser == \null || key == \null || out_str == \null;
    assigns \nothing;
    ensures \result == OKJ_ERROR_BAD_POINTER;

  behavior valid_args:
    assumes parser != \null && key != \null && out_str != \null;
    // We are only allowed to modify the output struct
    assigns *out_str;
    ensures \result == OKJ_SUCCESS || \result == OKJ_ERROR_BAD_STRING;

  complete behaviors;
  disjoint behaviors;
*/
OkjError okj_get_string(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonString *out_str)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_str == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_STRING))
        {
            result = OKJ_ERROR_BAD_STRING;
        }
        else
        {
            out_str->start  = parser->tokens[idx].start;
            out_str->length = parser->tokens[idx].length;
        }
    }

    return result;
}

/*@
  // 1. Preconditions
  // Pointers can be null (we handle that gracefully), but if they aren't, 
  // they must be valid for reading/writing.
  requires parser == \null || \valid_read(parser);
  requires key != \null ==> \valid_read(key + (0 .. key_len - 1));
  requires out_num == \null || \valid(out_num);

  // Inherit the token validity requirements so okj_find_value_index is satisfied
  requires parser != \null ==> parser->token_count <= OKJ_MAX_TOKENS;
  requires parser != \null ==> 
    (\forall integer k; 0 <= k < parser->token_count ==> 
      parser->tokens[k].start != \null ==> 
        \valid_read(parser->tokens[k].start + (0 .. parser->tokens[k].length - 1)));

  // 2. Behaviors
  behavior invalid_args:
    assumes parser == \null || key == \null || out_num == \null;
    assigns \nothing;
    ensures \result == OKJ_ERROR_BAD_POINTER;

  behavior valid_args:
    assumes parser != \null && key != \null && out_num != \null;
    // We are only allowed to modify the output struct
    assigns *out_num;
    ensures \result == OKJ_SUCCESS || \result == OKJ_ERROR_BAD_NUMBER;

  complete behaviors;
  disjoint behaviors;
*/
OkjError okj_get_number(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonNumber *out_num)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_num == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_NUMBER))
        {
            result = OKJ_ERROR_BAD_NUMBER;
        }
        else
        {
            out_num->start  = parser->tokens[idx].start;
            out_num->length = parser->tokens[idx].length;
        }
    }

    return result;
}

/*@
  // 1. Preconditions
  // Pointers can be null (we handle that gracefully), but if they aren't, 
  // they must be valid for reading/writing.
  requires parser == \null || \valid_read(parser);
  requires key != \null ==> \valid_read(key + (0 .. key_len - 1));
  requires out_bool == \null || \valid(out_bool);

  // Inherit the token validity requirements so okj_find_value_index is satisfied
  requires parser != \null ==> parser->token_count <= OKJ_MAX_TOKENS;
  requires parser != \null ==> 
    (\forall integer k; 0 <= k < parser->token_count ==> 
      parser->tokens[k].start != \null ==> 
        \valid_read(parser->tokens[k].start + (0 .. parser->tokens[k].length - 1)));

  // 2. Behaviors
  behavior invalid_args:
    assumes parser == \null || key == \null || out_bool == \null;
    assigns \nothing;
    ensures \result == OKJ_ERROR_BAD_POINTER;

  behavior valid_args:
    assumes parser != \null && key != \null && out_bool != \null;
    // We are only allowed to modify the output struct
    assigns *out_bool;
    ensures \result == OKJ_SUCCESS || \result == OKJ_ERROR_BAD_NUMBER;

  complete behaviors;
  disjoint behaviors;
*/
OkjError okj_get_boolean(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonBoolean *out_bool)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_bool == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_BOOLEAN))
        {
            result = OKJ_ERROR_BAD_BOOLEAN;
        }
        else
        {
            out_bool->start  = parser->tokens[idx].start;
            out_bool->length = parser->tokens[idx].length;
        }
    }

    return result;
}

OkjError okj_get_array(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonArray *out_arr)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_arr == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_ARRAY))
        {
            result = OKJ_ERROR_BAD_ARRAY;
        }
        else
        {
            out_arr->start  = parser->tokens[idx].start;
            out_arr->count  = okj_count_array_elements(parser->tokens[idx].start);
            out_arr->length = okj_measure_container(parser->tokens[idx].start);

            if (out_arr->count > OKJ_MAX_ARRAY_SIZE)
            {
                result = OKJ_ERROR_BAD_ARRAY;
            }
        }
    }

    return result;
}

OkjError okj_get_object(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonObject *out_obj)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_obj == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_OBJECT))
        {
            result = OKJ_ERROR_BAD_OBJECT;
        }
        else
        {
            out_obj->start  = parser->tokens[idx].start;
            out_obj->count  = okj_count_object_members(parser->tokens[idx].start);
            out_obj->length = okj_measure_container(parser->tokens[idx].start);

            if (out_obj->count > OKJ_MAX_OBJECT_SIZE)
            {
                result = OKJ_ERROR_BAD_OBJECT;
            }
        }
    }

    return result;
}

/*@
  // 1. Preconditions
  // Pointers can be null (we handle that gracefully), but if they aren't, 
  // they must be valid for reading/writing.
  requires parser == \null || \valid_read(parser);
  requires key != \null ==> \valid_read(key + (0 .. key_len - 1));
  requires out_tok == \null || \valid(out_tok);

  // Inherit the token validity requirements so okj_find_value_index is satisfied
  requires parser != \null ==> parser->token_count <= OKJ_MAX_TOKENS;
  requires parser != \null ==> 
    (\forall integer k; 0 <= k < parser->token_count ==> 
      parser->tokens[k].start != \null ==> 
        \valid_read(parser->tokens[k].start + (0 .. parser->tokens[k].length - 1)));

  // 2. Behaviors
  behavior invalid_args:
    assumes parser == \null || key == \null || out_tok == \null;
    assigns \nothing;
    ensures \result == OKJ_ERROR_BAD_POINTER;

  behavior valid_args:
    assumes parser != \null && key != \null && out_tok != \null;
    // We are only allowed to modify the output struct
    assigns *out_tok;
    ensures \result == OKJ_SUCCESS || \result == OKJ_ERROR_BAD_POINTER;

  complete behaviors;
  disjoint behaviors;
*/
OkjError okj_get_token(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonToken *out_tok)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_tok == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if (idx == OKJ_MAX_TOKENS)
        {
            result = OKJ_ERROR_BAD_POINTER;
        }
        else
        {
            out_tok->type   = parser->tokens[idx].type;
            out_tok->start  = parser->tokens[idx].start;
            out_tok->length = parser->tokens[idx].length;
        }
    }

    return result;
}

OkjError okj_get_array_raw(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonArray *out_arr)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_arr == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_ARRAY))
        {
            result = OKJ_ERROR_BAD_ARRAY;
        }
        else
        {
            out_arr->start  = parser->tokens[idx].start;
            out_arr->count  = okj_count_array_elements(parser->tokens[idx].start);
            out_arr->length = okj_measure_container(parser->tokens[idx].start);
        }
    }

    return result;
}

OkjError okj_get_object_raw(OkJsonParser *parser, const char *key, uint16_t key_len, OkJsonObject *out_obj)
{
    OkjError result = OKJ_SUCCESS;

    if ((parser == NULL) || (key == NULL) || (out_obj == NULL))
    {
        result = OKJ_ERROR_BAD_POINTER;
    }
    else
    {
        uint16_t idx = okj_find_value_index(parser, key, key_len);

        if ((idx == OKJ_MAX_TOKENS) || (parser->tokens[idx].type != OKJ_OBJECT))
        {
            result = OKJ_ERROR_BAD_OBJECT;
        }
        else
        {
            out_obj->start  = parser->tokens[idx].start;
            out_obj->count  = okj_count_object_members(parser->tokens[idx].start);
            out_obj->length = okj_measure_container(parser->tokens[idx].start);
        }
    }

    return result;
}

/*@
  // 1. Preconditions
  requires str == \null || \valid_read(str);
  
  // If the string object is valid and has a length, the data it points to MUST be readable.
  requires str != \null && str->length > 0 ==> \valid_read(str->start + (0 .. str->length - 1));
  
  // If a buffer is provided, it MUST be safely writable up to buf_size.
  requires buf != \null && buf_size > 0 ==> \valid(buf + (0 .. buf_size - 1));

  // 2. Behaviors
  behavior invalid_args:
    assumes str == \null || buf == \null || buf_size == 0;
    assigns \nothing;
    ensures \result == 0;

  behavior valid_copy:
    assumes str != \null && buf != \null && buf_size > 0;
    
    // The function is only allowed to modify the provided buffer up to buf_size.
    assigns buf[0 .. buf_size - 1];
    
    // Prove the mathematical logic of the copy length boundary.
    ensures \result == (str->length >= buf_size ? buf_size - 1 : str->length);
    
    // Prove the string is always safely null-terminated!
    ensures buf[\result] == '\0';

  complete behaviors;
  disjoint behaviors;
*/
uint16_t okj_copy_string(const OkJsonString *str, char *buf, uint16_t buf_size)
{
    uint16_t copy_len = 0U;

    if ((str != NULL) && (buf != NULL) && (buf_size != 0U))
    {
        /* Copy at most (buf_size - 1) bytes to leave room for the null terminator. */
        copy_len = str->length;

        if (copy_len >= buf_size)
        {
            copy_len = (uint16_t)(buf_size - 1U);
        }

        uint16_t i;

        /*@
          // LOOP INVARIANTS
          // i never exceeds the bounded copy_len
          loop invariant 0 <= i <= copy_len;
          
          // The loop only modifies the buffer and the index variable
          loop assigns i, buf[0 .. copy_len - 1];
          
          loop variant copy_len - i;
        */
        for (i = 0U; i < copy_len; i++)
        {
            buf[i] = str->start[i];
        }

        buf[copy_len] = '\0';
    }

    return copy_len;
}

/*@
  // 1. Preconditions
  requires parser == \null || \valid_read(parser);
  requires parser != \null ==> parser->token_count <= OKJ_MAX_TOKENS;

  // 2. Frame Condition
  assigns \nothing;

  // 3. Behaviors
  behavior is_null:
    assumes parser == \null;
    ensures \result == 0;

  behavior valid_parser:
    assumes parser != \null;
    // Mathematically guarantee the count never exceeds the token total
    ensures \result <= parser->token_count;

  complete behaviors;
  disjoint behaviors;
*/
uint16_t okj_count_objects(const OkJsonParser *parser)
{
    uint16_t count = 0U;

    if (parser != NULL)
    {
        uint16_t i;

        /*@
          loop invariant 0 <= i <= parser->token_count;
          
          // Prove the count increments safely and never exceeds the current index
          loop invariant count <= i;
          
          loop assigns i, count;
          loop variant parser->token_count - i;
        */
        for (i = 0U; i < parser->token_count; i++)
        {
            if (parser->tokens[i].type == OKJ_OBJECT)
            {
                count++;
            }
        }
    }

    return count;
}

/*@
  // 1. Preconditions
  requires parser == \null || \valid_read(parser);
  requires parser != \null ==> parser->token_count <= OKJ_MAX_TOKENS;

  // 2. Frame Condition
  assigns \nothing;

  // 3. Behaviors
  behavior is_null:
    assumes parser == \null;
    ensures \result == 0;

  behavior valid_parser:
    assumes parser != \null;
    ensures \result <= parser->token_count;

  complete behaviors;
  disjoint behaviors;
*/
uint16_t okj_count_arrays(const OkJsonParser *parser)
{
    uint16_t count = 0U;

    if (parser != NULL)
    {
        uint16_t i;

        /*@
          loop invariant 0 <= i <= parser->token_count;
          loop invariant count <= i;
          
          loop assigns i, count;
          loop variant parser->token_count - i;
        */
        for (i = 0U; i < parser->token_count; i++)
        {
            if (parser->tokens[i].type == OKJ_ARRAY)
            {
                count++;
            }
        }
    }

    return count;
}

/*@
  // 1. Preconditions
  requires parser == \null || \valid_read(parser);

  // 2. Frame Condition
  assigns \nothing;

  // 3. Behaviors
  behavior is_null:
    assumes parser == \null;
    ensures \result == 0;

  behavior valid_parser:
    assumes parser != \null;
    ensures \result == parser->token_count;

  complete behaviors;
  disjoint behaviors;
*/
uint16_t okj_count_elements(const OkJsonParser *parser)
{
    uint16_t result = 0U;

    if (parser != NULL)
    {
        result = parser->token_count;
    }

    return result;
}

/* ---------------------------------------------------------------------------
 * Debug print — only compiled when OK_JSON_DEBUG is defined
 * ---------------------------------------------------------------------------*/
/* GCOVR_EXCL_START */
#ifdef OK_JSON_DEBUG

/**
 * Suppressing the following include line due to it being part of the conditional
 * debug build code and used only for development/debugging purposes.  DO NOT USE
 * THIS DEBUG SECTION IN YOUR PRODUCTION CODE!
 */
/* cppcheck-suppress misra-c2012-20.1 */
/* cppcheck-suppress misra-c2012-21.6 */
/* cppcheck-suppress missingIncludeSystem */
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

/**
 * Suppressing the following function due to it being part of the conditional
 * debug build code and used only for development/debugging purposes.  DO NOT USE
 * THIS DEBUG SECTION IN YOUR PRODUCTION CODE!
 */
/* cppcheck-suppress unusedFunction */
void okj_debug_print(OkJsonParser *parser)
{
    if (parser == NULL)
    {
        (void)printf("okj_debug_print: NULL parser\n");
    }
    else
    {
        (void)printf("=== OK_JSON Debug Dump: %u token(s) ===\n",
                    (unsigned int)parser->token_count);

        uint16_t i;

        for (i = 0U; i < parser->token_count; i++)
        {
            const OkJsonToken *t = &parser->tokens[i];

            if (t != NULL)
            {
                uint16_t dlen = t->length;

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
                    uint16_t j;

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
    }
}

#endif /* OK_JSON_DEBUG */
/* GCOVR_EXCL_STOP */
