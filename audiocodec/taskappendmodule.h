#ifndef TASKAPPENDMODULE_H
#define TASKAPPENDMODULE_H
#include "taskjoinmodule.h"
#include "taskoutbuffer.h"

/**
 * @brief The TaskAppendModule class Describes a task module that supports the attachment of another module.
 */
class TaskAppendModule
{
private:
        TaskJoinModule* m_next;
        TaskOutBuffer m_out_buffer;

        bool nextProccess(std::ostringstream& error_stream)
        {
                if (!m_next)
                        return true;
                return m_next->process(m_out_buffer,error_stream);
        }

public:
        /**
         * @brief TaskAppendModule Constructor
         * @param next Next module
         */
        TaskAppendModule(TaskJoinModule* next) :m_next(next) {}

protected:

        /**
         * @brief out The method that should be called when the current module received the data to exit (Pass the processed data to the next module)
         * Calls the Process method of the next module if present.
         * @param data Pointer to the data
         * @param size Data size
         * @param error_stream Steam for errors
         * @return True if everything went well. False if bad luck
         */
        bool out(unsigned char* data, size_t size,std::ostringstream& error_stream)
        {
                if (!m_out_buffer.append(data, size)){
                     error_stream<<"(TaskAppendModule::out) Could not append data"<<std::endl;
                     return false;
                }
                return nextProccess(error_stream);
        }

        /**
         * @brief nextFinish Method that should be called after the end of the module.
         * Calls the Finish method of the next module if present.
         * @param error_stream Steam for errors
         * @return True if everything went well. False if bad luck
         */
        bool nextFinish(std::ostringstream& error_stream)
        {
                if (!m_next)
                        return true;
                return m_next->finish(m_out_buffer,error_stream);
        }


};
#endif // TASKAPPENDMODULE_H
