#ifndef OUTBUFFERMODULE_H
#define OUTBUFFERMODULE_H

#include "taskjoinmodule.h"

#include "queue_buffer.h"

class OutBufferModule:public TaskJoinModule
{
    QueueBuffer * m_buffer;

public:
    OutBufferModule(QueueBuffer* buffer);
    ~OutBufferModule();
    bool process(TaskOutBuffer& buffer,std::ostringstream& error_stream);
    bool finish(TaskOutBuffer& buffer,std::ostringstream& error_stream);
};


#endif // OUTBUFFERMODULE_H
