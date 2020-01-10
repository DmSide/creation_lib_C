#ifndef PARAMETERS
#define PARAMETERS

#include "outbuffermodule.h"
#include "ffmpegdecodermodule.h"
#include "ffmpegencodermodule.h"
#include "rnnoisemodule.h"
#include "synctask.h"

#include "plain_queue_buffer.h"
#include "whole_plain_queue_buffer.h"
#include "threadsafe_queue_buffer.h"
#include <cstring>


void generate_header(uint8_t* buffer, short int channel, int sample_rate)
{
    memcpy(buffer, "RIFF",4);
    int len = 36;
    memcpy(buffer+4, &len, sizeof(len));
    memcpy(buffer+8, "WAVEfmt ", 8);
    int len_format_data = 16;
    memcpy(buffer+16, &len_format_data, sizeof(len_format_data));
    short int format_type = 1;
    memcpy(buffer+20, &format_type, sizeof(format_type));
    memcpy(buffer+22, &channel, sizeof(channel));
    memcpy(buffer+24, &sample_rate, sizeof(sample_rate));
    short int bits_p_sample = 16;
    int byte_rate = sample_rate * bits_p_sample * channel / 8;
    memcpy(buffer+28, &byte_rate, sizeof(byte_rate));
    memcpy(buffer+34, &bits_p_sample, sizeof(bits_p_sample));
    memcpy(buffer+36, "data", 4);
    len = 0;
    memcpy(buffer+40, &len, sizeof(len));
}


struct Parameters
{
    ThreadSafeQueueBufferWrapper<WholePlainQueueBuffer> m_input_buffer;
    ThreadSafeQueueBufferWrapper<PlainQueueBuffer> m_output_buffer;

    bool end_of_input;
    bool end_of_output;

    OutBufferModule outModule;
    FfmpegEncoderModule encoder;
    RNnoiseModule rnnoise;
    FfmpegDecoderModule decoder;
    SyncTask process;
    Parameters() :
        m_input_buffer(),
        m_output_buffer(),
        outModule(&m_output_buffer),
        encoder(&outModule, true),
        rnnoise(&encoder),
        decoder(&rnnoise, m_input_buffer, 48000),
        process(decoder) {
        end_of_input = false;
        end_of_output = false;
        uint8_t * head = new uint8_t[44];
        memset(head, 0, 44);
        generate_header(head, 1, 16000);
        m_output_buffer.put(head, 44);
        delete[] head;
    }
    ~Parameters() {
        // to make sure that background process will stop at some point
        // (because after that ~SyncTask will be called and it will wait for thread to finish)
        m_input_buffer.setError();
    }
};
#endif // PARAMETERS

