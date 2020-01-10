#ifndef TEST_H
#define TEST_H
#include <chrono>
#include <thread>

#include "synctask.h"
#include "ffmpegdecodermodule.h"
#include "ffmpegencodermodule.h"
#include "outbuffermodule.h"
#include "rnnoisemodule.h"
#include "cyclic_buffer.h"
#include <iostream>

struct BufferProperties
{
    size_t left_size;
    size_t offset;
    bool is_first;
    BufferProperties(){
        left_size = 0;
        offset = 0;
        is_first = true;
    }
    ~BufferProperties(){}
};


struct Parameters
{
    MultiThreadBuffer inputBuffer;
    bool end_of_input;
    bool end_of_output;
    MultiThreadBuffer outBuffer;
    OutBufferModule outModule;
    FfmpegEncoderModule encoder;
    RNnoiseModule rnnoise;
    FfmpegDecoderModule decoder;
    SyncTask process;
    BufferProperties prop_out_buf;
    Parameters():prop_out_buf(), inputBuffer(), outBuffer(), outModule(&outBuffer), encoder(&outModule), rnnoise(&encoder), decoder(&rnnoise, inputBuffer, 48000), process(decoder){
        end_of_input = false;
        end_of_output = false;
    }
    ~Parameters(){}
};


class Test{


    bool run(FILE* file,FILE* outFile)
    {
        MultiThreadBuffer inputBuffer;
        MultiThreadBuffer outBuffer;
        OutBufferModule outModule(&outBuffer);
        FfmpegEncoderModule encoder(&outModule);
        if (!encoder.init(AV_CODEC_ID_PCM_S16LE, 64000, AV_SAMPLE_FMT_S16, 48000))
            return false;
        RNnoiseModule rnnoise(&encoder);
        if (!rnnoise.init())
            return false;
        FfmpegDecoderModule decoder(&rnnoise,inputBuffer, 48000);
        SyncTask process(decoder);




        if (!process.run())
            return false;
        while (process.condition() == SyncTaskCondition::S_T_RUN)
        {
            size_t write_size = inputBuffer.nextSize();
            std::cout << " " << write_size << std::endl;
            if (write_size > 0)
            {
                //write_size = min(100, write_size);
                unsigned char* data = new unsigned char[write_size];
                size_t length = fread(data, sizeof(unsigned char), write_size, file);
                if (length != 0) {
                    inputBuffer.append(data, length);
                }
                else {
                    inputBuffer.setEOF();
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        fwrite(outBuffer.data(), sizeof(unsigned char), outBuffer.available(), outFile);
        return  process.condition() == SyncTaskCondition::S_T_COMPLETE;
    }


public:
    bool test(const char* inputpath,const char* outpath)
    {
        FILE* file = fopen(inputpath, "rb");
        if (file == NULL){
             fprintf(stderr, "Could not open %s\n", inputpath);
             return false;
        }
        FILE *f = fopen(outpath, "wb");
        if (f==NULL) {
             fclose(file);
            fprintf(stderr, "Could not open %s\n", outpath);
            return false;
        }
        bool result = run(file, f);
        fclose(file);
        fclose(f);
        return result;
    }  

};

#endif // TEST_H
