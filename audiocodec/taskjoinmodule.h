#ifndef TASKJOINMODULE_H
#define TASKJOINMODULE_H


#include "taskoutbuffer.h"
#include <sstream>



/**
 * @brief The TaskJoinModule virtual class Represents a task module
 */
class TaskJoinModule
{

public:
    /**
     * @brief process Called every time data appears to be processed.
     * @param buffer Data buffer
     * @param error_stream Steam for errors
     * @return True if everything went well. False in case of error.
     * If there is still not enough data, then you should not return a False
     */
    virtual bool process(TaskOutBuffer& buffer,std::ostringstream& error_stream) = 0;
    /**
     * @brief finish Called upon completion of the task
     * @param buffer Data buffer
     * @param error_stream Steam for errors
     * @return True if everything went well. False in case of error.
     */
    virtual bool finish(TaskOutBuffer& buffer,std::ostringstream& error_stream) = 0;

};

#endif // TASKJOINMODULE_H
