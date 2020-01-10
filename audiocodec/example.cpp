#include <iostream>
#include "audiocodec.h"


int main(){
    const char* input_path="//home//user//Загрузки//cello1.wav";
    const char* out_path="//home//user//Загрузки//out_wo_rnnoise.wav";
    FILE *in_file = fopen(input_path, "rb");
    int size = 330828;
    uint8_t* input_buffer = new uint8_t[size];
    fread(input_buffer, 1, size, in_file);
    fclose(in_file);
    int out_buffer_size = 120000;
    uint8_t* out_buffer = new uint8_t[out_buffer_size];
    int out_data_size = 0, size_of_data = 0;
    int status = 0;
    bool rnnoise_on = true;

    void* audiocodec = init(rnnoise_on);

    /*push input data for further conversion*/
    status = put_input_data(input_buffer, size, audiocodec, rnnoise_on);
    if (status == Results::ERROR_PUT_DATA)
    {
        release(audiocodec, rnnoise_on);
        return -1;
    }

    /*push input data = NULL and size = 0 for stoping input*/
    status = put_input_data(NULL, 0, audiocodec, rnnoise_on);
    if (status == Results::ERROR_PUT_DATA)
    {
        release(audiocodec, rnnoise_on);
        return -1;
    }

    while (true)
    {
        /*Try to get output data (return size of count or the total number of elements successfully read)*/
        size_of_data = get_output_data(out_buffer + out_data_size, 250, audiocodec, rnnoise_on);
        if (size_of_data == Results::END_OF_OUTPUT)
        {
            break;
        } else if (size_of_data == Results::PROCESS_ERROR) {
            release(audiocodec, rnnoise_on);
            return -1;
        } else {
            out_data_size += size_of_data;
        }
    }

    FILE *out_file = fopen(out_path, "wb");
    fwrite(out_buffer, 1, out_data_size, out_file);
    fclose(out_file);
    release(audiocodec, rnnoise_on);
    delete[] out_buffer;
    delete[] input_buffer;
    return 0;
}


