#include "outbuffermodule.h"


OutBufferModule::OutBufferModule(QueueBuffer * buffer):m_buffer(buffer)
{
}

OutBufferModule::~OutBufferModule()
{
}

bool OutBufferModule::process(TaskOutBuffer & buffer,std::ostringstream& error_stream)
{
    if (m_buffer && buffer.available() > 0) {
        size_t available = buffer.available();
        if (m_buffer->put(buffer.data(), available) == available)
        {
            buffer.setEOF();
            return true;
        }else{
             error_stream<<"(OutBufferModule::process) Could not append data"<<std::endl;
            return false;
        }
    }
    return true;
}

bool OutBufferModule::finish(TaskOutBuffer & buffer,std::ostringstream& error_stream)
{
    if(m_buffer)
    {
        if (buffer.available() > 0) {
            size_t available = buffer.available();
            if (m_buffer->put(buffer.data(), available) != available)
            {
                error_stream<<"(OutBufferModule::finish) Could not append data"<<std::endl;
                return false;
            }
        }
        m_buffer->putEOF();
    }
    return true;
}
