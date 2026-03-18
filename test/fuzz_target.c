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

#include <stdint.h>
#include <stddef.h>

/* Include the implementation directly just like your unit tests */
#include "../src/ok_json.c"

/**
 * @brief libFuzzer requires a single entry-point function called LLVMFuzzerTestOneInput.
 * It feeds random byte arrays of various sizes into your parser to see if it crashes
 * or does an out-of-bounds read.
 */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* ok_json has a hard limit; ignore inputs larger than it can handle */
    if (size > OKJ_MAX_JSON_LEN)
    {
        return 0; 
    }

    /* okj_init expects a mutable char array. Copy the fuzzer data 
     * into a local buffer to prevent modifying the read-only fuzzer input. */
    char input_buf[OKJ_MAX_JSON_LEN];

    for (size_t i = 0; i < size; i++)
    {
        input_buf[i] = (char)data[i];
    }

    OkJsonParser parser;

    okj_init(&parser, input_buf, (uint16_t)size);
    
    /* We don't care about the return code (syntax errors are expected with fuzzing),
     * we only care if this function causes a segfault, hang, or ASan violation. */
    okj_parse(&parser);

    return 0;
}
