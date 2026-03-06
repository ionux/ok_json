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
void test_number_negative(void);
void test_number_float(void);
void test_number_exponent(void);
void test_number_zero_variants(void);
void test_number_invalid_lone_minus(void);
void test_number_invalid_leading_zero(void);
void test_number_invalid_trailing_decimal(void);
void test_number_invalid_exponent_no_digits(void);
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
void test_escape_newline(void);
void test_escape_other_single_char(void);
void test_escape_unicode_valid(void);
void test_escape_unicode_invalid_hex(void);
void test_escape_unicode_truncated(void);
void test_escape_unknown(void);
void test_array_too_large(void);
void test_object_too_large(void);
void test_get_array_raw(void);
void test_get_object_raw(void);
void test_count_objects(void);
void test_count_arrays(void);
void test_count_elements(void);
void test_debug_print(void);
void test_empty_object(void);
void test_empty_array(void);
void test_nested_object(void);
void test_nested_array_in_object(void);
void test_temp_negative_number(void);
void test_null_value(void);
void test_key_exactly_64_chars(void);
void test_key_65_chars_error(void);
void test_deeply_nested_at_limit(void);
void test_max_json_len_exceeded(void);
void test_parse_null_parser(void);
void test_truncated_backslash_at_eof(void);
void test_copy_string_basic(void);
void test_copy_string_null_terminated(void);
void test_copy_string_truncation(void);
void test_copy_string_exact_fit(void);
void test_copy_string_null_inputs(void);
void test_find_key_over_max_len(void);

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
    assert(result == OKJ_ERROR_SYNTAX); /* 'k' is not a valid value start */

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
    assert(okj_parse(&parser) == OKJ_ERROR_SYNTAX); /* 'A' is not a valid value start */

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

void test_number_negative(void)
{
    /* Parse a negative integer and verify the token covers the minus sign
     * and both digits. */

    OkJsonParser  parser;
    OkJsonNumber *num;
    char json_str[] = "{\"n\": -42}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 3U);      /* '-', '4', '2' */
    assert(num->start[0] == '-');

    printf("test_number_negative passed!\n");
}

void test_number_float(void)
{
    /* Parse positive and negative decimal numbers and verify token lengths. */

    OkJsonParser  parser;
    OkJsonNumber *num;
    char json1[] = "{\"n\": 3.14}";
    char json2[] = "{\"n\": -1.5}";

    /* 3.14 */
    okj_init(&parser, json1);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 4U);      /* '3', '.', '1', '4' */
    assert(num->start[0] == '3');

    /* -1.5 */
    okj_init(&parser, json2);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 4U);      /* '-', '1', '.', '5' */
    assert(num->start[0] == '-');

    printf("test_number_float passed!\n");
}

void test_number_exponent(void)
{
    /* Parse numbers that use exponent notation (RFC 8259 §6).
     * Both 'e' and 'E' forms, with and without an explicit sign, must
     * be accepted and their full raw text captured in the token. */

    OkJsonParser  parser;
    OkJsonNumber *num;
    char json1[] = "{\"n\": 1e10}";
    char json2[] = "{\"n\": 2.5E-3}";
    char json3[] = "{\"n\": 1E+2}";

    /* 1e10 */
    okj_init(&parser, json1);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 4U);      /* '1', 'e', '1', '0' */
    assert(num->start[0] == '1');

    /* 2.5E-3 */
    okj_init(&parser, json2);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 6U);      /* '2', '.', '5', 'E', '-', '3' */
    assert(num->start[0] == '2');

    /* 1E+2 */
    okj_init(&parser, json3);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 4U);      /* '1', 'E', '+', '2' */
    assert(num->start[0] == '1');

    printf("test_number_exponent passed!\n");
}

void test_number_zero_variants(void)
{
    /* Verify that bare zero and negative zero are both accepted. */

    OkJsonParser  parser;
    OkJsonNumber *num;
    char json1[] = "{\"n\": 0}";
    char json2[] = "{\"n\": -0}";

    /* 0 */
    okj_init(&parser, json1);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 1U);
    assert(num->start[0] == '0');

    /* -0 */
    okj_init(&parser, json2);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    assert(num != NULL);
    assert(num->length == 2U);
    assert(num->start[0] == '-');

    printf("test_number_zero_variants passed!\n");
}

void test_number_invalid_lone_minus(void)
{
    /* A bare '-' with no following digits is not a valid JSON number. */

    OkJsonParser parser;
    char json_str[] = "{\"n\": -}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_NUMBER);

    printf("test_number_invalid_lone_minus passed!\n");
}

void test_number_invalid_leading_zero(void)
{
    /* RFC 8259 §6 forbids a leading zero followed by another digit.
     * "012" must be rejected; "0" alone or "0.5" are valid. */

    OkJsonParser parser;
    char json_str[] = "{\"n\": 012}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_NUMBER);

    printf("test_number_invalid_leading_zero passed!\n");
}

void test_number_invalid_trailing_decimal(void)
{
    /* A decimal point must be followed by at least one digit.
     * "1." is not a valid JSON number. */

    OkJsonParser parser;
    char json_str[] = "{\"n\": 1.}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_NUMBER);

    printf("test_number_invalid_trailing_decimal passed!\n");
}

void test_number_invalid_exponent_no_digits(void)
{
    /* An exponent marker ('e'/'E') must be followed by at least one digit
     * (after an optional sign).  Both "1e" and "1e+" must be rejected. */

    OkJsonParser parser;
    char json1[] = "{\"n\": 1e}";
    char json2[] = "{\"n\": 1e+}";

    okj_init(&parser, json1);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_NUMBER);

    okj_init(&parser, json2);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_NUMBER);

    printf("test_number_invalid_exponent_no_digits passed!\n");
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

    assert(result == OKJ_ERROR_UNEXPECTED_END); /* EOF before closing '"' */

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

void test_escape_newline(void)
{
    /* Parse a string containing \n (RFC 8259 newline escape).
     * The parser stores raw bytes, so the token must contain the two
     * source bytes '\' and 'n', not a decoded newline character. */

    OkJsonParser  parser;
    OkJsonString *str;

    /* JSON: {"msg": "line1\nline2"} — 'line1\nline2' is 12 raw bytes */
    char json_str[] = "{\"msg\": \"line1\\nline2\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "msg");

    assert(str != NULL);
    assert(str->length == 12U);     /* l,i,n,e,1,\,n,l,i,n,e,2 */
    assert(str->start[5] == '\\');  /* raw backslash byte */
    assert(str->start[6] == 'n');   /* literal 'n' byte */

    printf("test_escape_newline passed!\n");
}

void test_escape_other_single_char(void)
{
    /* Parse a string containing \t, \r, \b, \f, and \/ — all valid
     * single-character RFC 8259 escape sequences beyond \" and \\.
     * Token must contain 10 raw bytes (two per escape). */

    OkJsonParser  parser;
    OkJsonString *str;

    /* JSON: {"msg": "\t\r\b\f\/"} — 10 raw bytes */
    char json_str[] = "{\"msg\": \"\\t\\r\\b\\f\\/\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "msg");

    assert(str != NULL);
    assert(str->length == 10U);     /* \,t,\,r,\,b,\,f,\,/ */
    assert(str->start[0] == '\\');
    assert(str->start[1] == 't');

    printf("test_escape_other_single_char passed!\n");
}

void test_escape_unicode_valid(void)
{
    /* Parse a string containing a valid \uXXXX escape (\u0041 = 'A').
     * The parser stores raw bytes, so the token must be 6 bytes long:
     * '\', 'u', '0', '0', '4', '1'. */

    OkJsonParser  parser;
    OkJsonString *str;

    /* JSON: {"ch": "\u0041"} — 6 raw bytes in the value */
    char json_str[] = "{\"ch\": \"\\u0041\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "ch");

    assert(str != NULL);
    assert(str->length == 6U);      /* \,u,0,0,4,1 */
    assert(str->start[0] == '\\');
    assert(str->start[1] == 'u');

    printf("test_escape_unicode_valid passed!\n");
}

void test_escape_unicode_invalid_hex(void)
{
    /* Attempt to parse a string containing \u004G where 'G' is not a
     * valid hexadecimal digit.  The parser must reject this input. */

    OkJsonParser parser;

    /* JSON: {"ch": "\u004G"} — 'G' is not a hex digit */
    char json_str[] = "{\"ch\": \"\\u004G\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_STRING);

    printf("test_escape_unicode_invalid_hex passed!\n");
}

void test_escape_unicode_truncated(void)
{
    /* Attempt to parse a string containing \u followed by only 2 hex
     * digits instead of the required 4.  The parser must reject this. */

    OkJsonParser parser;

    /* JSON: {"ch": "\u00"} — only 2 hex digits after \u */
    char json_str[] = "{\"ch\": \"\\u00\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_STRING);

    printf("test_escape_unicode_truncated passed!\n");
}

void test_escape_unknown(void)
{
    /* Attempt to parse a string containing \q, which is not a valid
     * JSON escape sequence.  The parser must reject this input. */

    OkJsonParser parser;

    /* JSON: {"msg": "hello\qworld"} — \q is not a valid escape */
    char json_str[] = "{\"msg\": \"hello\\qworld\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_ERROR_BAD_STRING);

    printf("test_escape_unknown passed!\n");
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

void test_get_array_raw(void)
{
    /* Parse an object containing an array and verify that okj_get_array_raw()
     * returns the correct element count AND the full raw byte length of the
     * array text (including surrounding brackets). */

    OkJsonParser parser;
    OkJsonArray *arr;

    /* "[1, 2, 3]" = 9 bytes */
    char json_str[] = "{\"items\": [1, 2, 3]}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    arr = okj_get_array_raw(&parser, "items");

    assert(arr != NULL);
    assert(arr->count  == 3U);
    assert(arr->start[0] == '[');
    assert(arr->length == 9U);   /* [1, 2, 3] = 9 bytes */

    printf("test_get_array_raw passed!\n");
}

void test_get_object_raw(void)
{
    /* Parse an object containing a nested object and verify that
     * okj_get_object_raw() returns the correct member count AND the full raw
     * byte length of the object text (including surrounding braces). */

    OkJsonParser  parser;
    OkJsonObject *obj;

    /* {"a": 1} = 8 bytes */
    char json_str[] = "{\"info\": {\"a\": 1}}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    obj = okj_get_object_raw(&parser, "info");

    assert(obj != NULL);
    assert(obj->count  == 1U);
    assert(obj->start[0] == '{');
    assert(obj->length == 8U);   /* {"a": 1} = 8 bytes */

    printf("test_get_object_raw passed!\n");
}

void test_count_objects(void)
{
    /* Parse a JSON value that contains two object tokens (one outer, one
     * nested) and verify that okj_count_objects() returns 2. */

    OkJsonParser parser;
    char json_str[] = "{\"a\": {\"b\": 1}, \"c\": 2}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    assert(okj_count_objects(&parser) == 2U);

    printf("test_count_objects passed!\n");
}

void test_count_arrays(void)
{
    /* Parse a JSON value that contains two array tokens and verify that
     * okj_count_arrays() returns 2. */

    OkJsonParser parser;
    char json_str[] = "{\"x\": [1, 2], \"y\": [3]}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    assert(okj_count_arrays(&parser) == 2U);

    printf("test_count_arrays passed!\n");
}

void test_count_elements(void)
{
    /* Parse a simple object and verify that okj_count_elements() equals the
     * total token count: 1 object + 1 string key + 1 number value = 3. */

    OkJsonParser parser;
    char json_str[] = "{\"key\": 42}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    assert(okj_count_elements(&parser) == 3U);

    printf("test_count_elements passed!\n");
}

void test_debug_print(void)
{
    /* Parse a JSON value and call okj_debug_print() to exercise the full
     * token dump path.  Correctness is verified visually in the output;
     * this test ensures no crash and no assertion failure. */

    OkJsonParser parser;
    char json_str[] = "{\"key\": 42, \"arr\": [1, 2]}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

#ifdef OK_JSON_DEBUG
    okj_debug_print(&parser);
#endif

    printf("test_debug_print passed!\n");
}

void test_empty_object(void)
{
    /* Verify that a standalone empty object {} is parsed successfully,
     * produces exactly one token, and that okj_count_objects() returns 1.
     * Also verify that a nested empty object value is accessible via
     * okj_get_object() and reports a member count of 0. */

    OkJsonParser  parser;
    OkJsonObject *obj;

    /* Standalone empty object */
    char json1[] = "{}";

    okj_init(&parser, json1);
    assert(okj_parse(&parser) == OKJ_SUCCESS);
    assert(parser.token_count == 1U);          /* only the OKJ_OBJECT token */
    assert(parser.tokens[0].type == OKJ_OBJECT);
    assert(okj_count_objects(&parser) == 1U);

    /* Empty object as a value — getter must return non-NULL with count 0 */
    char json2[] = "{\"k\": {}}";

    okj_init(&parser, json2);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    obj = okj_get_object(&parser, "k");

    assert(obj != NULL);
    assert(obj->count == 0U);

    printf("test_empty_object passed!\n");
}

void test_empty_array(void)
{
    /* Verify that a standalone empty array [] is parsed successfully,
     * produces exactly one token, and that okj_count_arrays() returns 1.
     * Also verify that a nested empty array value is accessible via
     * okj_get_array() and reports an element count of 0. */

    OkJsonParser parser;
    OkJsonArray *arr;

    /* Standalone empty array */
    char json1[] = "[]";

    okj_init(&parser, json1);
    assert(okj_parse(&parser) == OKJ_SUCCESS);
    assert(parser.token_count == 1U);          /* only the OKJ_ARRAY token */
    assert(parser.tokens[0].type == OKJ_ARRAY);
    assert(okj_count_arrays(&parser) == 1U);

    /* Empty array as a value — getter must return non-NULL with count 0 */
    char json2[] = "{\"k\": []}";

    okj_init(&parser, json2);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    arr = okj_get_array(&parser, "k");

    assert(arr != NULL);
    assert(arr->count == 0U);

    printf("test_empty_array passed!\n");
}

void test_nested_object(void)
{
    /* Parse {"outer": {"inner": 1}} and verify:
     *   - Parsing succeeds
     *   - Token count is 5: outer object, "outer" key, inner object,
     *                        "inner" key, number 1
     *   - The inner OKJ_OBJECT token is present at index 2
     *   - okj_get_object() for "outer" returns non-NULL with member count 1 */

    OkJsonParser  parser;
    OkJsonObject *obj;
    char json_str[] = "{\"outer\": {\"inner\": 1}}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);
    assert(parser.token_count == 5U);

    assert(parser.tokens[0].type == OKJ_OBJECT);   /* outer { } */
    assert(parser.tokens[1].type == OKJ_STRING);   /* "outer"   */
    assert(parser.tokens[2].type == OKJ_OBJECT);   /* inner { } */
    assert(parser.tokens[3].type == OKJ_STRING);   /* "inner"   */
    assert(parser.tokens[4].type == OKJ_NUMBER);   /* 1         */

    obj = okj_get_object(&parser, "outer");

    assert(obj != NULL);
    assert(obj->count == 1U);   /* one key-value member in the inner object */

    printf("test_nested_object passed!\n");
}

void test_nested_array_in_object(void)
{
    /* Parse {"list": [1, 2, 3]} and verify that okj_get_array() returns
     * non-NULL with the correct element count. */

    OkJsonParser parser;
    OkJsonArray *arr;
    char json_str[] = "{\"list\": [1, 2, 3]}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    arr = okj_get_array(&parser, "list");

    assert(arr != NULL);
    assert(arr->count == 3U);

    printf("test_nested_array_in_object passed!\n");
}

void test_temp_negative_number(void)
{
    /* Parse {"temp": -42} and verify that okj_get_number() returns a token
     * covering all three raw bytes: '-', '4', '2'. */

    OkJsonParser  parser;
    OkJsonNumber *num;
    char json_str[] = "{\"temp\": -42}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "temp");

    assert(num != NULL);
    assert(num->length == 3U);      /* '-', '4', '2' */
    assert(num->start[0] == '-');

    printf("test_temp_negative_number passed!\n");
}

void test_null_value(void)
{
    /* Parse {"x": null} and verify that okj_get_token() returns a token
     * with type OKJ_NULL and the correct raw byte length. */

    OkJsonParser  parser;
    OkJsonToken  *tok;
    char json_str[] = "{\"x\": null}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    tok = okj_get_token(&parser, "x");

    assert(tok != NULL);
    assert(tok->type   == OKJ_NULL);
    assert(tok->length == 4U);      /* 'n', 'u', 'l', 'l' */

    printf("test_null_value passed!\n");
}

void test_key_exactly_64_chars(void)
{
    /* Build {"<64 x's>": 1} — a key that is exactly OKJ_MAX_STRING_LEN
     * bytes long.  Parsing must succeed. */

    OkJsonParser parser;
    OkjError     result;

    /* Layout: { " <64 chars> " : 1 } \0
     *          1  1    64    1  1 1 1  1  = 71 bytes  */
    char     json_str[71];
    uint16_t pos = 0U;
    uint16_t i;

    json_str[pos++] = '{';
    json_str[pos++] = '"';

    for (i = 0U; i < 64U; i++)
    {
        json_str[pos++] = 'x';
    }

    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '1';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);

    printf("test_key_exactly_64_chars passed!\n");
}

void test_key_65_chars_error(void)
{
    /* Build {"<65 x's>": 1} — a key one byte beyond OKJ_MAX_STRING_LEN.
     * The parser must return OKJ_ERROR_MAX_STR_LEN_EXCEEDED. */

    OkJsonParser parser;
    OkjError     result;

    /* Layout: { " <65 chars> " : 1 } \0
     *          1  1    65    1  1 1 1  1  = 72 bytes  */
    char     json_str[72];
    uint16_t pos = 0U;
    uint16_t i;

    json_str[pos++] = '{';
    json_str[pos++] = '"';

    for (i = 0U; i < 65U; i++)
    {
        json_str[pos++] = 'x';
    }

    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '1';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_STR_LEN_EXCEEDED);

    printf("test_key_65_chars_error passed!\n");
}

void test_deeply_nested_at_limit(void)
{
    /* Build a deeply nested object: {"a":{"a":{"a":...{"a":1}...}}}
     * with enough levels to push the token count beyond OKJ_MAX_TOKENS (128).
     *
     * Token count for N levels = 2*N + 1 (N objects + N string keys + 1 number).
     * At N=63: 127 tokens  -> OKJ_SUCCESS
     * At N=64: 129 tokens  -> OKJ_ERROR_MAX_TOKENS_EXCEEDED
     *
     * Both outcomes are deterministic and safe; neither is a crash or
     * silent corruption. */

    OkJsonParser parser;
    OkjError     result;

    /* N=63: 63*5 + 1 + 63 = 379 chars + NUL = 380 bytes */
    char json63[380];
    uint16_t pos = 0U;
    uint16_t i;

    for (i = 0U; i < 63U; i++)
    {
        json63[pos++] = '{';
        json63[pos++] = '"';
        json63[pos++] = 'a';
        json63[pos++] = '"';
        json63[pos++] = ':';
    }

    json63[pos++] = '1';

    for (i = 0U; i < 63U; i++)
    {
        json63[pos++] = '}';
    }

    json63[pos] = '\0';

    okj_init(&parser, json63);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 127U);  /* 2*63 + 1 */

    /* N=64: 64*5 + 1 + 64 = 385 chars + NUL = 386 bytes */
    char json64[386];
    pos = 0U;

    for (i = 0U; i < 64U; i++)
    {
        json64[pos++] = '{';
        json64[pos++] = '"';
        json64[pos++] = 'a';
        json64[pos++] = '"';
        json64[pos++] = ':';
    }

    json64[pos++] = '1';

    for (i = 0U; i < 64U; i++)
    {
        json64[pos++] = '}';
    }

    json64[pos] = '\0';

    okj_init(&parser, json64);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_TOKENS_EXCEEDED);

    printf("test_deeply_nested_at_limit passed!\n");
}

void test_max_json_len_exceeded(void)
{
    /* Build a JSON string longer than OKJ_MAX_JSON_LEN (4096) bytes.
     * The parser must reject it before tokenising and return
     * OKJ_ERROR_MAX_JSON_LEN_EXCEEDED. */

    OkJsonParser parser;
    OkjError     result;

    /* Array of 4098 zeros: '[' + 4096 '0' chars + ']' + '\0' = 4099 bytes.
     * The raw JSON length (4098 bytes excluding NUL) exceeds the 4096 limit. */
    char     json_str[4099];
    uint16_t pos = 0U;
    uint16_t i;

    json_str[pos++] = '[';

    for (i = 0U; i < 4096U; i++)
    {
        json_str[pos++] = '0';
    }

    json_str[pos++] = ']';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_JSON_LEN_EXCEEDED);

    printf("test_max_json_len_exceeded passed!\n");
}

void test_parse_null_parser(void)
{
    /* Passing NULL as the parser pointer must return OKJ_ERROR_BAD_POINTER
     * without accessing any memory. */

    OkjError result = okj_parse(NULL);

    assert(result == OKJ_ERROR_BAD_POINTER);

    printf("test_parse_null_parser passed!\n");
}

void test_truncated_backslash_at_eof(void)
{
    /* A string that ends with a lone backslash (no following escape character)
     * must return OKJ_ERROR_UNEXPECTED_END, not OKJ_ERROR_SYNTAX. */

    OkJsonParser parser;
    OkjError     result;

    /* JSON: {"k": "val\<EOF>} — backslash is the last byte before NUL */
    char json_str[] = "{\"k\": \"val\\";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_UNEXPECTED_END);

    printf("test_truncated_backslash_at_eof passed!\n");
}

void test_copy_string_basic(void)
{
    /* Parse an object, retrieve a string, and verify that okj_copy_string()
     * writes the correct content into the destination buffer. */

    OkJsonParser  parser;
    OkJsonString *str;
    char          buf[16];
    uint16_t      copied;

    char json_str[] = "{\"city\": \"Paris\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "city");
    assert(str != NULL);

    copied = okj_copy_string(str, buf, (uint16_t)sizeof(buf));

    assert(copied == 5U);           /* "Paris" is 5 chars */
    assert(buf[0] == 'P');
    assert(buf[4] == 's');
    assert(buf[5] == '\0');         /* must be null-terminated */

    printf("test_copy_string_basic passed!\n");
}

void test_copy_string_null_terminated(void)
{
    /* Verify that the copied buffer is always null-terminated even when the
     * source string fills the buffer to its last usable byte. */

    OkJsonParser  parser;
    OkJsonString *str;
    char          buf[6];           /* exactly room for "Paris" + NUL */
    uint16_t      copied;

    char json_str[] = "{\"city\": \"Paris\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "city");
    assert(str != NULL);

    copied = okj_copy_string(str, buf, (uint16_t)sizeof(buf));

    assert(copied == 5U);
    assert(buf[5] == '\0');         /* null terminator must be present */

    printf("test_copy_string_null_terminated passed!\n");
}

void test_copy_string_truncation(void)
{
    /* When the destination buffer is smaller than the string, okj_copy_string()
     * must copy only (buf_size - 1) bytes and still null-terminate. */

    OkJsonParser  parser;
    OkJsonString *str;
    char          buf[4];           /* only 3 chars fit + NUL */
    uint16_t      copied;

    char json_str[] = "{\"word\": \"Hello\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "word");
    assert(str != NULL);
    assert(str->length == 5U);

    copied = okj_copy_string(str, buf, (uint16_t)sizeof(buf));

    assert(copied == 3U);           /* only 3 chars copied */
    assert(buf[0] == 'H');
    assert(buf[1] == 'e');
    assert(buf[2] == 'l');
    assert(buf[3] == '\0');         /* must still be null-terminated */

    printf("test_copy_string_truncation passed!\n");
}

void test_copy_string_exact_fit(void)
{
    /* A buffer of exactly (length + 1) bytes must hold the full string
     * with no truncation and a null terminator at the final position. */

    OkJsonParser  parser;
    OkJsonString *str;
    char          buf[5];           /* "Hi" (2 chars) + NUL, extra space */
    uint16_t      copied;

    char json_str[] = "{\"k\": \"Hi\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "k");
    assert(str != NULL);
    assert(str->length == 2U);

    /* buf_size == length + 1 == 3: exact fit */
    copied = okj_copy_string(str, buf, 3U);

    assert(copied == 2U);
    assert(buf[0] == 'H');
    assert(buf[1] == 'i');
    assert(buf[2] == '\0');

    printf("test_copy_string_exact_fit passed!\n");
}

void test_copy_string_null_inputs(void)
{
    /* okj_copy_string() must return 0 gracefully for all invalid arguments. */

    OkJsonParser  parser;
    OkJsonString *str;
    char          buf[8];
    uint16_t      result;

    char json_str[] = "{\"x\": \"abc\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "x");
    assert(str != NULL);

    /* NULL str pointer */
    result = okj_copy_string(NULL, buf, (uint16_t)sizeof(buf));
    assert(result == 0U);

    /* NULL buf pointer */
    result = okj_copy_string(str, NULL, (uint16_t)sizeof(buf));
    assert(result == 0U);

    /* buf_size == 0 */
    result = okj_copy_string(str, buf, 0U);
    assert(result == 0U);

    printf("test_copy_string_null_inputs passed!\n");
}

void test_find_key_over_max_len(void)
{
    /* When okj_get_string() is called with a key that exceeds OKJ_MAX_STRING_LEN
     * the lookup must return NULL rather than overreading the key buffer. */

    OkJsonParser  parser;
    OkJsonString *str;

    /* JSON contains a short key "k" mapped to "v". */
    char json_str[] = "{\"k\": \"v\"}";

    /* A key string of 65 'k' characters (one beyond OKJ_MAX_STRING_LEN). */
    char long_key[66];
    uint16_t i;

    for (i = 0U; i < 65U; i++)
    {
        long_key[i] = 'k';
    }
    long_key[65] = '\0';

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* The long key cannot match the 1-char stored key; must return NULL. */
    str = okj_get_string(&parser, long_key);
    assert(str == NULL);

    printf("test_find_key_over_max_len passed!\n");
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
    test_number_negative();
    test_number_float();
    test_number_exponent();
    test_number_zero_variants();
    test_number_invalid_lone_minus();
    test_number_invalid_leading_zero();
    test_number_invalid_trailing_decimal();
    test_number_invalid_exponent_no_digits();
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
    test_escape_newline();
    test_escape_other_single_char();
    test_escape_unicode_valid();
    test_escape_unicode_invalid_hex();
    test_escape_unicode_truncated();
    test_escape_unknown();
    test_array_too_large();
    test_object_too_large();
    test_get_array_raw();
    test_get_object_raw();
    test_count_objects();
    test_count_arrays();
    test_count_elements();
    test_debug_print();
    test_empty_object();
    test_empty_array();
    test_nested_object();
    test_nested_array_in_object();
    test_temp_negative_number();
    test_null_value();
    test_key_exactly_64_chars();
    test_key_65_chars_error();
    test_deeply_nested_at_limit();
    test_max_json_len_exceeded();
    test_parse_null_parser();
    test_truncated_backslash_at_eof();
    test_copy_string_basic();
    test_copy_string_null_terminated();
    test_copy_string_truncation();
    test_copy_string_exact_fit();
    test_copy_string_null_inputs();
    test_find_key_over_max_len();

    printf("All OK_JSON tests passed!\n");

    return 0;
}
