#ifndef THREADSAFE_QUEUE_BUFFER_H
#define THREADSAFE_QUEUE_BUFFER_H

#include <type_traits>
#include <mutex>
#include <condition_variable>

#include "queue_buffer.h"

class ThreadSafeQueueBuffer : public QueueBuffer {
public:
    /**
     * @brief wait until there is some data in the queue, or end of file or error
     */
    virtual void wait() = 0;
};

template<typename T>
class ThreadSafeQueueBufferWrapper final : public ThreadSafeQueueBuffer {
    static_assert(std::is_base_of<QueueBuffer, T>::value,
                  "ThreadSafeQueueBufferWrapper requires class implementing QueueBuffer interface");
    static_assert(!std::is_abstract<T>::value,
                  "ThreadSafeQueueBufferWrapper requires non-abstract class implementing QueueBuffer interface");

private:
    T *m_buffer;
    std::mutex m_mutex;
    std::condition_variable m_cond_var;

    bool wait_predicate();

public:
    ThreadSafeQueueBufferWrapper();

    size_t put(const unsigned char * const buf, const size_t size) override;
    size_t read(unsigned char * const buf, const size_t size) override;
    size_t available() override;
    bool isEOF() override;
    void putEOF() override;
    bool isError() override;
    void setError() override;

    void wait() override;

    int64_t seek_set(int64_t position) override;

    int64_t seek_cur(int64_t offset) override;

    ~ThreadSafeQueueBufferWrapper();
};

template<typename T>
ThreadSafeQueueBufferWrapper<T>::ThreadSafeQueueBufferWrapper() : m_mutex(), m_cond_var() {
    m_buffer = new T();
}

template<typename T>
ThreadSafeQueueBufferWrapper<T>::~ThreadSafeQueueBufferWrapper() {
    delete m_buffer;
}

template<typename T>
size_t ThreadSafeQueueBufferWrapper<T>::put(const unsigned char * const buf, const size_t size) {
    size_t result;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        result = m_buffer->put(buf, size);
    }
    m_cond_var.notify_all();
    return result;
}

template<typename T>
size_t ThreadSafeQueueBufferWrapper<T>::read(unsigned char * const buf, const size_t size) {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_buffer->read(buf, size);
}

template<typename T>
size_t ThreadSafeQueueBufferWrapper<T>::available() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_buffer->available();
}

template<typename T>
bool ThreadSafeQueueBufferWrapper<T>::isEOF() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_buffer->isEOF();
}

template<typename T>
void ThreadSafeQueueBufferWrapper<T>::putEOF() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_buffer->putEOF();
    }
    m_cond_var.notify_all();
}

template<typename T>
bool ThreadSafeQueueBufferWrapper<T>::isError() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_buffer->isError();
}

template<typename T>
void ThreadSafeQueueBufferWrapper<T>::setError() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_buffer->setError();
    }
    m_cond_var.notify_all();
}

template<typename T>
void ThreadSafeQueueBufferWrapper<T>::wait() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond_var.wait(lock, [this]{ return wait_predicate(); });
}

template<typename T>
bool ThreadSafeQueueBufferWrapper<T>::wait_predicate() {
    return m_buffer->isError() || m_buffer->available() > 0 || m_buffer->isEOF();
}

template<typename T>
int64_t ThreadSafeQueueBufferWrapper<T>::seek_set(int64_t position) {
    int64_t res;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        res = m_buffer->seek_set(position);
    }
    m_cond_var.notify_all();
    return res;
}

template<typename T>
int64_t ThreadSafeQueueBufferWrapper<T>::seek_cur(int64_t offset) {
    int64_t res;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        res = m_buffer->seek_cur(offset);
    }
    m_cond_var.notify_all();
    return res;
}


#endif // THREADSAFE_QUEUE_BUFFER_H

