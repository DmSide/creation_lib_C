#ifndef SYNCTASK_H
#define SYNCTASK_H

#include <thread>
#include "taskstartmodule.h"

/**
 * @brief The SyncTaskCondition enum Describes the state of SyncTask
 */
enum  SyncTaskCondition
{
    S_T_NONE,
    /**
      Task started
     **/
    S_T_RUN,
    /**
      Task completed successfully
      **/
    S_T_COMPLETE,
    /**
      The Task is terminated with an error
      **/
    S_T_ERROR
};

/**
 * @brief The SyncTask class Represents a sync task runable in a separate thread
 */
class SyncTask
{
    TaskStartModule* m_start;
    SyncTaskCondition m_condition;
    std::thread* m_thread;
    std::ostringstream m_stream;
public:
    /**
     * @brief SyncTask Constructor
     * @param start Task Startup Module
     */
    SyncTask(TaskStartModule& start);
    /**
     * @brief join Blocks the current thread until the task is completed
     */
    void join();

    /**
     * @brief error_stream
     * @return error stream
     */
    std::ostringstream * error_stream();

    ~SyncTask();
    /**
     * @brief run Runs the current task in a separate thread.
     * @return True if it turned out otherwise a false
     */
    bool run();
    /**
     * @brief condition
     * @return  state of the SyncTask
     */
    SyncTaskCondition condition();

};

#endif // SYNCTASK_H
