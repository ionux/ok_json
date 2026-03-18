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

#include <stdio.h>
#include <stdlib.h>

/* Include your implementation directly */
#include "../src/ok_json.c"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (f == NULL)
    {
        return 1;
    }

    /* Determine file size */
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* Enforce your parser's design limits */
    if (fsize < 0 || fsize > OKJ_MAX_JSON_LEN)
    {
        fclose(f);
        return 1; 
    }

    /* Read the file into a buffer */
    char *json_data = malloc((size_t)fsize + 1);
    if (json_data == NULL)
    {
        fclose(f);
        return 1;
    }
    
    size_t read_len = fread(json_data, 1, (size_t)fsize, f);
    fclose(f);
    
    json_data[read_len] = '\0';

    /* Run the parser */
    OkJsonParser parser;
    okj_init(&parser, json_data, (uint16_t)read_len);
    OkjError result = okj_parse(&parser);

    free(json_data);

    /* Return 0 on success, 1 on any error */
    return (result == OKJ_SUCCESS) ? 0 : 1;
}
