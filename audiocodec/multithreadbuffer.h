#ifndef MULTITHREADBUFFER_H
#define MULTITHREADBUFFER_H

#include <mutex>

/**
 * @brief The MultiThreadBufferReadStatus struct-Structure describing the result of reading from MultiThreadBuffer
 */
struct MultiThreadBufferReadStatus
{
    /**
     * @brief size Read data size
     */
    size_t size=0;
    /**
     * @brief IsEOF No more data is expected from this buffer.
     * Exposed true if only buffer is empty
     */
    bool IsEOF=false;
    /**
     * @brief IsError An error occurred during the readings.
     * Exposed true if only buffer is empty.
     */
    bool IsError=false;
};

/**
 * @brief Class MultiThreadBuffer - describes a thread-safe clipboard between threads
 */
class MultiThreadBuffer
{
private:
    unsigned char* m_buffer;
    size_t m_size;
    size_t m_offset;
    size_t m_next_size;
    std::mutex m_mutex;
    bool m_isEOF;
    bool m_isError;
public:
    MultiThreadBuffer();
    ~MultiThreadBuffer();
    /**
     * @brief nextSize
     * @return  the size of the data that the read stream requested but did not receive.
     */
    size_t nextSize();
    /**
     * @brief data Returns a pointer to unread data.
     * Non-thread safe method.
     * It is not recommended to use while there is another running thread working with this buffer.
     * @return pointer to unread data
     */
    unsigned char* data();
    /**
     * @brief setEOF Indicates that the write stream can no longer provide data to this buffer.
     * The reading thread only learns about this when it considers all available data in the MultiThreadBufferReadStatus structure of the IsEOF field.
     */
    void setEOF();
    /**
     * @brief setIsError Indicates that an error occurred in the write stream.
     * The reading thread only finds out about this when it considers all available data in the MultiThreadBufferReadStatus structure of the IsError field.
     */
    void setIsError();
    /**
     * @brief append-Adds new data to the buffer.
     * @param data-Pointer to new data
     * @param size-Size of new data
     * @return True if all is successful. False if not enough memory
     */
    bool append(unsigned char* data, size_t size);
    /**
     * @brief read Tries to read data from the buffer.
     * @param data Pointer to destination buffer
     * @param size Readable size
     * @param status Reading status
     */
    void read(unsigned char * data, size_t size,MultiThreadBufferReadStatus& status);
    /**
     * @brief available
     * @return The size of the data to be read at a given time.
     */
    size_t available();
    /**
     * @brief flush Flush buffer
     */
    void flush();
};

#endif // MULTITHREADBUFFER_H
