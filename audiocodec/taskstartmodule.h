#ifndef TASKSTARTMODULE_H
#define TASKSTARTMODULE_H
#include "taskappendmodule.h"
#include "taskoutbuffer.h"

/**
 * @brief The TaskStartModule class Describes the starting module of the task. Point of entry
 */
class TaskStartModule:public TaskAppendModule {
public:

        /**
        * @brief TaskStartModule Constructor
        * @param next Next module
        */
        TaskStartModule(TaskJoinModule* next):TaskAppendModule(next) {}

        /**
         * @brief process Virtual method of starting a task.
         * Here begins the initialization of the module, processing and completion of the task.
         * @param error_stream Steam for errors
         * @return True if everything went well. False if bad luck
         */
        virtual bool process(std::ostringstream& error_stream) = 0;
};
#endif // TASKSTARTMODULE_H
