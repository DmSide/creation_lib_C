#include "audiocodec.h"
#include "parameters.h"
#include "parameters_rnnoise_off.h"
extern "C"
{
    void* init(bool rnnoise_on)
    {
        if (rnnoise_on)
        {
            Parameters* audiocodec = nullptr;
            try {
                audiocodec = new Parameters();
                if (!audiocodec->process.run())
                {
                    delete audiocodec;
                    audiocodec = nullptr;
                }
            } catch (...) {
                delete audiocodec;
                audiocodec = nullptr;
            }
            return audiocodec;
        } else {
            Parameters_Rnnoise_OFF* audiocodec = nullptr;
            try {
                audiocodec = new Parameters_Rnnoise_OFF();
                if (!audiocodec->process.run())
                {
                    delete audiocodec;
                    audiocodec = nullptr;
                }
            } catch (...) {
                delete audiocodec;
                audiocodec = nullptr;
            }
            return audiocodec;
        }

    }

    int put_input_data(uint8_t *input_data, unsigned int data_size, void* audiocodec, bool rnnoise_on)
    {
        try { // although it won't be able to catch segfault
            if (rnnoise_on)
            {
                Parameters *b = static_cast<Parameters *>(audiocodec);
                if (b->end_of_input)
                {
                    return Results::END_OF_INPUT;
                }
                if (input_data == NULL && data_size == 0)
                {
                    b->m_input_buffer.putEOF();
                    b->end_of_input = true;
                    return 0; // Results::SET_END_OF_INPUT;
                }
                if (b->m_input_buffer.put(input_data, data_size) == data_size)
                {
                    return 0; //Success
                } else {
                    return Results::ERROR_PUT_DATA;
                }
            } else {
                Parameters_Rnnoise_OFF* b = static_cast<Parameters_Rnnoise_OFF *>(audiocodec);
                if (b->end_of_input)
                {
                    return Results::END_OF_INPUT;
                }
                if (input_data == NULL && data_size == 0)
                {
                    b->m_input_buffer.putEOF();
                    b->end_of_input = true;
                    return 0; // Results::SET_END_OF_INPUT;
                }
                if (b->m_input_buffer.put(input_data, data_size) == data_size)
                {
                    return 0; //Success
                } else {
                    return Results::ERROR_PUT_DATA;
                }
            }
        } catch (...) {
            return Results::PROCESS_ERROR;
        }
    }

    int get_output_data(uint8_t* dst_buffer, unsigned int count, void* audiocodec, bool rnnoise_on)
    {
        try {
            size_t read = 0;
            if (rnnoise_on)
            {
                Parameters* b = static_cast<Parameters*>(audiocodec);
                if (b->process.condition() == SyncTaskCondition::S_T_ERROR)
                {
                    return Results::PROCESS_ERROR;
                }
                if (b->end_of_output)
                {
                    return Results::END_OF_OUTPUT;
                }

                read = b->m_output_buffer.read(dst_buffer, count);
                if (read == 0) {
                    if (b->m_output_buffer.isError())
                        return Results::PROCESS_ERROR;
                    if (b->m_output_buffer.isEOF()) {
                        b->end_of_output = true;
                        return Results::END_OF_OUTPUT;
                    }
                }
            } else {
                Parameters_Rnnoise_OFF* b = static_cast<Parameters_Rnnoise_OFF *>(audiocodec);
                if (b->process.condition() == SyncTaskCondition::S_T_ERROR)
                {
                    return Results::PROCESS_ERROR;
                }
                if (b->end_of_output)
                {
                    return Results::END_OF_OUTPUT;
                }

                read = b->m_output_buffer.read(dst_buffer, count);
                if (read == 0) {
                    if (b->m_output_buffer.isError()) // it is actually never set?
                        return Results::PROCESS_ERROR;
                    if (b->m_output_buffer.isEOF()) {
                        b->end_of_output = true;
                        return Results::END_OF_OUTPUT;
                    }
                }
            }
            return read;
        } catch (...) {
            return Results::PROCESS_ERROR;
        }
    }

    void release(void *audiocodec, bool rnnoise_on)
    {
        if (rnnoise_on)
        {
            Parameters *b = static_cast<Parameters *>(audiocodec);
            delete b;
        } else {
            Parameters_Rnnoise_OFF* b = static_cast<Parameters_Rnnoise_OFF *>(audiocodec);
            delete b;
        }
    }
}

