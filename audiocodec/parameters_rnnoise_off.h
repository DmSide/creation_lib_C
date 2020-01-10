#ifndef PARAMETERS_RNNOISE_OFF
#define PARAMETERS_RNNOISE_OFF

#include "outbuffermodule.h"
#include "ffmpegdecodermodule.h"
#include "ffmpegencodermodule.h"
#include "rnnoisemodule.h"
#include "synctask.h"

#include "plain_queue_buffer.h"
#include "whole_plain_queue_buffer.h"
#include "threadsafe_queue_buffer.h"


struct Parameters_Rnnoise_OFF
{
    ThreadSafeQueueBufferWrapper<WholePlainQueueBuffer> m_input_buffer;
    ThreadSafeQueueBufferWrapper<PlainQueueBuffer> m_output_buffer;

    bool end_of_input;
    bool end_of_output;

    OutBufferModule outModule;
    FfmpegEncoderModule encoder;
    FfmpegDecoderModule decoder;
    SyncTask process;
    Parameters_Rnnoise_OFF() :
        m_input_buffer(),
        m_output_buffer(),
        outModule(&m_output_buffer),
        encoder(&outModule, false),
        decoder(&encoder, m_input_buffer, 16000),
        process(decoder) {
        end_of_input = false;
        end_of_output = false;
    }
    ~Parameters_Rnnoise_OFF() {
        // to make sure that background process will stop at some point
        // (because after that ~SyncTask will be called and it will wait for thread to finish)
        m_input_buffer.setError();
    }
};

#endif // PARAMETERS_RNNOISE_OFF

