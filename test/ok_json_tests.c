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
void test_escape_unicode_surrogate_pair(void);
void test_escape_unknown(void);
void test_utf8_valid_multibyte(void);
void test_utf8_invalid_overlong(void);
void test_utf8_invalid_surrogate(void);
void test_utf8_invalid_truncated(void);
/* MC/DC tests for okj_validate_utf8_sequence */
void test_utf8_2byte_valid_boundary_min(void);
void test_utf8_2byte_valid_boundary_max(void);
void test_utf8_2byte_b1_not_continuation(void);
void test_utf8_3byte_e0_valid(void);
void test_utf8_3byte_e0_b1_below_range(void);
void test_utf8_3byte_e0_b1_above_range(void);
void test_utf8_3byte_e0_b2_not_continuation(void);
void test_utf8_3byte_e1_ec_boundary_min(void);
void test_utf8_3byte_e1_ec_boundary_max(void);
void test_utf8_3byte_ee_ef_boundary_min(void);
void test_utf8_3byte_ee_ef_boundary_max(void);
void test_utf8_3byte_range_b1_not_continuation(void);
void test_utf8_3byte_range_b2_not_continuation(void);
void test_utf8_3byte_ed_valid(void);
void test_utf8_3byte_ed_valid_boundary_max(void);
void test_utf8_3byte_ed_b1_below_range(void);
void test_utf8_3byte_ed_b2_not_continuation(void);
void test_utf8_4byte_f0_b1_below_range(void);
void test_utf8_4byte_f0_b1_above_range(void);
void test_utf8_4byte_f0_b2_not_continuation(void);
void test_utf8_4byte_f0_b3_not_continuation(void);
void test_utf8_4byte_f1_f3_valid(void);
void test_utf8_4byte_f1_f3_boundary_max(void);
void test_utf8_4byte_f1_f3_b1_not_continuation(void);
void test_utf8_4byte_f1_f3_b2_not_continuation(void);
void test_utf8_4byte_f1_f3_b3_not_continuation(void);
void test_utf8_4byte_f4_valid(void);
void test_utf8_4byte_f4_valid_boundary_max(void);
void test_utf8_4byte_f4_b1_below_range(void);
void test_utf8_4byte_f4_b1_above_range(void);
void test_utf8_4byte_f4_b2_not_continuation(void);
void test_utf8_4byte_f4_b3_not_continuation(void);
void test_array_too_large(void);
void test_object_too_large(void);
void test_object_exactly_32_members(void);
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
void test_max_json_len_boundary(void);
void test_parse_null_parser(void);
void test_null_pointer_safety(void);
void test_truncated_backslash_at_eof(void);
void test_copy_string_basic(void);
void test_copy_string_null_terminated(void);
void test_copy_string_truncation(void);
void test_copy_string_one_byte_buf(void);
void test_copy_string_exact_fit(void);
void test_copy_string_null_inputs(void);
void test_find_key_over_max_len(void);
void test_depth_stack_bracket_mismatch_obj(void);
void test_depth_stack_bracket_mismatch_arr(void);
void test_depth_stack_extra_close_brace(void);
void test_depth_stack_extra_close_bracket(void);
void test_depth_stack_unclosed_object(void);
void test_depth_stack_unclosed_array(void);
void test_depth_stack_mixed_nesting(void);
void test_depth_stack_alternating_unwind(void);
void test_trailing_whitespace_after_object(void);
void test_trailing_garbage_after_object(void);
void test_trailing_garbage_after_array(void);
void test_trailing_garbage_after_primitive(void);
void test_two_top_level_primitives(void);
void test_two_top_level_objects(void);
void test_trailing_comma_in_object(void);
void test_trailing_comma_in_array(void);
void test_number_as_object_key(void);
void test_boolean_as_object_key(void);
void test_null_as_object_key(void);
void test_missing_colon(void);
void test_colon_without_key(void);
void test_double_colon(void);
void test_empty_input(void);
void test_whitespace_only_input(void);
void test_keyword_no_boundary(void);
void test_value_after_value_in_object(void);
void test_comma_at_start_of_array(void);
void test_comma_at_start_of_object(void);
void test_top_level_number(void);
void test_top_level_string(void);
void test_top_level_boolean(void);
void test_top_level_null(void);
void test_iot_sensor_json(void);
void test_user_data_json(void);
void test_deeply_nested_valid_json(void);
void test_upper_limits_json(void);
void test_null_byte_in_string_value(void);
void test_duplicate_key_first_match_wins(void);
void test_empty_string_key(void);
void test_number_large_near_json_limit(void);
void test_rfc8259_all_whitespace_between_tokens(void);
void test_control_char_tab_in_string_value(void);
void test_control_char_lf_in_string_value(void);
void test_quoted_string_spoofing(void);
void test_utf8_overlong_nul_c0_80(void);
void test_backslash_flood_at_limit(void);
void test_backslash_flood_one_over(void);
void test_backslash_straddle_limit(void);
void test_backslash_truncated_at_boundary(void);
void test_okj_match_null_src_and_lit(void);

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

void test_escape_unicode_surrogate_pair(void)
{
    /* RFC 8259 allows code points outside the BMP to be encoded as a
     * 12-character surrogate pair (e.g. \uD83D\uDE00 for U+1F600 😀).
     * Because ok_json is a raw tokenizer it stores the escape sequences
     * verbatim, so the token for the value must be exactly 12 bytes:
     *   \,u,D,8,3,D,\,u,D,E,0,0
     * Neither \uXXXX sequence must trigger OKJ_ERROR_BAD_STRING because
     * every character in both sequences is a valid hexadecimal digit. */

    OkJsonParser  parser;
    OkJsonString *str;

    /* JSON: {"emoji": "\uD83D\uDE00"} */
    char json_str[] = "{\"emoji\": \"\\uD83D\\uDE00\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "emoji");

    assert(str != NULL);
    assert(str->length == 12U);     /* \,u,D,8,3,D,\,u,D,E,0,0 */
    assert(str->start[0] == '\\');
    assert(str->start[1] == 'u');
    assert(str->start[6] == '\\');  /* start of second \uXXXX */
    assert(str->start[7] == 'u');

    printf("test_escape_unicode_surrogate_pair passed!\n");
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


void test_utf8_valid_multibyte(void)
{
    /* RFC 8259 §8.1: UTF-8 encoded non-ASCII text in strings is valid. */

    OkJsonParser  parser;
    OkjError      result;
    OkJsonString *str;
    char json_str[] = "{\"s\":\"\xC3\xA9\xE6\xBC\xA2\xF0\x9F\x98\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);

    str = okj_get_string(&parser, "s");
    assert(str != NULL);
    assert(str->length == 9U);  /* C3 A9 E6 BC A2 F0 9F 98 80 */

    printf("test_utf8_valid_multibyte passed!\n");
}

void test_utf8_invalid_overlong(void)
{
    /* Overlong UTF-8 sequence (C0 AF) is forbidden by UTF-8 and RFC 8259 §8.1. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xC0\xAF\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);

    printf("test_utf8_invalid_overlong passed!\n");
}

void test_utf8_invalid_surrogate(void)
{
    /* UTF-8 encoding of surrogate code point U+D800 (ED A0 80) is invalid. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xED\xA0\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);

    printf("test_utf8_invalid_surrogate passed!\n");
}

void test_utf8_invalid_truncated(void)
{
    /* Truncated UTF-8 leading byte (C2 without continuation) is invalid. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xC2\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);

    printf("test_utf8_invalid_truncated passed!\n");
}

void test_utf8_overlong_nul_c0_80(void)
{
    /* Modified UTF-8 / overlong NUL attack: the two-byte sequence 0xC0 0x80
     * mathematically encodes U+0000 (NUL) but evades single-byte '\0' guards.
     *
     * Proof that okj_validate_utf8_sequence rejects it:
     *
     *   b0 = 0xC0 = 192
     *
     *   Step 1 – ASCII gate (line: b0 <= 0x7FU):
     *     0xC0 <= 0x7F  =>  192 <= 127  =>  FALSE
     *     Not handled as a single-byte ASCII character.
     *
     *   Step 2 – 2-byte leader gate (b0 >= 0xC2U && b0 <= 0xDFU):
     *     0xC0 >= 0xC2  =>  192 >= 194  =>  FALSE
     *     The range [0xC2, 0xDF] deliberately excludes 0xC0 and 0xC1 because
     *     any sequence starting with those bytes would be an overlong encoding
     *     of a code point that fits in fewer bits (U+0000–U+007F for 0xC0,
     *     U+0040–U+007F for 0xC1).  RFC 3629 §3 forbids such encodings.
     *
     *   Step 3 – All remaining branches test b0 against 0xE0, 0xE1-0xEC,
     *     0xED, 0xEE-0xEF, 0xF0, 0xF1-0xF3, 0xF4 – none match 0xC0.
     *
     *   Step 4 – Fall-through return 0U: sequence is invalid.
     *     The string-parse loop maps this to OKJ_ERROR_BAD_STRING.
     *
     * This confirms the parser cannot be tricked into treating 0xC0 0x80 as a
     * valid continuation sequence or as a premature string terminator. */

    OkJsonParser parser;
    OkjError     result;

    /* Build {"s":"\xC0\x80"} – use explicit array so the compiler does not
     * interpret 0xC0 0x80 as a multi-byte character constant. */
    char json_str[] = {
        '{', '"', 's', '"', ':', '"',
        (char)0xC0, (char)0x80,   /* Modified UTF-8 overlong NUL (U+0000) */
        '"', '}',
        '\0'                       /* C-string terminator for okj_init */
    };

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);

    printf("test_utf8_overlong_nul_c0_80 passed!\n");
}

void test_backslash_flood_at_limit(void)
{
    /* A string value consisting entirely of escaped backslashes (\\), with
     * exactly OKJ_MAX_STRING_LEN (64) raw bytes of content.  The 64 raw bytes
     * are 32 '\\' pairs, each representing one literal backslash.
     *
     * This is the maximal valid all-backslash string; the parser must accept
     * it and report tok->length == 64.
     *
     * Buffer layout: {"k":"<32 x \\>"}
     *   '{'        =  1
     *   '"k"'      =  3
     *   ':'        =  1
     *   '"'        =  1   opening quote
     *   64 x '\'   = 64   (32 escape pairs)
     *   '"'        =  1   closing quote
     *   '}'        =  1
     *   '\0'       =  1
     *   Total      = 73 bytes
     */

    OkJsonParser  parser;
    OkjError      result;
    OkJsonString *str;
    char          json_str[73];
    uint16_t      i;
    uint16_t      pos = 0U;

    json_str[pos++] = '{';
    json_str[pos++] = '"';
    json_str[pos++] = 'k';
    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '"';

    for (i = 0U; i < 32U; i++)   /* 32 pairs = 64 raw bytes */
    {
        json_str[pos++] = '\\';
        json_str[pos++] = '\\';
    }

    json_str[pos++] = '"';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);

    str = okj_get_string(&parser, "k");
    assert(str != NULL);
    assert(str->length == 64U);

    printf("test_backslash_flood_at_limit passed!\n");
}

void test_backslash_flood_one_over(void)
{
    /* A string value of 33 '\\' pairs = 66 raw bytes, two beyond
     * OKJ_MAX_STRING_LEN.  The length-limit check at the top of the parse
     * loop must catch this and return OKJ_ERROR_MAX_STR_LEN_EXCEEDED.
     *
     * Buffer layout: {"k":"<33 x \\>"}
     *   '{'        =  1
     *   '"k"'      =  3
     *   ':'        =  1
     *   '"'        =  1
     *   66 x '\'   = 66
     *   '"'        =  1
     *   '}'        =  1
     *   '\0'       =  1
     *   Total      = 75 bytes
     */

    OkJsonParser parser;
    OkjError     result;
    char         json_str[75];
    uint16_t     i;
    uint16_t     pos = 0U;

    json_str[pos++] = '{';
    json_str[pos++] = '"';
    json_str[pos++] = 'k';
    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '"';

    for (i = 0U; i < 33U; i++)   /* 33 pairs = 66 raw bytes */
    {
        json_str[pos++] = '\\';
        json_str[pos++] = '\\';
    }

    json_str[pos++] = '"';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_STR_LEN_EXCEEDED);

    printf("test_backslash_flood_one_over passed!\n");
}

void test_backslash_straddle_limit(void)
{
    /* Buffer-exhaustion edge case: a '\\' escape sequence whose FIRST byte
     * lands at relative position 63 (one below the 64-byte ceiling) and
     * whose SECOND byte therefore lands at position 64 (one past it).
     *
     * Without a post-escape length guard the loop's top-of-iteration check
     * never fires for that second byte; the closing '"' is found immediately
     * after and the parser would incorrectly return OKJ_SUCCESS with
     * tok->length == 65.  The correct result is OKJ_ERROR_MAX_STR_LEN_EXCEEDED.
     *
     * Buffer layout: {"k":"<63 x 'a'>\\"}
     *   '{'        =  1
     *   '"k"'      =  3
     *   ':'        =  1
     *   '"'        =  1   opening quote
     *   63 x 'a'   = 63   fills relative positions 0-62
     *   '\\'       =  2   escape at relative positions 63-64  <- straddle
     *   '"'        =  1   closing quote
     *   '}'        =  1
     *   '\0'       =  1
     *   Total      = 74 bytes
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

    for (i = 0U; i < 63U; i++)   /* 63 plain bytes → relative positions 0-62 */
    {
        json_str[pos++] = 'a';
    }

    json_str[pos++] = '\\';      /* relative position 63 — first byte of escape  */
    json_str[pos++] = '\\';      /* relative position 64 — second byte of escape */

    json_str[pos++] = '"';
    json_str[pos++] = '}';
    json_str[pos]   = '\0';

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_STR_LEN_EXCEEDED);

    printf("test_backslash_straddle_limit passed!\n");
}

void test_backslash_truncated_at_boundary(void)
{
    /* A string value of 31 '\\' pairs (62 raw bytes) followed by a lone
     * backslash whose escape character is the NUL terminator.  This places
     * a lone '\' at relative position 62; after the parser consumes it
     * (advancing to relative 63), it reads '\0' as the escape character
     * and must break with OKJ_ERROR_UNEXPECTED_END — not read out of bounds.
     *
     * Buffer layout: {"k":"<31 x \\>\"}
     *   '{'        =  1
     *   '"k"'      =  3
     *   ':'        =  1
     *   '"'        =  1   opening quote
     *   62 x '\'   = 62   (31 escape pairs)
     *   '\'        =  1   lone backslash at relative position 62
     *   '\0'       =  1   NUL — acts as the (missing) escape char
     *   Total      = 70 bytes (no closing '"' or '}')
     */

    OkJsonParser parser;
    OkjError     result;
    char         json_str[70];
    uint16_t     i;
    uint16_t     pos = 0U;

    json_str[pos++] = '{';
    json_str[pos++] = '"';
    json_str[pos++] = 'k';
    json_str[pos++] = '"';
    json_str[pos++] = ':';
    json_str[pos++] = '"';

    for (i = 0U; i < 31U; i++)   /* 31 pairs = 62 raw bytes */
    {
        json_str[pos++] = '\\';
        json_str[pos++] = '\\';
    }

    json_str[pos++] = '\\';      /* lone backslash — escape char is '\0' */
    json_str[pos]   = '\0';      /* NUL terminator */

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_UNEXPECTED_END);

    printf("test_backslash_truncated_at_boundary passed!\n");
}

/* ==================================================================
 * MC/DC tests for okj_validate_utf8_sequence
 *
 * For DO-178C MC/DC compliance every atomic condition inside each
 * compound predicate must independently affect the overall outcome.
 * Each group below targets one compound predicate and provides:
 *   - at least one passing (valid) case, and
 *   - one failing case per atomic condition, holding the other
 *     conditions true so that the killed condition is the sole cause
 *     of the failure.
 * ================================================================== */

/* ------------------------------------------------------------------ *
 *  2-byte sequence  (b0 in [0xC2, 0xDF])                            *
 *  Compound predicate: okj_is_utf8_continuation(b1) != 0U           *
 * ------------------------------------------------------------------ */

void test_utf8_2byte_valid_boundary_min(void)
{
    /* b0=0xC2 (minimum valid 2-byte leader), b1=0x80 (min continuation).
     * Encodes U+0080.  Shows b0 >= 0xC2 is satisfied at its boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xC2\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_2byte_valid_boundary_min passed!\n");
}

void test_utf8_2byte_valid_boundary_max(void)
{
    /* b0=0xDF (maximum valid 2-byte leader), b1=0xBF (max continuation).
     * Encodes U+07FF.  Shows b0 <= 0xDF is satisfied at its boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xDF\xBF\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_2byte_valid_boundary_max passed!\n");
}

void test_utf8_2byte_b1_not_continuation(void)
{
    /* b0=0xC2 (valid leader), b1=0x41 ('A' — not a continuation byte).
     * Independently kills: okj_is_utf8_continuation(b1) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xC2\x41\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_2byte_b1_not_continuation passed!\n");
}

/* ------------------------------------------------------------------ *
 *  3-byte 0xE0 sequence                                              *
 *  Compound predicate:                                               *
 *    (b1 >= 0xA0U) && (b1 <= 0xBFU) &&                             *
 *    (okj_is_utf8_continuation(b2) != 0U)                           *
 * ------------------------------------------------------------------ */

void test_utf8_3byte_e0_valid(void)
{
    /* b0=0xE0, b1=0xA0, b2=0x80 → U+0800.
     * All three conditions in the compound predicate are true. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xE0\xA0\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_3byte_e0_valid passed!\n");
}

void test_utf8_3byte_e0_b1_below_range(void)
{
    /* b0=0xE0, b1=0x9F (one below the 0xA0 lower bound), b2=0x80.
     * Note: 0x9F is a valid continuation byte (10011111), so the only
     * reason this sequence is rejected is b1 < 0xA0 (overlong encoding).
     * Independently kills: b1 >= 0xA0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xE0\x9F\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_3byte_e0_b1_below_range passed!\n");
}

void test_utf8_3byte_e0_b1_above_range(void)
{
    /* b0=0xE0, b1=0xC0 (one above the 0xBF upper bound), b2=0x80.
     * Independently kills: b1 <= 0xBFU. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xE0\xC0\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_3byte_e0_b1_above_range passed!\n");
}

void test_utf8_3byte_e0_b2_not_continuation(void)
{
    /* b0=0xE0, b1=0xA0 (valid), b2=0x41 (not a continuation byte).
     * Independently kills: okj_is_utf8_continuation(b2) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xE0\xA0\x41\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_3byte_e0_b2_not_continuation passed!\n");
}

/* ------------------------------------------------------------------ *
 *  3-byte sequences: b0 in [0xE1,0xEC] or [0xEE,0xEF]              *
 *  Compound predicate:                                               *
 *    (okj_is_utf8_continuation(b1) != 0U) &&                        *
 *    (okj_is_utf8_continuation(b2) != 0U)                           *
 * ------------------------------------------------------------------ */

void test_utf8_3byte_e1_ec_boundary_min(void)
{
    /* b0=0xE1 (minimum of first sub-range), b1=0x80, b2=0x80 → U+1000.
     * Shows b0 >= 0xE1 is satisfied at its boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xE1\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_3byte_e1_ec_boundary_min passed!\n");
}

void test_utf8_3byte_e1_ec_boundary_max(void)
{
    /* b0=0xEC (maximum of first sub-range), b1=0x80, b2=0x80 → U+C000.
     * Shows b0 <= 0xEC is satisfied at its boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xEC\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_3byte_e1_ec_boundary_max passed!\n");
}

void test_utf8_3byte_ee_ef_boundary_min(void)
{
    /* b0=0xEE (minimum of second sub-range), b1=0x80, b2=0x80 → U+E000.
     * Shows b0 >= 0xEE (second sub-range) is satisfied at its boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xEE\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_3byte_ee_ef_boundary_min passed!\n");
}

void test_utf8_3byte_ee_ef_boundary_max(void)
{
    /* b0=0xEF (maximum of second sub-range), b1=0xBF, b2=0xBF → U+FFFF.
     * Shows b0 <= 0xEF is satisfied at its boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xEF\xBF\xBF\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_3byte_ee_ef_boundary_max passed!\n");
}

void test_utf8_3byte_range_b1_not_continuation(void)
{
    /* b0=0xE1 (valid), b1=0x41 (not a continuation byte), b2=0x80.
     * Independently kills: okj_is_utf8_continuation(b1) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xE1\x41\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_3byte_range_b1_not_continuation passed!\n");
}

void test_utf8_3byte_range_b2_not_continuation(void)
{
    /* b0=0xE1 (valid), b1=0x80 (valid continuation), b2=0x41 (not continuation).
     * b1 passes its condition; only b2 fails.
     * Independently kills: okj_is_utf8_continuation(b2) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xE1\x80\x41\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_3byte_range_b2_not_continuation passed!\n");
}

/* ------------------------------------------------------------------ *
 *  3-byte 0xED sequence (surrogate range guard)                      *
 *  Compound predicate:                                               *
 *    (b1 >= 0x80U) && (b1 <= 0x9FU) &&                             *
 *    (okj_is_utf8_continuation(b2) != 0U)                           *
 * ------------------------------------------------------------------ */

void test_utf8_3byte_ed_valid(void)
{
    /* b0=0xED, b1=0x80, b2=0x80 → U+D000 (valid, pre-surrogate).
     * All three conditions are true. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xED\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_3byte_ed_valid passed!\n");
}

void test_utf8_3byte_ed_valid_boundary_max(void)
{
    /* b0=0xED, b1=0x9F, b2=0xBF → U+D7FF (last code point before surrogates).
     * Shows b1 <= 0x9FU is satisfied at its upper boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xED\x9F\xBF\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_3byte_ed_valid_boundary_max passed!\n");
}

void test_utf8_3byte_ed_b1_below_range(void)
{
    /* b0=0xED, b1=0x7F (one below the 0x80 lower bound), b2=0x80.
     * Independently kills: b1 >= 0x80U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xED\x7F\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_3byte_ed_b1_below_range passed!\n");
}

void test_utf8_3byte_ed_b2_not_continuation(void)
{
    /* b0=0xED, b1=0x80 (valid, range satisfied), b2=0x41 (not continuation).
     * Independently kills: okj_is_utf8_continuation(b2) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xED\x80\x41\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_3byte_ed_b2_not_continuation passed!\n");
}

/* ------------------------------------------------------------------ *
 *  4-byte 0xF0 sequence                                              *
 *  Compound predicate:                                               *
 *    (b1 >= 0x90U) && (b1 <= 0xBFU) &&                             *
 *    (okj_is_utf8_continuation(b2) != 0U) &&                        *
 *    (okj_is_utf8_continuation(b3) != 0U)                           *
 * ------------------------------------------------------------------ */

void test_utf8_4byte_f0_b1_below_range(void)
{
    /* b0=0xF0, b1=0x8F (one below 0x90 lower bound), b2=0x80, b3=0x80.
     * Encodes an overlong 4-byte sequence.
     * Independently kills: b1 >= 0x90U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF0\x8F\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f0_b1_below_range passed!\n");
}

void test_utf8_4byte_f0_b1_above_range(void)
{
    /* b0=0xF0, b1=0xC0 (one above 0xBF upper bound), b2=0x80, b3=0x80.
     * Independently kills: b1 <= 0xBFU. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF0\xC0\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f0_b1_above_range passed!\n");
}

void test_utf8_4byte_f0_b2_not_continuation(void)
{
    /* b0=0xF0, b1=0x90 (valid), b2=0x41 (not continuation), b3=0x80.
     * Independently kills: okj_is_utf8_continuation(b2) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF0\x90\x41\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f0_b2_not_continuation passed!\n");
}

void test_utf8_4byte_f0_b3_not_continuation(void)
{
    /* b0=0xF0, b1=0x90 (valid), b2=0x80 (valid), b3=0x41 (not continuation).
     * b1 and b2 both pass; only b3 fails.
     * Independently kills: okj_is_utf8_continuation(b3) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF0\x90\x80\x41\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f0_b3_not_continuation passed!\n");
}

/* ------------------------------------------------------------------ *
 *  4-byte sequences: b0 in [0xF1, 0xF3]                             *
 *  Compound predicate:                                               *
 *    (okj_is_utf8_continuation(b1) != 0U) &&                        *
 *    (okj_is_utf8_continuation(b2) != 0U) &&                        *
 *    (okj_is_utf8_continuation(b3) != 0U)                           *
 * ------------------------------------------------------------------ */

void test_utf8_4byte_f1_f3_valid(void)
{
    /* b0=0xF1, b1=0x80, b2=0x80, b3=0x80 → U+40000.
     * All three continuation conditions are true. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF1\x80\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_4byte_f1_f3_valid passed!\n");
}

void test_utf8_4byte_f1_f3_boundary_max(void)
{
    /* b0=0xF3 (maximum of range), b1=0xBF, b2=0xBF, b3=0xBF → U+FFFFF.
     * Shows b0 <= 0xF3U is satisfied at its boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF3\xBF\xBF\xBF\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_4byte_f1_f3_boundary_max passed!\n");
}

void test_utf8_4byte_f1_f3_b1_not_continuation(void)
{
    /* b0=0xF1 (valid), b1=0x41 (not continuation), b2=0x80, b3=0x80.
     * Independently kills: okj_is_utf8_continuation(b1) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF1\x41\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f1_f3_b1_not_continuation passed!\n");
}

void test_utf8_4byte_f1_f3_b2_not_continuation(void)
{
    /* b0=0xF1 (valid), b1=0x80 (valid), b2=0x41 (not continuation), b3=0x80.
     * b1 passes; only b2 fails.
     * Independently kills: okj_is_utf8_continuation(b2) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF1\x80\x41\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f1_f3_b2_not_continuation passed!\n");
}

void test_utf8_4byte_f1_f3_b3_not_continuation(void)
{
    /* b0=0xF1 (valid), b1=0x80 (valid), b2=0x80 (valid), b3=0x41 (not continuation).
     * b1 and b2 both pass; only b3 fails.
     * Independently kills: okj_is_utf8_continuation(b3) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF1\x80\x80\x41\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f1_f3_b3_not_continuation passed!\n");
}

/* ------------------------------------------------------------------ *
 *  4-byte 0xF4 sequence                                              *
 *  Compound predicate:                                               *
 *    (b1 >= 0x80U) && (b1 <= 0x8FU) &&                             *
 *    (okj_is_utf8_continuation(b2) != 0U) &&                        *
 *    (okj_is_utf8_continuation(b3) != 0U)                           *
 * ------------------------------------------------------------------ */

void test_utf8_4byte_f4_valid(void)
{
    /* b0=0xF4, b1=0x80, b2=0x80, b3=0x80 → U+100000.
     * All four conditions in the compound predicate are true. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF4\x80\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_4byte_f4_valid passed!\n");
}

void test_utf8_4byte_f4_valid_boundary_max(void)
{
    /* b0=0xF4, b1=0x8F, b2=0xBF, b3=0xBF → U+10FFFF (maximum Unicode scalar).
     * Shows b1 <= 0x8FU is satisfied at its upper boundary. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF4\x8F\xBF\xBF\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    printf("test_utf8_4byte_f4_valid_boundary_max passed!\n");
}

void test_utf8_4byte_f4_b1_below_range(void)
{
    /* b0=0xF4, b1=0x7F (one below the 0x80 lower bound), b2=0x80, b3=0x80.
     * Independently kills: b1 >= 0x80U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF4\x7F\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f4_b1_below_range passed!\n");
}

void test_utf8_4byte_f4_b1_above_range(void)
{
    /* b0=0xF4, b1=0x90 (one above 0x8F upper bound), b2=0x80, b3=0x80.
     * Encodes above U+10FFFF.
     * Independently kills: b1 <= 0x8FU. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF4\x90\x80\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f4_b1_above_range passed!\n");
}

void test_utf8_4byte_f4_b2_not_continuation(void)
{
    /* b0=0xF4, b1=0x80 (valid), b2=0x41 (not continuation), b3=0x80.
     * Independently kills: okj_is_utf8_continuation(b2) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF4\x80\x41\x80\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f4_b2_not_continuation passed!\n");
}

void test_utf8_4byte_f4_b3_not_continuation(void)
{
    /* b0=0xF4, b1=0x80 (valid), b2=0x80 (valid), b3=0x41 (not continuation).
     * b1 and b2 both pass; only b3 fails.
     * Independently kills: okj_is_utf8_continuation(b3) != 0U. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"s\":\"\xF4\x80\x80\x41\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);
    printf("test_utf8_4byte_f4_b3_not_continuation passed!\n");
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

void test_object_exactly_32_members(void)
{
    /* Build {"data": {"k0":1,"k1":1,...,"k31":1}} — exactly 32 members in
     * the nested object, matching OKJ_MAX_OBJECT_SIZE (32).  Parsing must
     * succeed and okj_get_object() must return a non-NULL pointer whose
     * count equals 32. */

    OkJsonParser  parser;
    OkjError      result;
    OkJsonObject *obj;

    /* Worst-case size: 9 + 244 + 2 + 1 = 256 bytes */
    char     json_str[256];
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

    /* 32 members: "k0":1 … "k31":1 */
    for (i = 0U; i < 32U; i++)
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

    assert(result == OKJ_SUCCESS);         /* 67 tokens — well within limit  */

    obj = okj_get_object(&parser, "data");

    assert(obj != NULL);                   /* exactly 32 == OKJ_MAX_OBJECT_SIZE */
    assert(obj->count == 32U);

    printf("test_object_exactly_32_members passed!\n");
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
     * and verify the depth-stack ceiling (OKJ_MAX_DEPTH) is enforced.
     *
     * Token count for N levels = 2*N + 1 (N objects + N string keys + 1 number).
     * At N = OKJ_MAX_DEPTH   (16): uses all 16 slots exactly -> OKJ_SUCCESS
     * At N = OKJ_MAX_DEPTH+1 (17): opening the 17th '{' tries to push beyond
     *                               the last slot -> OKJ_ERROR_MAX_DEPTH_EXCEEDED */

    OkJsonParser parser;
    OkjError     result;
    uint16_t     i;
    uint16_t     pos;

    /* N=16: 16*5 + 1 + 16 = 97 chars + NUL = 98 bytes */
    char json16[98];
    pos = 0U;

    for (i = 0U; i < 16U; i++)
    {
        json16[pos++] = '{';
        json16[pos++] = '"';
        json16[pos++] = 'a';
        json16[pos++] = '"';
        json16[pos++] = ':';
    }

    json16[pos++] = '1';

    for (i = 0U; i < 16U; i++)
    {
        json16[pos++] = '}';
    }

    json16[pos] = '\0';

    okj_init(&parser, json16);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 33U);  /* 2*16 + 1 */
    assert(parser.depth == 0U);         /* all containers closed */

    /* N=17: 17*5 + 1 + 17 = 103 chars + NUL = 104 bytes */
    char json17[104];
    pos = 0U;

    for (i = 0U; i < 17U; i++)
    {
        json17[pos++] = '{';
        json17[pos++] = '"';
        json17[pos++] = 'a';
        json17[pos++] = '"';
        json17[pos++] = ':';
    }

    json17[pos++] = '1';

    for (i = 0U; i < 17U; i++)
    {
        json17[pos++] = '}';
    }

    json17[pos] = '\0';

    okj_init(&parser, json17);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_DEPTH_EXCEEDED);

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

void test_max_json_len_boundary(void)
{
    /* Prove the exact boundary of OKJ_MAX_JSON_LEN (4096).
     *
     * Sub-test 1: a string of exactly 4096 bytes (the limit) must be accepted
     *             and return OKJ_SUCCESS.
     * Sub-test 2: a string of exactly 4097 bytes (one over the limit) must be
     *             rejected with OKJ_ERROR_MAX_JSON_LEN_EXCEEDED.
     *
     * Both strings use the layout: {"x":1} (7 bytes) + N trailing spaces.
     * Trailing whitespace after a complete top-level object is valid JSON. */

    OkJsonParser parser;
    OkjError     result;
    char         json_buf[4098]; /* large enough for both sub-tests */
    uint16_t     i;
    uint16_t     pos;

    /* --- Sub-test 1: exactly OKJ_MAX_JSON_LEN (4096) bytes must succeed ---
     * {"x":1} (7 bytes) + 4089 spaces = 4096 bytes, then '\0'. */
    pos = 0U;
    json_buf[pos++] = '{';
    json_buf[pos++] = '"';
    json_buf[pos++] = 'x';
    json_buf[pos++] = '"';
    json_buf[pos++] = ':';
    json_buf[pos++] = '1';
    json_buf[pos++] = '}';

    for (i = 0U; i < 4089U; i++)
    {
        json_buf[pos++] = ' ';
    }

    json_buf[pos] = '\0'; /* pos == 4096 == OKJ_MAX_JSON_LEN */

    okj_init(&parser, json_buf);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);

    /* --- Sub-test 2: exactly OKJ_MAX_JSON_LEN + 1 (4097) bytes must fail ---
     * {"x":1} (7 bytes) + 4090 spaces = 4097 bytes, then '\0'. */
    pos = 0U;
    json_buf[pos++] = '{';
    json_buf[pos++] = '"';
    json_buf[pos++] = 'x';
    json_buf[pos++] = '"';
    json_buf[pos++] = ':';
    json_buf[pos++] = '1';
    json_buf[pos++] = '}';

    for (i = 0U; i < 4090U; i++)
    {
        json_buf[pos++] = ' ';
    }

    json_buf[pos] = '\0'; /* pos == 4097 == OKJ_MAX_JSON_LEN + 1 */

    okj_init(&parser, json_buf);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_MAX_JSON_LEN_EXCEEDED);

    printf("test_max_json_len_boundary passed!\n");
}

void test_parse_null_parser(void)
{
    /* Passing NULL as the parser pointer must return OKJ_ERROR_BAD_POINTER
     * without accessing any memory. */

    OkjError result = okj_parse(NULL);

    assert(result == OKJ_ERROR_BAD_POINTER);

    printf("test_parse_null_parser passed!\n");
}

void test_null_pointer_safety(void)
{
    /* Exhaustively verify every public API boundary that accepts a pointer
     * handles NULL gracefully (no segfault, no undefined behaviour).
     * This satisfies the MISRA C defensive-programming requirement that
     * API entry points validate all pointer arguments before use. */

    OkJsonParser parser;
    char         json_str[] = "{\"k\":1}";

    /* --- okj_init(NULL, json) -------------------------------------------- */
    /* Must return immediately without touching any memory. */
    okj_init(NULL, json_str);

    /* --- okj_init(parser, NULL) ------------------------------------------ */
    /* Prime the parser with known values, then attempt a re-init with a NULL
     * JSON string.  The call must be a no-op: the parser fields must remain
     * exactly as set by the prior okj_init() call. */
    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    {
        uint16_t saved_token_count = parser.token_count;
        uint16_t saved_depth       = parser.depth;

        okj_init(&parser, NULL);

        assert(parser.token_count == saved_token_count);
        assert(parser.depth       == saved_depth);
    }

    /* --- okj_count_objects(NULL) ----------------------------------------- */
    assert(okj_count_objects(NULL)  == 0U);

    /* --- okj_count_arrays(NULL) ------------------------------------------ */
    assert(okj_count_arrays(NULL)   == 0U);

    /* --- okj_count_elements(NULL) ---------------------------------------- */
    assert(okj_count_elements(NULL) == 0U);

    printf("test_null_pointer_safety passed!\n");
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

void test_copy_string_one_byte_buf(void)
{
    /* When buf_size == 1, copy_len collapses to 0 and buf[0] must be '\0'.
     * This proves the API never overflows the smallest possible destination. */

    OkJsonParser  parser;
    OkJsonString *str;
    char          buf[1];           /* only room for the null terminator */
    uint16_t      copied;

    char json_str[] = "{\"x\": \"Hello\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    str = okj_get_string(&parser, "x");
    assert(str != NULL);
    assert(str->length == 5U);

    /* Sentinel: fill with a non-zero value so we can confirm it was overwritten. */
    buf[0] = 'Z';

    copied = okj_copy_string(str, buf, (uint16_t)sizeof(buf));

    assert(copied == 0U);           /* no data bytes fit; copy_len = buf_size-1 = 0 */
    assert(buf[0] == '\0');         /* null terminator must occupy the sole byte */

    printf("test_copy_string_one_byte_buf passed!\n");
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

void test_depth_stack_bracket_mismatch_obj(void)
{
    /* An object opened with '{' but closed with ']' must be rejected with
     * OKJ_ERROR_BRACKET_MISMATCH rather than silently consuming the ']'. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"k\": 1]";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BRACKET_MISMATCH);

    printf("test_depth_stack_bracket_mismatch_obj passed!\n");
}

void test_depth_stack_bracket_mismatch_arr(void)
{
    /* An array opened with '[' but closed with '}' must be rejected with
     * OKJ_ERROR_BRACKET_MISMATCH. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "[1, 2}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BRACKET_MISMATCH);

    printf("test_depth_stack_bracket_mismatch_arr passed!\n");
}

void test_depth_stack_extra_close_brace(void)
{
    /* A lone '}' with no matching '{' must be rejected.
     * The context check fires before the depth check (a closing bracket is
     * not valid where a value is expected), so OKJ_ERROR_SYNTAX is returned. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_depth_stack_extra_close_brace passed!\n");
}

void test_depth_stack_extra_close_bracket(void)
{
    /* A lone ']' with no matching '[' must be rejected.
     * Like the extra brace case, the context check fires first and returns
     * OKJ_ERROR_SYNTAX rather than OKJ_ERROR_BRACKET_MISMATCH. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "]";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_depth_stack_extra_close_bracket passed!\n");
}

void test_depth_stack_unclosed_object(void)
{
    /* An object whose closing '}' is absent must be rejected.
     * The depth check at end-of-input produces OKJ_ERROR_UNEXPECTED_END. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"key\": 42";   /* missing closing '}' */

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_UNEXPECTED_END);

    printf("test_depth_stack_unclosed_object passed!\n");
}

void test_depth_stack_unclosed_array(void)
{
    /* An array whose closing ']' is absent must be rejected.
     * The depth check at end-of-input produces OKJ_ERROR_UNEXPECTED_END. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "[1, 2, 3";   /* missing closing ']' */

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_UNEXPECTED_END);

    printf("test_depth_stack_unclosed_array passed!\n");
}

void test_depth_stack_mixed_nesting(void)
{
    /* Verify that the stack correctly tracks alternating object/array
     * container types: {"arr": [{"x": 1}]} */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"arr\": [{\"x\": 1}]}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.depth == 0U);   /* all containers closed */

    printf("test_depth_stack_mixed_nesting passed!\n");
}

void test_depth_stack_alternating_unwind(void)
{
    /* Verify that the depth_stack correctly tracks every independent container
     * type through a full 15-level alternating object/array nesting and then
     * perfectly unwinds on the way back out without triggering
     * OKJ_ERROR_BRACKET_MISMATCH at any level.
     *
     * Structure (depth_stack slot → container type):
     *   [0]=OBJ  [1]=ARR  [2]=OBJ  [3]=ARR  [4]=OBJ  [5]=ARR  [6]=OBJ
     *   [7]=ARR  [8]=OBJ  [9]=ARR  [10]=OBJ [11]=ARR [12]=OBJ [13]=ARR
     *   [14]=OBJ
     *
     * JSON:
     *   {"a":[{"b":[{"c":[{"d":[{"e":[{"f":[{"g":[{"h":1}]}]}]}]}]}]}]}
     *
     * Each closing bracket is validated against depth_stack[parser->depth]
     * after the pre-decrement, exercising all 15 stack slots:
     *   }  checks depth_stack[14]==OBJ  ✓
     *   ]  checks depth_stack[13]==ARR  ✓
     *   ...continuing up...
     *   }  checks depth_stack[0] ==OBJ  ✓
     *
     * Token budget:
     *   8 OKJ_OBJECT + 7 OKJ_ARRAY + 8 OKJ_STRING (keys a–h) + 1 OKJ_NUMBER = 24
     */

    OkJsonParser parser;
    OkjError     result;

    char json_str[] =
        "{\"a\":[{\"b\":[{\"c\":[{\"d\":[{\"e\":[{\"f\":[{\"g\":[{\"h\":1}]}]}]}]}]}]}]}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    /* The parser must accept the payload without any bracket mismatch. */
    assert(result == OKJ_SUCCESS);

    /* All 15 containers must be fully closed. */
    assert(parser.depth == 0U);

    /* Exact token count: 8 objects + 7 arrays + 8 string keys + 1 number. */
    assert(parser.token_count == 24U);

    /* Container type counts confirm the alternating structure was tracked. */
    assert(okj_count_objects(&parser) == 8U);
    assert(okj_count_arrays(&parser)  == 7U);

    printf("test_depth_stack_alternating_unwind passed!\n");
}

void test_trailing_whitespace_after_object(void)
{
    /* Trailing whitespace after a valid top-level object must be accepted. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"x\": 1}   ";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);

    printf("test_trailing_whitespace_after_object passed!\n");
}

void test_trailing_garbage_after_object(void)
{
    /* Non-whitespace content after a complete top-level object must be
     * rejected with OKJ_ERROR_SYNTAX. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"x\": 1} garbage";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_trailing_garbage_after_object passed!\n");
}

void test_trailing_garbage_after_array(void)
{
    /* Non-whitespace content after a complete top-level array must be
     * rejected with OKJ_ERROR_SYNTAX. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "[1, 2, 3] x";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_trailing_garbage_after_array passed!\n");
}

void test_trailing_garbage_after_primitive(void)
{
    /* Non-whitespace content after a top-level primitive value must be
     * rejected with OKJ_ERROR_SYNTAX. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "42 extra";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_trailing_garbage_after_primitive passed!\n");
}

void test_two_top_level_primitives(void)
{
    /* Two consecutive top-level primitive values must be rejected because
     * a JSON text contains exactly one top-level value (RFC 8259 §2). */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "true false";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_two_top_level_primitives passed!\n");
}

void test_two_top_level_objects(void)
{
    /* Two consecutive top-level objects must be rejected. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{} {}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_two_top_level_objects passed!\n");
}

/* ---------------------------------------------------------------------------
 * RFC 8259 grammar compliance tests
 * These cover structural rules that were not previously enforced.
 * ---------------------------------------------------------------------------*/

void test_trailing_comma_in_object(void)
{
    /* RFC 8259 §4 does not permit trailing commas in objects.
     * {"a": 1,} must be rejected. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"a\": 1,}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_trailing_comma_in_object passed!\n");
}

void test_trailing_comma_in_array(void)
{
    /* RFC 8259 §5 does not permit trailing commas in arrays.
     * [1, 2,] must be rejected. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "[1, 2,]";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_trailing_comma_in_array passed!\n");
}

void test_number_as_object_key(void)
{
    /* RFC 8259 §4 requires object keys to be strings.
     * A number used as a key must be rejected. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{42: \"value\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_number_as_object_key passed!\n");
}

void test_boolean_as_object_key(void)
{
    /* RFC 8259 §4: object keys must be strings; 'true' as a key is invalid. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{true: \"value\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_boolean_as_object_key passed!\n");
}

void test_null_as_object_key(void)
{
    /* RFC 8259 §4: object keys must be strings; 'null' as a key is invalid. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{null: \"value\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_null_as_object_key passed!\n");
}

void test_missing_colon(void)
{
    /* An object whose key is not followed by ':' must be rejected.
     * {"a" "b"} is invalid — the colon is mandatory. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"a\" \"b\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_missing_colon passed!\n");
}

void test_colon_without_key(void)
{
    /* A colon that appears before any key is not valid. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{: \"value\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_colon_without_key passed!\n");
}

void test_double_colon(void)
{
    /* Two consecutive colons are not valid JSON structure. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"a\":: 1}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_double_colon passed!\n");
}

void test_empty_input(void)
{
    /* An empty string contains no JSON value and must be rejected.
     * RFC 8259 §2 requires exactly one top-level value. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_UNEXPECTED_END);

    printf("test_empty_input passed!\n");
}

void test_whitespace_only_input(void)
{
    /* A string containing only whitespace has no JSON value and must be
     * rejected. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "   \t\n\r  ";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_UNEXPECTED_END);

    printf("test_whitespace_only_input passed!\n");
}

void test_keyword_no_boundary(void)
{
    /* A keyword literal ("true", "false", "null") must be followed by a
     * value terminator.  "truetrue" is not a valid JSON value. */

    OkJsonParser parser;
    OkjError     result;
    char json1[] = "{\"x\": truetrue}";
    char json2[] = "{\"x\": falsefalse}";
    char json3[] = "{\"x\": nullnull}";

    okj_init(&parser, json1);
    result = okj_parse(&parser);
    assert(result == OKJ_ERROR_SYNTAX);

    okj_init(&parser, json2);
    result = okj_parse(&parser);
    assert(result == OKJ_ERROR_SYNTAX);

    okj_init(&parser, json3);
    result = okj_parse(&parser);
    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_keyword_no_boundary passed!\n");
}

void test_value_after_value_in_object(void)
{
    /* Two values in a row inside an object with no separating comma or colon
     * must be rejected. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{\"a\": 1 2}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_value_after_value_in_object passed!\n");
}

void test_comma_at_start_of_array(void)
{
    /* A leading comma in an array is not valid JSON. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "[,1]";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_comma_at_start_of_array passed!\n");
}

void test_comma_at_start_of_object(void)
{
    /* A leading comma in an object is not valid JSON. */

    OkJsonParser parser;
    OkjError     result;
    char json_str[] = "{,\"a\": 1}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_SYNTAX);

    printf("test_comma_at_start_of_object passed!\n");
}

void test_top_level_number(void)
{
    /* RFC 8259 §2: any JSON value may be the top-level text.
     * A bare number must parse successfully. */

    OkJsonParser  parser;
    OkjError      result;
    char json_str[] = "42";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 1U);
    assert(parser.tokens[0].type == OKJ_NUMBER);

    printf("test_top_level_number passed!\n");
}

void test_top_level_string(void)
{
    /* A bare string must parse successfully as a top-level JSON value. */

    OkJsonParser  parser;
    OkjError      result;
    char json_str[] = "\"hello\"";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 1U);
    assert(parser.tokens[0].type == OKJ_STRING);

    printf("test_top_level_string passed!\n");
}

void test_top_level_boolean(void)
{
    /* A bare boolean literal must parse successfully as a top-level value. */

    OkJsonParser  parser;
    OkjError      result;
    char json1[] = "true";
    char json2[] = "false";

    okj_init(&parser, json1);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 1U);
    assert(parser.tokens[0].type == OKJ_BOOLEAN);

    okj_init(&parser, json2);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 1U);
    assert(parser.tokens[0].type == OKJ_BOOLEAN);

    printf("test_top_level_boolean passed!\n");
}

void test_top_level_null(void)
{
    /* A bare null literal must parse successfully as a top-level JSON value. */

    OkJsonParser  parser;
    OkjError      result;
    char json_str[] = "null";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS);
    assert(parser.token_count == 1U);
    assert(parser.tokens[0].type == OKJ_NULL);

    printf("test_top_level_null passed!\n");
}

void test_iot_sensor_json(void)
{
    /* Parse a realistic IoT sensor payload that exercises all major JSON
     * value types at once:
     *   - Top-level string fields  (deviceId, deviceModel)
     *   - Boolean values           (isActive: true, requiresMaintenance: false)
     *   - Null value               (assignedLocation)
     *   - Integer and float numbers (batteryLevel, uptimeSeconds, signalStrength)
     *   - Nested object            (networkFeatures, calibrationData)
     *   - Array of numbers         (recentTemperatures, 4 elements)
     *   - Array of strings         (systemTags, 3 elements)
     *   - Scientific-notation number (scaleFactor: 1.002e1)
     *   - Negative float           (baseOffset: -0.45)
     *   - Empty array              (pendingErrorCodes: [])
     *
     * Expected token count: 48
     *   1  root OKJ_OBJECT
     *   16 top-level primitive key-value pairs  (8 pairs × 2)
     *   10 networkFeatures key + OKJ_OBJECT + 4 inner key-value pairs (2+8)
     *    6 recentTemperatures key + OKJ_ARRAY + 4 NUMBER elements (2+4)
     *    5 systemTags key + OKJ_ARRAY + 3 STRING elements (2+3)
     *   10 calibrationData key + OKJ_OBJECT + 4 inner key-value pairs (2+8)
     */

    OkJsonParser  parser;
    OkJsonString *str;
    OkJsonNumber *num;
    OkJsonBoolean *bval;
    OkJsonToken  *tok;
    OkJsonObject *obj;
    OkJsonArray  *arr;
    char          buf[32];

    char json_str[] =
        "{"
        "\"deviceId\": \"sens-a83-992b\","
        "\"deviceModel\": \"TempSense-X\","
        "\"isActive\": true,"
        "\"requiresMaintenance\": false,"
        "\"assignedLocation\": null,"
        "\"batteryLevel\": 87.5,"
        "\"uptimeSeconds\": 145920,"
        "\"signalStrength\": -65,"
        "\"networkFeatures\": {"
            "\"wifiEnabled\": true,"
            "\"bluetoothEnabled\": false,"
            "\"ipv4Address\": \"192.168.4.105\","
            "\"macAddress\": \"00:1B:44:11:3A:B7\""
        "},"
        "\"recentTemperatures\": [22.4, 22.5, 22.1, 21.9],"
        "\"systemTags\": [\"hvac-monitor\", \"zone-1\", \"primary\"],"
        "\"calibrationData\": {"
            "\"baseOffset\": -0.45,"
            "\"scaleFactor\": 1.002e1,"
            "\"lastRunTimestamp\": \"2026-03-10T08:15:30Z\","
            "\"pendingErrorCodes\": []"
        "}"
        "}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* Token budget */
    assert(parser.token_count == 48U);
    assert(parser.tokens[0].type == OKJ_OBJECT);

    /* Object and array counts (root + networkFeatures + calibrationData = 3
     * objects; recentTemperatures + systemTags + pendingErrorCodes = 3 arrays) */
    assert(okj_count_objects(&parser) == 3U);
    assert(okj_count_arrays(&parser)  == 3U);

    /* --- top-level string fields --- */
    str = okj_get_string(&parser, "deviceId");
    assert(str != NULL);
    assert(str->length == 13U);   /* "sens-a83-992b" */
    assert(str->start[0] == 's');

    /* copy_string round-trip on deviceId */
    assert(okj_copy_string(str, buf, (uint16_t)sizeof(buf)) == 13U);
    assert(buf[0] == 's');
    assert(buf[13] == '\0');

    str = okj_get_string(&parser, "deviceModel");
    assert(str != NULL);
    assert(str->length == 11U);   /* "TempSense-X" */
    assert(str->start[0] == 'T');

    /* --- boolean fields --- */
    bval = okj_get_boolean(&parser, "isActive");
    assert(bval != NULL);
    assert(bval->start[0] == 't');   /* true */

    bval = okj_get_boolean(&parser, "requiresMaintenance");
    assert(bval != NULL);
    assert(bval->start[0] == 'f');   /* false */

    /* --- null field --- */
    tok = okj_get_token(&parser, "assignedLocation");
    assert(tok != NULL);
    assert(tok->type   == OKJ_NULL);
    assert(tok->length == 4U);   /* 'n','u','l','l' */

    /* --- numeric fields --- */
    num = okj_get_number(&parser, "batteryLevel");
    assert(num != NULL);
    assert(num->length == 4U);   /* "87.5" */

    num = okj_get_number(&parser, "uptimeSeconds");
    assert(num != NULL);
    assert(num->length == 6U);   /* "145920" */

    num = okj_get_number(&parser, "signalStrength");
    assert(num != NULL);
    assert(num->start[0] == '-');
    assert(num->length == 3U);   /* "-65" */

    /* --- nested object: networkFeatures --- */
    obj = okj_get_object(&parser, "networkFeatures");
    assert(obj != NULL);
    assert(obj->count == 4U);   /* wifiEnabled, bluetoothEnabled, ipv4Address, macAddress */

    str = okj_get_string(&parser, "ipv4Address");
    assert(str != NULL);
    assert(str->length == 13U);   /* "192.168.4.105" */

    str = okj_get_string(&parser, "macAddress");
    assert(str != NULL);
    assert(str->length == 17U);   /* "00:1B:44:11:3A:B7" */

    /* --- array of numbers: recentTemperatures --- */
    arr = okj_get_array(&parser, "recentTemperatures");
    assert(arr != NULL);
    assert(arr->count == 4U);

    /* --- array of strings: systemTags --- */
    arr = okj_get_array(&parser, "systemTags");
    assert(arr != NULL);
    assert(arr->count == 3U);

    /* --- nested object: calibrationData --- */
    obj = okj_get_object(&parser, "calibrationData");
    assert(obj != NULL);
    assert(obj->count == 4U);   /* baseOffset, scaleFactor, lastRunTimestamp, pendingErrorCodes */

    num = okj_get_number(&parser, "baseOffset");
    assert(num != NULL);
    assert(num->start[0] == '-');
    assert(num->length == 5U);   /* "-0.45" */

    num = okj_get_number(&parser, "scaleFactor");
    assert(num != NULL);
    assert(num->length == 7U);   /* "1.002e1" */

    str = okj_get_string(&parser, "lastRunTimestamp");
    assert(str != NULL);
    assert(str->length == 20U);   /* "2026-03-10T08:15:30Z" */

    /* --- empty array: pendingErrorCodes --- */
    arr = okj_get_array(&parser, "pendingErrorCodes");
    assert(arr != NULL);
    assert(arr->count == 0U);

    printf("test_iot_sensor_json passed!\n");
}

void test_user_data_json(void)
{
    /* Parse a realistic user-data payload (array of two user objects) that
     * exercises all major JSON value types at once:
     *   - Top-level array containing two objects
     *   - String fields  (_id, guid, balance, name, company, email, about)
     *   - Boolean values (isActive: false for both users)
     *   - Integer and float numbers (index, age, latitude, longitude)
     *   - Nested array of strings (tags, 3 elements per user)
     *   - Nested array of objects (friends, 1 element per user)
     *     each friend object has integer id and string name
     *
     * Token budget: 75
     *   1  root OKJ_ARRAY
     *   37 first user object  (1 OKJ_OBJECT + 12 key-value pairs×2
     *                          + tags key+array+3 strings
     *                          + friends key+array+object+2 key-value pairs)
     *   37 second user object (same structure)
     *
     * Object count: 4  (user1, user2, user1.friends[0], user2.friends[0])
     * Array count:  5  (root, user1.tags, user1.friends,
     *                         user2.tags, user2.friends)
     */

    OkJsonParser   parser;
    OkJsonString  *str;
    OkJsonNumber  *num;
    OkJsonBoolean *bval;
    OkJsonArray   *arr;
    char           buf[64];

    char json_str[] =
        "["
        "{"
        "\"_id\":\"69b18062f9d860aa282072e7\","
        "\"index\":0,"
        "\"guid\":\"695ef86b-6392-448a-a346-68e174770d11\","
        "\"isActive\":false,"
        "\"balance\":\"$2,992.34\","
        "\"age\":33,"
        "\"name\":\"Hamilton Mcdowell\","
        "\"company\":\"EXPOSA\","
        "\"email\":\"hamilton@exposa.com\","
        "\"about\":\"Consectetur adipisicing ipsum fugiat aute aliqua.\","
        "\"latitude\":-63.370226,"
        "\"longitude\":-97.934255,"
        "\"tags\":[\"ipsum\",\"officia\",\"enim\"],"
        "\"friends\":[{\"id\":0,\"name\":\"Angelina Mercado\"}]"
        "},"
        "{"
        "\"_id\":\"69b18062cce81d0eac3e3e8a\","
        "\"index\":1,"
        "\"guid\":\"0b1c834d-334f-4eba-9cb2-d74dc3643704\","
        "\"isActive\":false,"
        "\"balance\":\"$1,712.77\","
        "\"age\":40,"
        "\"name\":\"Simone Fuentes\","
        "\"company\":\"QUAILCOM\","
        "\"email\":\"simonefuentes@quailcom.com\","
        "\"about\":\"Sunt eu ipsum pariatur occaecat ullamco minim.\","
        "\"latitude\":18.085851,"
        "\"longitude\":177.469499,"
        "\"tags\":[\"laborum\",\"laboris\",\"consequat\"],"
        "\"friends\":[{\"id\":0,\"name\":\"Elva Whitfield\"}]"
        "}"
        "]";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* Token budget */
    assert(parser.token_count == 75U);
    assert(parser.tokens[0].type == OKJ_ARRAY);

    /* Structural counts */
    assert(okj_count_objects(&parser) == 4U);
    assert(okj_count_arrays(&parser)  == 5U);

    /* --- first user: string fields --- */
    str = okj_get_string(&parser, "_id");
    assert(str != NULL);
    assert(str->length == 24U);   /* "69b18062f9d860aa282072e7" */
    assert(str->start[0] == '6');

    str = okj_get_string(&parser, "guid");
    assert(str != NULL);
    assert(str->length == 36U);   /* "695ef86b-6392-448a-a346-68e174770d11" */

    str = okj_get_string(&parser, "balance");
    assert(str != NULL);
    assert(str->length == 9U);    /* "$2,992.34" */
    assert(str->start[0] == '$');

    str = okj_get_string(&parser, "name");
    assert(str != NULL);
    assert(str->length == 17U);   /* "Hamilton Mcdowell" */
    assert(str->start[0] == 'H');

    /* copy_string round-trip on name */
    assert(okj_copy_string(str, buf, (uint16_t)sizeof(buf)) == 17U);
    assert(buf[0]  == 'H');
    assert(buf[17] == '\0');

    str = okj_get_string(&parser, "company");
    assert(str != NULL);
    assert(str->length == 6U);    /* "EXPOSA" */

    str = okj_get_string(&parser, "email");
    assert(str != NULL);
    assert(str->length == 19U);   /* "hamilton@exposa.com" */

    str = okj_get_string(&parser, "about");
    assert(str != NULL);
    assert(str->length == 49U);   /* "Consectetur adipisicing ipsum fugiat aute aliqua." */

    /* --- first user: boolean field --- */
    bval = okj_get_boolean(&parser, "isActive");
    assert(bval != NULL);
    assert(bval->start[0] == 'f');   /* false */
    assert(bval->length   == 5U);

    /* --- first user: numeric fields --- */
    num = okj_get_number(&parser, "index");
    assert(num != NULL);
    assert(num->length == 1U);    /* "0" */

    num = okj_get_number(&parser, "age");
    assert(num != NULL);
    assert(num->length == 2U);    /* "33" */

    num = okj_get_number(&parser, "latitude");
    assert(num != NULL);
    assert(num->start[0] == '-');
    assert(num->length == 10U);   /* "-63.370226" */

    num = okj_get_number(&parser, "longitude");
    assert(num != NULL);
    assert(num->start[0] == '-');
    assert(num->length == 10U);   /* "-97.934255" */

    /* --- first user: tags array --- */
    arr = okj_get_array(&parser, "tags");
    assert(arr != NULL);
    assert(arr->count == 3U);

    /* --- first user: friends array --- */
    arr = okj_get_array(&parser, "friends");
    assert(arr != NULL);
    assert(arr->count == 1U);

    printf("test_user_data_json passed!\n");
}

void test_deeply_nested_valid_json(void)
{
    /* Parse a deeply nested infrastructure-style JSON object and verify:
     *
     * Structure (15 levels deep — within OKJ_MAX_DEPTH of 16):
     *   region -> datacenter -> rack -> server -> vm -> os ->
     *   filesystem -> var -> log -> app -> module -> component ->
     *   settings (array) -> settings[0] (object) -> id, active
     *
     * Expected token count: 32
     *   14 OKJ_OBJECT tokens  (top + 12 nesting layers + settings[0])
     *    1 OKJ_ARRAY  token   (settings)
     *   13 OKJ_STRING keys    (region … component, settings, id, active)
     *    1 OKJ_NUMBER token   (42)
     *    1 OKJ_BOOLEAN token  (true)
     *   -- NOTE: "id" and "active" count toward the 15 string keys total:
     *      region, datacenter, rack, server, vm, os, filesystem, var, log,
     *      app, module, component, settings (13) + id, active (2) = 15
     *   Total = 14 + 1 + 15 + 1 + 1 = 32
     */

    OkJsonParser  parser;
    OkJsonObject *obj;
    OkJsonArray  *arr;
    OkJsonNumber *num;
    OkJsonBoolean *bval;

    char json_str[] =
        "{"
          "\"region\":{"
            "\"datacenter\":{"
              "\"rack\":{"
                "\"server\":{"
                  "\"vm\":{"
                    "\"os\":{"
                      "\"filesystem\":{"
                        "\"var\":{"
                          "\"log\":{"
                            "\"app\":{"
                              "\"module\":{"
                                "\"component\":{"
                                  "\"settings\":[{"
                                    "\"id\":42,"
                                    "\"active\":true"
                                  "}]"
                                "}"
                              "}"
                            "}"
                          "}"
                        "}"
                      "}"
                    "}"
                  "}"
                "}"
              "}"
            "}"
          "}"
        "}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* Token budget */
    assert(parser.token_count == 32U);
    assert(parser.tokens[0].type  == OKJ_OBJECT);
    assert(parser.tokens[26].type == OKJ_ARRAY);    /* settings array      */
    assert(parser.tokens[27].type == OKJ_OBJECT);   /* settings[0] object  */
    assert(parser.tokens[29].type == OKJ_NUMBER);   /* 42                  */
    assert(parser.tokens[31].type == OKJ_BOOLEAN);  /* true                */

    /* Container counts */
    assert(okj_count_objects(&parser) == 14U);
    assert(okj_count_arrays(&parser)  == 1U);

    /* Nested object lookups — each single-member object */
    obj = okj_get_object(&parser, "region");
    assert(obj != NULL);
    assert(obj->count == 1U);

    obj = okj_get_object(&parser, "component");
    assert(obj != NULL);
    assert(obj->count == 1U);   /* sole member: settings */

    /* settings array contains exactly one element */
    arr = okj_get_array(&parser, "settings");
    assert(arr != NULL);
    assert(arr->count == 1U);

    /* Leaf values inside settings[0] */
    num = okj_get_number(&parser, "id");
    assert(num != NULL);
    assert(num->length == 2U);   /* "42" */
    assert(num->start[0] == '4');

    bval = okj_get_boolean(&parser, "active");
    assert(bval != NULL);
    assert(bval->start[0] == 't');   /* true */

    printf("test_deeply_nested_valid_json passed!\n");
}

void test_upper_limits_json(void)
{
    /* Parse a JSON object that simultaneously exercises multiple parser limits:
     *
     *   maxLengthStringTest63Chrs  — string value of 63 bytes, one below the
     *                                OKJ_MAX_STRING_LEN (64) ceiling
     *   largeArrayTest             — array of 60 integers, near OKJ_MAX_ARRAY_SIZE (64)
     *   deepNestingTest            — 14 levels of nesting, two below OKJ_MAX_DEPTH (16)
     *   k1–k16                     — 16 flat key-value pairs covering every primitive
     *                                type: boolean (true/false), null, integer,
     *                                float, and string
     *
     * Parser limit summary:
     *   Tokens used:          125  of 128  (OKJ_MAX_TOKENS)
     *   Root object members:   19  of  32  (OKJ_MAX_OBJECT_SIZE)
     *   Array elements:        60  of  64  (OKJ_MAX_ARRAY_SIZE)
     *   Max nesting depth:     14  of  16  (OKJ_MAX_DEPTH)
     *   String value length:   63  of  64  (OKJ_MAX_STRING_LEN)
     *
     * Token budget (125 total):
     *    1   root OKJ_OBJECT
     *    2   maxLengthStringTest63Chrs: key + STRING value
     *   62   largeArrayTest: key + OKJ_ARRAY + 60 OKJ_NUMBER elements
     *   28   deepNestingTest: key + 13 OKJ_OBJECTs + 13 inner keys + "d15" key + "max" STRING
     *   32   k1–k16: 16 key-value pairs
     */

    OkJsonParser   parser;
    OkJsonString  *str;
    OkJsonNumber  *num;
    OkJsonBoolean *bval;
    OkJsonArray   *arr;
    OkJsonObject  *obj;
    OkJsonToken   *tok;

    char json_str[] =
        "{"
        "\"maxLengthStringTest63Chrs\":"
            "\"123456789012345678901234567890123456789012345678901234567890123\","
        "\"largeArrayTest\":["
            "1,2,3,4,5,6,7,8,9,10,"
            "11,12,13,14,15,16,17,18,19,20,"
            "21,22,23,24,25,26,27,28,29,30,"
            "31,32,33,34,35,36,37,38,39,40,"
            "41,42,43,44,45,46,47,48,49,50,"
            "51,52,53,54,55,56,57,58,59,60"
        "],"
        "\"deepNestingTest\":{"
            "\"d3\":{"
                "\"d4\":{"
                    "\"d5\":{"
                        "\"d6\":{"
                            "\"d7\":{"
                                "\"d8\":{"
                                    "\"d9\":{"
                                        "\"d10\":{"
                                            "\"d11\":{"
                                                "\"d12\":{"
                                                    "\"d13\":{"
                                                        "\"d14\":{"
                                                            "\"d15\":\"max\""
                                                        "}"
                                                    "}"
                                                "}"
                                            "}"
                                        "}"
                                    "}"
                                "}"
                            "}"
                        "}"
                    "}"
                "}"
            "}"
        "},"
        "\"k1\":true,"
        "\"k2\":false,"
        "\"k3\":null,"
        "\"k4\":4,"
        "\"k5\":5.5,"
        "\"k6\":\"six\","
        "\"k7\":7,"
        "\"k8\":8,"
        "\"k9\":9,"
        "\"k10\":10,"
        "\"k11\":11,"
        "\"k12\":12,"
        "\"k13\":13,"
        "\"k14\":14,"
        "\"k15\":15,"
        "\"k16\":16"
        "}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* Token budget: 125 tokens out of OKJ_MAX_TOKENS (128) */
    assert(parser.token_count == 125U);
    assert(parser.tokens[0].type == OKJ_OBJECT);
    assert(parser.depth == 0U);   /* all containers closed */

    /* Structural counts:
     *   14 objects: root + deepNestingTest + d3 through d14
     *    1 array:   largeArrayTest */
    assert(okj_count_objects(&parser) == 14U);
    assert(okj_count_arrays(&parser)  == 1U);

    /* --- max-length string value (63 bytes, one below the 64-byte ceiling) --- */
    str = okj_get_string(&parser, "maxLengthStringTest63Chrs");
    assert(str != NULL);
    assert(str->length == 63U);
    assert(str->start[0]  == '1');
    assert(str->start[62] == '3');   /* last digit of the 63-char sequence */

    /* --- large array (60 elements, near OKJ_MAX_ARRAY_SIZE of 64) --- */
    arr = okj_get_array(&parser, "largeArrayTest");
    assert(arr != NULL);
    assert(arr->count == 60U);

    /* --- deep nesting (14 levels, two below OKJ_MAX_DEPTH of 16) --- */
    obj = okj_get_object(&parser, "deepNestingTest");
    assert(obj != NULL);
    assert(obj->count == 1U);   /* single member: d3 */

    obj = okj_get_object(&parser, "d3");
    assert(obj != NULL);
    assert(obj->count == 1U);   /* single member: d4 */

    /* Leaf string at the deepest level */
    str = okj_get_string(&parser, "d15");
    assert(str != NULL);
    assert(str->length == 3U);   /* "max" */
    assert(str->start[0] == 'm');

    /* --- flat primitive fields: booleans --- */
    bval = okj_get_boolean(&parser, "k1");
    assert(bval != NULL);
    assert(bval->start[0] == 't');   /* true */
    assert(bval->length   == 4U);

    bval = okj_get_boolean(&parser, "k2");
    assert(bval != NULL);
    assert(bval->start[0] == 'f');   /* false */
    assert(bval->length   == 5U);

    /* --- null --- */
    tok = okj_get_token(&parser, "k3");
    assert(tok != NULL);
    assert(tok->type   == OKJ_NULL);
    assert(tok->length == 4U);   /* "null" */

    /* --- integers --- */
    num = okj_get_number(&parser, "k4");
    assert(num != NULL);
    assert(num->length == 1U);   /* "4" */

    num = okj_get_number(&parser, "k16");
    assert(num != NULL);
    assert(num->length == 2U);   /* "16" */

    /* --- float --- */
    num = okj_get_number(&parser, "k5");
    assert(num != NULL);
    assert(num->length == 3U);   /* "5.5" */
    assert(num->start[1] == '.');

    /* --- string primitive --- */
    str = okj_get_string(&parser, "k6");
    assert(str != NULL);
    assert(str->length == 3U);   /* "six" */
    assert(str->start[0] == 's');

    printf("test_upper_limits_json passed!\n");
}

void test_null_byte_in_string_value(void)
{
    /* Verify that a literal null byte injected into the middle of a JSON
     * string value is caught by the parser.  A string literal cannot be used
     * here because the C compiler would truncate it at the embedded '\0';
     * the payload is therefore built as an explicit character array so that
     * the full byte sequence — including the null byte at offset 12 — is
     * preserved in memory.
     *
     * Payload (18 bytes + final NUL):
     *   { " k e y " :   " v a l \0 u e " }
     *   0 1 2 3 4 5 6 7 8 9 ...          17
     *
     * The parser's string-scan loop exits on '\0' (its loop condition checks
     * != '\0'), so it never reaches the closing '"'.  The post-loop check
     * then sees '\0' instead of '"' and returns OKJ_ERROR_UNEXPECTED_END. */

    OkJsonParser parser;
    OkjError     result;

    /* Build {"key": "val\0ue"} manually — cannot use a string literal. */
    char json_str[] = {
        '{', '"', 'k', 'e', 'y', '"', ':', ' ', '"', 'v', 'a', 'l',
        '\0',           /* injected null byte inside the value string */
        'u', 'e', '"', '}',
        '\0'            /* actual C-string terminator for okj_init */
    };

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_UNEXPECTED_END);

    printf("test_null_byte_in_string_value passed!\n");
}

void test_duplicate_key_first_match_wins(void)
{
    /* RFC 8259 §4 leaves the behaviour for duplicate object keys undefined.
     * ok_json performs a linear scan in okj_find_value_index(), so it always
     * returns the first matching key.  This test documents and enforces that
     * "first-match wins" contract so that future changes to the search logic
     * cannot silently switch to "last-match wins" without breaking a test. */

    OkJsonParser  parser;
    OkJsonNumber *num;
    char json_str[] = "{\"x\": 10, \"x\": 99}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    num = okj_get_number(&parser, "x");

    assert(num != NULL);
    assert(num->length == 2U);   /* "10" is 2 characters */
    assert(num->start[0] == '1');
    assert(num->start[1] == '0');

    printf("test_duplicate_key_first_match_wins passed!\n");
}

void test_empty_string_key(void)
{
    /* RFC 7159 / RFC 8259 allow "" as a valid object key.  Verify that:
     *   1. {"": "value"} parses without error.
     *   2. The key token has length == 0.
     *   3. okj_get_string(&parser, "") finds the value token correctly.
     *
     * This exercises the length == 0 path in okj_find_value_index() and the
     * zero-iteration loop in okj_match(), both of which must handle the edge
     * case without infinite-looping or underflowing. */

    OkJsonParser  parser;
    OkJsonString *str;
    char json_str[] = "{\"\":\"value\"}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* Three tokens: OKJ_OBJECT container, empty-string key, string value. */
    assert(parser.token_count == 3U);
    assert(parser.tokens[0].type == OKJ_OBJECT);

    /* Key token must be a zero-length STRING. */
    assert(parser.tokens[1].type   == OKJ_STRING);
    assert(parser.tokens[1].length == 0U);

    /* Value lookup via the empty-string key must succeed. */
    str = okj_get_string(&parser, "");
    assert(str != NULL);

    /* The value is the 5-character string "value". */
    assert(str->length    == 5U);
    assert(str->start[0]  == 'v');
    assert(str->start[1]  == 'a');
    assert(str->start[2]  == 'l');
    assert(str->start[3]  == 'u');
    assert(str->start[4]  == 'e');

    printf("test_empty_string_key passed!\n");
}

void test_number_large_near_json_limit(void)
{
    /* Prove that the numeric scanner handles a number token that spans
     * several hundred characters without overflowing any uint16_t length
     * variable or triggering an out-of-bounds read.
     *
     * Layout (total == OKJ_MAX_JSON_LEN == 4096 bytes, which must succeed):
     *
     *   {"n": <4089-digit integer>}
     *   ^----^                    ^
     *   6 bytes prefix            1 byte suffix
     *
     * The number is "1" followed by 4088 '0' characters, giving a
     * 4089-byte token that is astronomically large but syntactically
     * valid per RFC 8259 §6. */

    OkJsonParser  parser;
    OkjError      result;
    OkJsonNumber *num;

    /* Prefix: {"n":  (6 bytes) + 4089 digit chars + suffix: } (1 byte) +
     * NUL terminator = 4097 bytes total storage. */
    char     json_buf[4097];
    uint16_t pos = 0U;
    uint16_t i;

    /* Write the object header and key. */
    json_buf[pos++] = '{';
    json_buf[pos++] = '"';
    json_buf[pos++] = 'n';
    json_buf[pos++] = '"';
    json_buf[pos++] = ':';
    json_buf[pos++] = ' ';

    /* Write the number: leading '1' then 4088 trailing zeros. */
    json_buf[pos++] = '1';

    for (i = 0U; i < 4088U; i++)
    {
        json_buf[pos++] = '0';
    }

    /* Close the object. */
    json_buf[pos++] = '}';
    json_buf[pos]   = '\0'; /* pos == 4096 == OKJ_MAX_JSON_LEN */

    okj_init(&parser, json_buf);
    result = okj_parse(&parser);

    /* The JSON is exactly OKJ_MAX_JSON_LEN bytes and syntactically valid,
     * so the parser must accept it. */
    assert(result == OKJ_SUCCESS);

    num = okj_get_number(&parser, "n");

    /* The number token must be found and its length must equal the full
     * 4089-character digit sequence without uint16_t truncation. */
    assert(num != NULL);
    assert(num->length == 4089U);
    assert(num->start[0] == '1');

    printf("test_number_large_near_json_limit passed!\n");
}

void test_rfc8259_all_whitespace_between_tokens(void)
{
    /* RFC 8259 §2 defines exactly four insignificant whitespace characters:
     * Space (0x20), Horizontal Tab (0x09), Line Feed (0x0A), and Carriage
     * Return (0x0D).  This test inserts every one of those characters, in
     * all four positions, aggressively between every single token of an
     * array to prove that okj_skip_whitespace() handles consecutive CR, LF,
     * Tab, and Space sequences without looping incorrectly or
     * miscalculating a token start.
     *
     * The payload (shown with escape sequences for clarity) is:
     *
     *   \r\n\t [\r\n\t 1\r\n\t ,\r\n\t 2\r\n\t ,\r\n\t 3\r\n\t ]\r\n\t
     *
     * Every inter-token gap contains CR + LF + Tab + Space in that order,
     * cycling through all four RFC 8259 whitespace code points.
     */

    OkJsonParser parser;
    OkjError     result;

    /* Use a char array literal so the escape sequences are baked in by the
     * compiler.  The leading and trailing whitespace clusters exercise the
     * pre-value skip and the post-value trailing-whitespace acceptance path
     * in okj_parse(). */
    char json_str[] =
        "\r\n\t [\r\n\t 1\r\n\t ,\r\n\t 2\r\n\t ,\r\n\t 3\r\n\t ]\r\n\t ";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    /* The JSON is syntactically valid so the parser must accept it. */
    assert(result == OKJ_SUCCESS);

    /* One array + three number elements = four tokens total. */
    assert(okj_count_elements(&parser) == 4U);

    /* Exactly one array token was produced. */
    assert(okj_count_arrays(&parser) == 1U);

    printf("test_rfc8259_all_whitespace_between_tokens passed!\n");
}

void test_control_char_tab_in_string_value(void)
{
    /* RFC 8259 §7 forbids unescaped control characters (U+0000–U+001F)
     * inside JSON strings.  A literal horizontal-tab byte (0x09, U+0009)
     * is a control character and must therefore be rejected.
     *
     * The parser's inner scan loop reaches the < 0x20U boundary check:
     *
     *   if ((unsigned char)parser->json[parser->position] < 0x20U)
     *       result = OKJ_ERROR_BAD_STRING;
     *
     * 0x09 < 0x20 is true, so the parser must return OKJ_ERROR_BAD_STRING
     * without crashing.
     *
     * Payload bytes: { " s " : " v a l <TAB> u e " }
     *                0x09 is the raw byte at the position marked <TAB>.
     *
     * A regular string literal is used because 0x09 is not a C-string
     * terminator; the hex escape \x09 is embedded directly. */

    OkJsonParser parser;
    OkjError     result;

    /* {"s":"val\x09ue"} — literal tab byte inside the string value */
    char json_str[] = "{\"s\":\"val\x09ue\"}";

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);

    printf("test_control_char_tab_in_string_value passed!\n");
}

void test_control_char_lf_in_string_value(void)
{
    /* RFC 8259 §7 forbids unescaped control characters (U+0000–U+001F)
     * inside JSON strings.  A literal line-feed byte (0x0A, U+000A) is a
     * control character and must be rejected.  This is distinct from the
     * two-character escape sequence \n which is perfectly legal.
     *
     * The same < 0x20U boundary check fires here:
     *
     *   if ((unsigned char)parser->json[parser->position] < 0x20U)
     *       result = OKJ_ERROR_BAD_STRING;
     *
     * 0x0A < 0x20 is true, so the parser must return OKJ_ERROR_BAD_STRING.
     *
     * Payload bytes: { " s " : " v a l <LF> u e " }
     *                0x0A is the raw byte at the position marked <LF>.
     *
     * A raw char array is used to make the embedded 0x0A byte explicit and
     * to avoid any ambiguity with the \n escape sequence. */

    OkJsonParser parser;
    OkjError     result;

    /* Build {"s":"val<LF>ue"} — literal line-feed byte (0x0A) injected
     * inside the string value, not the two-character escape sequence \n. */
    char json_str[] = {
        '{', '"', 's', '"', ':', '"', 'v', 'a', 'l',
        '\x0A',         /* literal line-feed byte — forbidden control char */
        'u', 'e', '"', '}',
        '\0'            /* C-string terminator for okj_init */
    };

    okj_init(&parser, json_str);
    result = okj_parse(&parser);

    assert(result == OKJ_ERROR_BAD_STRING);

    printf("test_control_char_lf_in_string_value passed!\n");
}

void test_quoted_string_spoofing(void)
{
    /* Parse a JSON object whose array value contains string elements that
     * look like structural JSON characters: '{', '}', '[', ']', and ','.
     * These characters must be ignored by the raw-measurement and element-
     * counting helpers because they appear inside quoted strings.
     *
     * Payload: {"tricky": ["{", "}", "[", "]", ","]}
     *
     * Expected results:
     *   okj_count_elements() == 8
     *     (1 object + 1 key string + 1 array + 5 element strings)
     *   arr->count  == 5   (five genuine string elements)
     *   arr->length == 25  (byte length of the array text, see below)
     *
     * Array text byte count:
     *   [ " { " ,   " } " ,   " [ " ,   " ] " ,   " , " ]
     *   1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
     *                     1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2
     * = 25 bytes
     */

    OkJsonParser parser;
    OkJsonArray *arr;

    char json_str[] = "{\"tricky\": [\"{\", \"}\", \"[\", \"]\", \",\"]}";

    okj_init(&parser, json_str);
    assert(okj_parse(&parser) == OKJ_SUCCESS);

    /* Total token count must equal real tokens only – the fake structural
     * characters inside the strings must not inflate this number. */
    assert(okj_count_elements(&parser) == 8U);

    arr = okj_get_array_raw(&parser, "tricky");

    assert(arr != NULL);
    assert(arr->start[0] == '[');

    /* okj_count_array_elements() must not be fooled by '{' '}' '[' ']' ','
     * characters that live inside quoted strings. */
    assert(arr->count == 5U);

    /* okj_measure_container() must traverse the full array text, skipping
     * every string's body so that the brackets and commas within quotes do
     * not prematurely terminate or mis-count the length measurement. */
    assert(arr->length == 25U);

    printf("test_quoted_string_spoofing passed!\n");
}

void test_okj_match_null_src_and_lit(void)
{
    /* okj_match() guards against NULL src and lit by immediately returning 0.
     * Because okj_match() is a static helper it cannot be called directly;
     * both NULL branches are exercised indirectly:
     *
     * NULL src: We hand-craft an OkJsonParser whose STRING token has a NULL
     *           start pointer, then call okj_get_string().  The lookup path
     *           okj_get_string() -> okj_find_value_index() ->
     *           okj_match(NULL, key, len) must return 0 (no match) without
     *           crashing, causing the getter to return NULL.
     *
     * NULL lit: All callers of okj_match() inside ok_json.c either pass a
     *           string-literal keyword ("true"/"false"/"null") or the key
     *           argument, which is guarded by okj_find_value_index()'s own
     *           NULL check.  Passing NULL as the key to okj_get_string() is
     *           therefore caught at the public-API boundary before okj_match()
     *           is ever reached, and the function returns NULL gracefully. */

    OkJsonParser  parser = {0};
    OkJsonString *result;

    /* Hand-craft a parser state with a STRING token whose start is NULL.
     * token_count == 2 causes okj_find_value_index() to iterate once (i == 0).
     * The token passes the type and length checks (OKJ_STRING, length == 1),
     * so okj_match(NULL, "x", 1U) is called, triggering the src-NULL guard
     * which returns 0, meaning the match fails and the getter returns NULL. */
    parser.token_count      = 2U;
    parser.tokens[0].type   = OKJ_STRING;
    parser.tokens[0].start  = NULL;
    parser.tokens[0].length = 1U;  /* same length as the lookup key "x" */

    /* NULL src: okj_match(NULL, "x", 1) -> 0; getter must return NULL. */
    result = okj_get_string(&parser, "x");
    assert(result == NULL);

    /* NULL lit: okj_get_string() rejects a NULL key before reaching
     * okj_match(), so the result is NULL without touching okj_match(). */
    result = okj_get_string(&parser, NULL);
    assert(result == NULL);

    printf("test_okj_match_null_src_and_lit passed!\n");
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
    test_escape_unicode_surrogate_pair();
    test_escape_unknown();
    test_utf8_valid_multibyte();
    test_utf8_invalid_overlong();
    test_utf8_invalid_surrogate();
    test_utf8_invalid_truncated();
    /* MC/DC tests for okj_validate_utf8_sequence */
    test_utf8_2byte_valid_boundary_min();
    test_utf8_2byte_valid_boundary_max();
    test_utf8_2byte_b1_not_continuation();
    test_utf8_3byte_e0_valid();
    test_utf8_3byte_e0_b1_below_range();
    test_utf8_3byte_e0_b1_above_range();
    test_utf8_3byte_e0_b2_not_continuation();
    test_utf8_3byte_e1_ec_boundary_min();
    test_utf8_3byte_e1_ec_boundary_max();
    test_utf8_3byte_ee_ef_boundary_min();
    test_utf8_3byte_ee_ef_boundary_max();
    test_utf8_3byte_range_b1_not_continuation();
    test_utf8_3byte_range_b2_not_continuation();
    test_utf8_3byte_ed_valid();
    test_utf8_3byte_ed_valid_boundary_max();
    test_utf8_3byte_ed_b1_below_range();
    test_utf8_3byte_ed_b2_not_continuation();
    test_utf8_4byte_f0_b1_below_range();
    test_utf8_4byte_f0_b1_above_range();
    test_utf8_4byte_f0_b2_not_continuation();
    test_utf8_4byte_f0_b3_not_continuation();
    test_utf8_4byte_f1_f3_valid();
    test_utf8_4byte_f1_f3_boundary_max();
    test_utf8_4byte_f1_f3_b1_not_continuation();
    test_utf8_4byte_f1_f3_b2_not_continuation();
    test_utf8_4byte_f1_f3_b3_not_continuation();
    test_utf8_4byte_f4_valid();
    test_utf8_4byte_f4_valid_boundary_max();
    test_utf8_4byte_f4_b1_below_range();
    test_utf8_4byte_f4_b1_above_range();
    test_utf8_4byte_f4_b2_not_continuation();
    test_utf8_4byte_f4_b3_not_continuation();
    test_array_too_large();
    test_object_too_large();
    test_object_exactly_32_members();
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
    test_max_json_len_boundary();
    test_parse_null_parser();
    test_null_pointer_safety();
    test_truncated_backslash_at_eof();
    test_copy_string_basic();
    test_copy_string_null_terminated();
    test_copy_string_truncation();
    test_copy_string_one_byte_buf();
    test_copy_string_exact_fit();
    test_copy_string_null_inputs();
    test_find_key_over_max_len();
    test_depth_stack_bracket_mismatch_obj();
    test_depth_stack_bracket_mismatch_arr();
    test_depth_stack_extra_close_brace();
    test_depth_stack_extra_close_bracket();
    test_depth_stack_unclosed_object();
    test_depth_stack_unclosed_array();
    test_depth_stack_mixed_nesting();
    test_depth_stack_alternating_unwind();
    test_trailing_whitespace_after_object();
    test_trailing_garbage_after_object();
    test_trailing_garbage_after_array();
    test_trailing_garbage_after_primitive();
    test_two_top_level_primitives();
    test_two_top_level_objects();

    /* RFC 8259 grammar compliance */
    test_trailing_comma_in_object();
    test_trailing_comma_in_array();
    test_number_as_object_key();
    test_boolean_as_object_key();
    test_null_as_object_key();
    test_missing_colon();
    test_colon_without_key();
    test_double_colon();
    test_empty_input();
    test_whitespace_only_input();
    test_keyword_no_boundary();
    test_value_after_value_in_object();
    test_comma_at_start_of_array();
    test_comma_at_start_of_object();
    test_top_level_number();
    test_top_level_string();
    test_top_level_boolean();
    test_top_level_null();
    test_iot_sensor_json();
    test_user_data_json();
    test_deeply_nested_valid_json();
    test_upper_limits_json();
    test_null_byte_in_string_value();
    test_duplicate_key_first_match_wins();
    test_empty_string_key();
    test_number_large_near_json_limit();

    /* RFC 8259 whitespace compliance */
    test_rfc8259_all_whitespace_between_tokens();

    /* RFC 8259 §7 bare control character rejection (U+0000–U+001F) */
    test_control_char_tab_in_string_value();
    test_control_char_lf_in_string_value();

    /* okj_skip_string() robustness: structural chars inside quoted strings */
    test_quoted_string_spoofing();

    /* Modified UTF-8 overlong NUL (0xC0 0x80) attack vector */
    test_utf8_overlong_nul_c0_80();

    /* Backslash-flood / escape-straddling buffer-exhaustion attack vectors */
    test_backslash_flood_at_limit();
    test_backslash_flood_one_over();
    test_backslash_straddle_limit();
    test_backslash_truncated_at_boundary();

    /* okj_match() NULL src and lit parameter guard */
    test_okj_match_null_src_and_lit();

    printf("All OK_JSON tests passed!\n");

    return 0;
}
