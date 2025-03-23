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

#include "../include/ok_json.h"

#include <stdio.h>
#include <assert.h>

void test_parse_simple_object(void);
void test_parse_array(void);
void test_invalid_json(void);

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
     *   - Parse the correct number of objects (two, in this case)
     */
        
    OkJsonParser parser;
    char json_str[] = "{\"key\": 42}";
    
    okj_init(&parser, json_str);

    OkjError result = okj_parse(&parser);

    assert(result == OKJ_SUCCESS); 
    assert(parser.token_count == 2);

    assert(parser.tokens[0].type == OKJ_OBJECT);
    assert(parser.tokens[1].type == OKJ_NUMBER);

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

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    
    test_parse_simple_object();
    test_parse_array();
    test_invalid_json();

    printf("All OK_JSON tests passed!\n");

    return 0;
}
