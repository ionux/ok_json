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

#include <stdio.h>
#include <assert.h>

void test_parse_simple_object(void);
void test_parse_array(void);
void test_invalid_json(void);
void test_get_string(void);
void test_get_number(void);
void test_get_boolean(void);
void test_get_not_found(void);
void test_max_tokens_exceeded(void);
void test_truncated_string(void);

/**
 * These tests are a work in progress. If you have ideas
 * for additional or improved test cases, please submit
 * a pull request.
 *
 * Thanks,
 * Rich M.
 */

void test_parse_simple_object(void)
{
    /* Parse a basic JSON object and test for two things:
     *   - Ensure parsing succeeds
     *   - Parse the correct number of tokens (three: container, key, value)
     */

    OkJsonParser parser;
    char json_str[] = "{\"key\": 42}";

    okj_init(&parser, json_str);

    OkjError result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 3);

    assert(parser.tokens[0].type == OKJ_OBJECT);
    assert(parser.tokens[1].type == OKJ_STRING);   /* "key"  */
    assert(parser.tokens[2].type == OKJ_NUMBER);   /* 42     */

    printf("✅ test_parse_simple_object passed!\n");
}

void test_parse_array(void)
{
    /* Parse a basic JSON array and test for two things:
     *   - Ensure parsing succeeds
     *   - Parse the correct number of elements (four, in this case)
     */

    OkJsonParser parser;
    char json_str[] = "[1, 2, 3]";

    okj_init(&parser, json_str);

    OkjError result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 4); /* 3 numbers + array itself */

    assert(parser.tokens[0].type == OKJ_ARRAY);
    assert(parser.tokens[1].type == OKJ_NUMBER);
    assert(parser.tokens[2].type == OKJ_NUMBER);
    assert(parser.tokens[3].type == OKJ_NUMBER);

    printf("✅ test_parse_array passed!\n");
}

void test_invalid_json(void)
{
    /* Attempt to parse an invalid JSON object and check for one thing:
     *   - Ensure parsing fails
     */

    OkJsonParser parser;
    char json_str[] = "{key: 42}"; /* Invalid: missing quotes */

    okj_init(&parser, json_str);

    OkjError result = okj_parse(&parser);
    assert(result != OKJ_SUCCESS); /* Expect failure */

    printf("✅ test_invalid_json passed!\n");
}

void test_get_string(void)
{
    /* Parse an object containing a string value and verify that
     * okj_get_string() returns the correct content and length. */

    OkJsonParser  parser;
    OkJsonString *str;
    char json_str[] = "{\"name\": \"Alice\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "name");

    assert(str != NULL);
    assert(str->length == 5U);       /* "Alice" is 5 characters */
    assert(str->start[0] == 'A');

    printf("✅ test_get_string passed!\n");
}

void test_get_number(void)
{
    /* Parse an object containing a number value and verify that
     * okj_get_number() returns the correct content and length. */

    OkJsonParser  parser;
    OkJsonNumber *num;
    char json_str[] = "{\"age\": 30}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "age");

    assert(num != NULL);
    assert(num->length == 2U);       /* "30" is 2 characters */
    assert(num->start[0] == '3');

    printf("✅ test_get_number passed!\n");
}

void test_get_boolean(void)
{
    /* Parse an object containing a boolean value and verify that
     * okj_get_boolean() returns the correct content and length. */

    OkJsonParser   parser;
    OkJsonBoolean *flag;
    char json_str[] = "{\"active\": true}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    flag = okj_get_boolean(&parser, "active");

    assert(flag != NULL);
    assert(flag->length == 4U);      /* "true" is 4 characters */
    assert(flag->start[0] == 't');

    printf("✅ test_get_boolean passed!\n");
}

void test_get_not_found(void)
{
    /* Verify that getters return NULL for a missing key and for a
     * type mismatch (key exists but holds a different type). */

    OkJsonParser  parser;
    char json_str[] = "{\"key\": 42}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* Key does not exist */
    assert(okj_get_number(&parser,  "missing") == NULL);

    /* Key exists but type is NUMBER, not STRING */
    assert(okj_get_string(&parser,  "key") == NULL);

    /* Key exists but type is NUMBER, not BOOLEAN */
    assert(okj_get_boolean(&parser, "key") == NULL);

    printf("✅ test_get_not_found passed!\n");
}

void test_max_tokens_exceeded(void)
{
    /* Build a JSON array with more elements than OKJ_MAX_TOKENS (128).
     * The parser should report OKJ_ERROR_MAX_TOKENS_EXCEEDED rather
     * than silently truncating the input. */

    OkJsonParser parser;
    OkjError     result;

    /* "[0,0,0,...]" with 130 elements = 131 tokens (1 array + 130 numbers) */
    char json_str[524];  /* 1('[') + 130*2('0,') - 1(no trailing ',') + 1(']') + 1('\0') */
    uint16_t i;
    uint16_t pos = 0U;

    json_str[pos++] = '[';
    for (i = 0U; i < 130U; i++)
    {
        if (i > 0U) { json_str[pos++] = ','; }
        json_str[pos++] = '0';
    }
    json_str[pos++] = ']';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_TOKENS_EXCEEDED);

    printf("✅ test_max_tokens_exceeded passed!\n");
}

void test_truncated_string(void)
{
    /* Attempt to parse a JSON string that is missing its closing quote.
     * The parser should report a failure rather than succeeding. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"unclosed";  /* no closing '"' on the key */

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result != OKJ_SUCCESS);

    printf("✅ test_truncated_string passed!\n");
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    test_parse_simple_object();
    test_parse_array();
    test_invalid_json();
    test_get_string();
    test_get_number();
    test_get_boolean();
    test_get_not_found();
    test_max_tokens_exceeded();
    test_truncated_string();

    printf("All OK_JSON tests passed!\n");

    return 0;
}
