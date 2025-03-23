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

/* TODO: Remove dependency on these headers: */
#include <ctype.h>
#include <string.h>

/* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
#include <stdio.h>

void okj_init(OkJsonParser *parser, char *json_string)
{
    if (parser != NULL && json_string != NULL)
    {
        parser->json = json_string;

        parser->position    = 0;
        parser->token_count = 0;
    }
}

static void okj_skip_whitespace(OkJsonParser *parser)
{
    if (parser != NULL)
    {
        while (isspace(parser->json[parser->position]))
        {
            parser->position++;
        }
    }
}

static int okj_parse_value(OkJsonParser *parser)
{
    /* TODO: Refactor to use only one return. */
    if (parser == NULL)
    {
        return OKJ_ERROR_BAD_POINTER;
    }

    okj_skip_whitespace(parser);

    char c = parser->json[parser->position];
    
    if (c == '{')
    {
        parser->tokens[parser->token_count].type = OKJ_OBJECT;

        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nFound object.\n");
    }
    else if (c == '[')
    {
        parser->tokens[parser->token_count].type = OKJ_ARRAY;

        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nFound array.\n");
    }
    else if (c == '"')
    {
        parser->tokens[parser->token_count].type = OKJ_STRING;

        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nFound string.\n");

        uint16_t start = parser->position;

        while (
               parser->json[++parser->position] != '\0' &&
               //(isalnum(parser->json[++parser->position]))  ||
               /* (ispunct(parser->json[parser->position]))    || TODO: Will come back to this. */
               (parser->json[parser->position] != '"'))
        {
            /* Intentionally empty */
        }

        // /* A number cannot end with '.' */
        // if (parser->json[parser->position - 1] == '.')
        // {
        //     /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        //     printf("\nBAD number!!!\n");

        //     return OKJ_ERROR_BAD_NUMBER;
        // }

        parser->tokens[parser->token_count].length = parser->position - start;
    }
    else if ((isdigit(c)) || (c == '-'))
    {
        parser->tokens[parser->token_count].type = OKJ_NUMBER;

        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nFound number.\n");

        uint16_t start = parser->position;

        while ((isdigit(parser->json[++parser->position]))  ||
               (parser->json[parser->position] == '.'))
        {
            /* Intentionally empty */
        }

        /* A number cannot end with '.' */
        if (parser->json[parser->position - 1] == '.')
        {
            /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
            printf("\nBAD number!!!\n");

            return OKJ_ERROR_BAD_NUMBER;
        }

        parser->tokens[parser->token_count].length = parser->position - start;

        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nToken length == %d.\n", parser->tokens[parser->token_count].length);
    }
    else if ((strncmp(&parser->json[parser->position], "true",  4) == 0)   ||
             (strncmp(&parser->json[parser->position], "false", 5) == 0))
    {
        parser->tokens[parser->token_count].type = OKJ_BOOLEAN;

        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nFound boolean\n");
    }
    else if (strncmp(&parser->json[parser->position], "null", 4) == 0)
    {
        parser->tokens[parser->token_count].type = OKJ_NULL;

        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nFound null.\n");
    }
    else
    {
        /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
        printf("\nJSON SYNTAX ERROR!\n");

        /* Error: invalid JSON */
        return OKJ_ERROR_SYNTAX;
    }

    parser->tokens[parser->token_count].start = &parser->json[parser->position];

    /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
    printf("\nToken start == %c\n", *parser->tokens[parser->token_count].start);

    /* TODO: Placeholder, actual length needs to be determined. */
    //parser->tokens[parser->token_count].length = 1;

    /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
    printf("\nToken length == %d.\n", parser->tokens[parser->token_count].length);

    parser->position++;

    /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
    printf("\nParser position == %d.\n", parser->position);

    parser->token_count++;

    /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
    printf("\nToken count == %d.\n", parser->token_count);

    return OKJ_SUCCESS;
}

OkjError okj_parse(OkJsonParser *parser)
{
    /* TODO: Refactor to use only one return. */
    if (parser == NULL)
    {
        return OKJ_ERROR_BAD_POINTER;
    }

    /* okj_init(parser, parser->json); */

    /* TODO: What if the data legitmately contains the null char?  Is that possible? */
    while ((parser->json[parser->position] != '\0') &&
           (parser->token_count < OKJ_MAX_TOKENS))
    {
        if (okj_parse_value(parser) != OKJ_SUCCESS)
        {
            /* RLM - FOR DEBUGGING!  REMOVE BEFORE COMMIT */
            printf("\nPARSING ERROR!!!\n");

            /* Parsing error */
            return OKJ_ERROR_PARSING_FAILED;
        }
    }
    
    return OKJ_SUCCESS;
}

OkJsonString *okj_get_string(OkJsonParser *parser, const char *key)
{
    /* TODO: stub */
    if (parser == NULL || key == NULL)
    {
        return NULL;
    }

    (void)parser;
    (void)key;

    return NULL;
}

OkJsonNumber *okj_get_number(OkJsonParser *parser, const char *key)
{
    /* TODO: stub */
    if (parser == NULL || key == NULL)
    {
        return NULL;
    }

    (void)parser;
    (void)key;
    
    return NULL;
}

OkJsonBoolean *okj_get_boolean(OkJsonParser *parser, const char *key)
{
    /* TODO: stub */
    if (parser == NULL || key == NULL)
    {
        return NULL;
    }

    (void)parser;
    (void)key;

    return NULL;
}

OkJsonArray *okj_get_array(OkJsonParser *parser, const char *key)
{
    /* TODO: stub */
    if (parser == NULL || key == NULL)
    {
        return NULL;
    }

    (void)parser;
    (void)key;

    return NULL;
}

OkJsonObject *okj_get_object(OkJsonParser *parser, const char *key)
{
    /* TODO: stub */
    if (parser == NULL || key == NULL)
    {
        return NULL;
    }

    (void)parser;
    (void)key;

    return NULL;
}

OkJsonToken *okj_get_token(OkJsonParser *parser, const char *key)
{
    /* TODO: stub */
    if (parser == NULL || key == NULL)
    {
        return NULL;
    }

    (void)parser;
    (void)key;
    
    return NULL;
}
