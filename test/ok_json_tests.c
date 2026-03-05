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
void test_invalid_string(void);
void test_get_number(void);
void test_invalid_number(void);
void test_get_boolean_true(void);
void test_get_boolean_false(void);
void test_invalid_boolean(void);
void test_get_not_found(void);
void test_max_tokens_exceeded(void);
void test_truncated_string(void);
void test_get_array_count(void);
void test_get_object_count(void);
void test_string_too_long(void);
void test_escaped_quote_in_string(void);
void test_escaped_backslash_in_string(void);
void test_array_too_large(void);
void test_object_too_large(void);

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

    printf("test_parse_simple_object passed!\n");
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

    printf("test_parse_array passed!\n");
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

    printf("test_invalid_json passed!\n");
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

    printf("test_get_string passed!\n");
}

void test_invalid_string(void)
{
    /* Parse an object containing an invalid string value and verify that
     * the operation fails. */

    OkJsonParser  parser;
    
    char json_str[] = "{\"name\": Alice}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) != OKJ_SUCCESS);

    printf("test_invalid_string passed!\n");
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

    printf("test_get_number passed!\n");
}

void test_invalid_number(void)
{
    /* Parse an object containing an invalid number value and verify that
     * the operation fails. */
    
    OkJsonParser  parser;
    OkJsonNumber *num;
    char json_str[] = "{\"age\": \"XYZ\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "age");

    assert(num == NULL);

    printf("test_invalid_number passed!\n");
}

void test_get_boolean_true(void)
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

    printf("test_get_boolean_true passed!\n");
}

void test_get_boolean_false(void)
{
    /* Parse an object containing a boolean value and verify that
     * okj_get_boolean() returns the correct content and length. */

    OkJsonParser   parser;
    OkJsonBoolean *flag;
    char json_str[] = "{\"active\": false}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    flag = okj_get_boolean(&parser, "active");

    assert(flag != NULL);
    assert(flag->length == 5U);      /* "false" is 4 characters */
    assert(flag->start[0] == 'f');

    printf("test_get_boolean_false passed!\n");
}

void test_invalid_boolean(void)
{
    /* Parse an object containing a boolean value and verify that
     * okj_get_boolean() returns the correct content and length. */

    OkJsonParser   parser;
    OkJsonBoolean *flag;
    char json_str[] = "{\"active\": 42}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    flag = okj_get_boolean(&parser, "active");

    assert(flag == NULL);

    printf("test_invalid_boolean passed!\n");
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

    printf("test_get_not_found passed!\n");
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

    printf("test_max_tokens_exceeded passed!\n");
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

    printf("test_truncated_string passed!\n");
}

void test_get_array_count(void)
{
    /* Parse an object that contains an array value and verify that
     * okj_get_array() returns the correct element count. */

    OkJsonParser parser;
    OkJsonArray *arr;
    char json_str[] = "{\"items\": [1, 2, 3]}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    arr = okj_get_array(&parser, "items");

    assert(arr != NULL);
    assert(arr->count == 3U);   /* three numeric elements */

    printf("test_get_array_count passed!\n");
}

void test_get_object_count(void)
{
    /* Parse an object that contains a nested object value and verify that
     * okj_get_object() returns the correct member count. */

    OkJsonParser  parser;
    OkJsonObject *obj;
    char json_str[] = "{\"info\": {\"a\": 1, \"b\": 2}}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    obj = okj_get_object(&parser, "info");

    assert(obj != NULL);
    assert(obj->count == 2U);   /* two key-value members */

    printf("test_get_object_count passed!\n");
}

void test_string_too_long(void)
{
    /* Attempt to parse an object whose string value exceeds OKJ_MAX_STRING_LEN
     * (64 bytes).  The parser should return OKJ_ERROR_MAX_STR_LEN_EXCEEDED.
     *
     * Buffer layout: {"k":"<65 x chars>"}
     *   '{'  = 1
     *   '"k"' = 3
     *   ':'  = 1
     *   '"'  = 1  (opening quote of value)
     *   65 x = 65
     *   '"'  = 1  (closing quote)
     *   '}'  = 1
     *   '\0' = 1
     *   Total = 74 bytes
     */

    OkJsonParser parser;
    OkjError     result;
    char         json_str[74];
    uint16_t     i;
    uint16_t     pos = 0U;

    json_str[pos++] = '{';
    json_str[pos++] = '"';
    json_str[pos++] = 'k';
    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '"';

    for (i = 0U; i < 65U; i++)
    {
        json_str[pos++] = 'x';
    }

    json_str[pos++] = '"';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_STR_LEN_EXCEEDED);

    printf("test_string_too_long passed!\n");
}

void test_escaped_quote_in_string(void)
{
    /* Parse a string value that contains an escaped double-quote (\").
     * The scanner must not treat \" as the closing quote; the token
     * length must cover all raw bytes including the backslash. */

    OkJsonParser  parser;
    OkJsonString *str;

    /* JSON: {"msg": "say \"hi\""} — value raw bytes: say \"hi\" (10 bytes) */
    char json_str[] = "{\"msg\": \"say \\\"hi\\\"\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "msg");

    assert(str != NULL);
    assert(str->length == 10U);     /* s,a,y, ,\,",h,i,\," */
    assert(str->start[0] == 's');

    printf("test_escaped_quote_in_string passed!\n");
}

void test_escaped_backslash_in_string(void)
{
    /* Parse a string value that contains an escaped backslash (\\).
     * The scanner must treat \\ as a single escape unit and continue,
     * not stop at the second backslash mistaking it for an escape prefix. */

    OkJsonParser  parser;
    OkJsonString *str;

    /* JSON: {"path": "a\\b"} — value raw bytes: a,\,\,b (4 bytes) */
    char json_str[] = "{\"path\": \"a\\\\b\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "path");

    assert(str != NULL);
    assert(str->length == 4U);      /* a, \, \, b */
    assert(str->start[0] == 'a');

    printf("test_escaped_backslash_in_string passed!\n");
}

void test_array_too_large(void)
{
    /* Build {"items": [1,1,...,1]} with 65 elements — one more than
     * OKJ_MAX_ARRAY_SIZE (64).  Parsing must succeed (only 68 tokens are
     * needed), but okj_get_array() must return NULL because the element
     * count exceeds the configured limit. */

    OkJsonParser parser;
    OkjError     result;
    OkJsonArray *arr;

    /* Buffer: {"items": [  65 ones with 64 commas  ]}  + NUL
     *   10 + (65 + 64) + 2 + 1 = 142 bytes             */
    char     json_str[142];
    uint16_t pos = 0U;
    uint16_t i;

    json_str[pos++] = '{';
    json_str[pos++] = '"';
    json_str[pos++] = 'i';
    json_str[pos++] = 't';
    json_str[pos++] = 'e';
    json_str[pos++] = 'm';
    json_str[pos++] = 's';
    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '[';

    for (i = 0U; i < 65U; i++)
    {
        if (i > 0U) { json_str[pos++] = ','; }
        json_str[pos++] = '1';
    }

    json_str[pos++] = ']';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);     /* 68 tokens — well within limit */

    arr = okj_get_array(&parser, "items");

    assert(arr == NULL);               /* 65 > OKJ_MAX_ARRAY_SIZE(64)   */

    printf("test_array_too_large passed!\n");
}

void test_object_too_large(void)
{
    /* Build {"data": {"k0":1,"k1":1,...,"k32":1}} — 33 members in the
     * nested object, one more than OKJ_MAX_OBJECT_SIZE (32).  Parsing
     * succeeds (69 tokens needed, within OKJ_MAX_TOKENS=128), but
     * okj_get_object() must return NULL because the member count exceeds
     * the configured limit. */

    OkJsonParser  parser;
    OkjError      result;
    OkJsonObject *obj;

    /* Worst-case size: 9 + 253 + 2 + 1 = 265 bytes (see comment below) */
    char     json_str[265];
    uint16_t pos = 0U;
    uint16_t i;

    /* Outer wrapper: {"data": { */
    json_str[pos++] = '{';
    json_str[pos++] = '"';
    json_str[pos++] = 'd';
    json_str[pos++] = 'a';
    json_str[pos++] = 't';
    json_str[pos++] = 'a';
    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '{';

    /* 33 members: "k0":1 … "k32":1 */
    for (i = 0U; i < 33U; i++)
    {
        if (i > 0U) { json_str[pos++] = ','; }

        json_str[pos++] = '"';
        json_str[pos++] = 'k';

        if (i < 10U)
        {
            json_str[pos++] = (char)('0' + (char)i);
        }
        else
        {
            json_str[pos++] = (char)('0' + (char)(i / 10U));
            json_str[pos++] = (char)('0' + (char)(i % 10U));
        }

        json_str[pos++] = '"';
        json_str[pos++] = ':';
        json_str[pos++] = '1';
    }

    json_str[pos++] = '}';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);     /* 69 tokens — well within limit */

    obj = okj_get_object(&parser, "data");

    assert(obj == NULL);               /* 33 > OKJ_MAX_OBJECT_SIZE(32)  */

    printf("test_object_too_large passed!\n");
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    test_parse_simple_object();
    test_parse_array();
    test_invalid_json();
    test_get_string();
    test_invalid_string();
    test_get_number();
    test_invalid_number();
    test_get_boolean_true();
    test_get_boolean_false();
    test_invalid_boolean();
    test_get_not_found();
    test_max_tokens_exceeded();
    test_truncated_string();
    test_get_array_count();
    test_get_object_count();
    test_string_too_long();
    test_escaped_quote_in_string();
    test_escaped_backslash_in_string();
    test_array_too_large();
    test_object_too_large();

    printf("All OK_JSON tests passed!\n");

    return 0;
}
