#include "synctask.h"


SyncTask::SyncTask(TaskStartModule& start):m_start(&start), m_condition(SyncTaskCondition::S_T_NONE),m_thread(NULL)
{
}

SyncTask::~SyncTask()
{
    join();
}

void SyncTask::join()
{
    if (m_thread) {
        m_thread->join();
        m_thread = NULL;
    }
}

void run_callback(SyncTaskCondition* pointCondition, TaskStartModule* start,std::ostringstream* stream)
{
    *pointCondition= SyncTaskCondition::S_T_RUN;
    if (start != NULL) {
        try {
            if (start->process(*stream)) {
                *pointCondition = SyncTaskCondition::S_T_COMPLETE;
                return;
            }
        }
        catch (...)
        {

        }
    }
    *pointCondition = SyncTaskCondition::S_T_ERROR;
}

bool SyncTask::run()
{
    join();
    m_stream.str("");
    m_condition= SyncTaskCondition::S_T_RUN;
    m_thread = new (std::nothrow) std::thread(run_callback, &m_condition, m_start,&m_stream);
    if(m_thread==NULL)
        m_condition=SyncTaskCondition::S_T_ERROR;
    return m_thread != NULL;
}

 std::ostringstream * SyncTask::error_stream(){
    return &m_stream;

}

SyncTaskCondition SyncTask::condition()
{
    return m_condition;
}
