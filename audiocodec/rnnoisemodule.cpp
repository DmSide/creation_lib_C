#include "rnnoisemodule.h"


bool RNnoiseModule::init()
{
   den_state = rnnoise_create(NULL);
   if (den_state)
   {
       is_first = true;
        return true;
   }
   return false;
}

bool RNnoiseModule::rnnoise_buffer(TaskOutBuffer& buffer,std::ostringstream& error_stream)
{
    float* x = new float[480];
    uint8_t* tmp_buf = new uint8_t[960];
    buffer.read(tmp_buf, data_size);
    for (int i =0; i < 480; i++)
    {
        x[i] = ((short*)tmp_buf)[i];
    }
    delete [] tmp_buf;

    rnnoise_process_frame(den_state, x, x);
    short* out_buf = new short[480];
    for (int i = 0; i < 480; i++)
    {
        out_buf[i] = x[i];
    }
    delete[] x;

    bool res = true;
    if (!is_first)
    {
        res = out((uint8_t*)out_buf, 960,error_stream);
    }

    is_first = false;

    delete[] out_buf;

    return res;
}

bool RNnoiseModule::process(TaskOutBuffer& buffer,std::ostringstream& error_stream)
{
    return start(buffer,error_stream);
}
bool RNnoiseModule::finish(TaskOutBuffer& buffer,std::ostringstream& error_stream)
{
    if(!start(buffer,error_stream))
        return false;
    return nextFinish(error_stream);

}

bool RNnoiseModule::start(TaskOutBuffer& buffer,std::ostringstream& error_stream)
{

    while (buffer.available() >= data_size)
    {
        rnnoise_buffer(buffer,error_stream);
    }
    return true;
}

RNnoiseModule::RNnoiseModule(TaskJoinModule* next):TaskAppendModule(next)
{

}

