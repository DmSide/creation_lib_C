#ifndef AUDIOCODEC
#define AUDIOCODEC

#include <stdint.h>
#include "results.h"
extern "C"
{
    void* init(bool rnnoise_on);
    int put_input_data(uint8_t *input_data, unsigned int data_size, void* audiocodec, bool rnnoise_on);
    int get_output_data(uint8_t* dst_buffer, unsigned int count, void* audiocodec, bool rnnoise_on);
    void release(void *audiocodec, bool rnnoise_on);
}

#endif // AUDIOCODEC

