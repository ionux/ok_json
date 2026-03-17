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
