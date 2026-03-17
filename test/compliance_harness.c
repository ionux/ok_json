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
