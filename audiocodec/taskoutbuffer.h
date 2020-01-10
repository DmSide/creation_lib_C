#ifndef TASKOUTBUFFER_H
#define TASKOUTBUFFER_H
#include <stdlib.h>


/**
 * @brief The TaskOutBuffer class Represents the output buffer of one module and the input for another
 */
class TaskOutBuffer {

    size_t m_size;
    size_t m_offset;
    unsigned char* m_buffer;


public:
    TaskOutBuffer();
    ~TaskOutBuffer();

    /**
     * @brief available
     * @return How much data can be read from this buffer
     */
    size_t available();
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
     * @retrun Size of read data
     */
    size_t read(unsigned char* data, size_t size);
    /*
     * @brief data Returns a pointer to unread data.
     */
    unsigned char* data();
    /**
     * @brief skip Skips a certain amount of data in the buffer
     * @param size Amount of missing data
     */
    void skip(size_t size);

    /**
     * @brief setEOF Skips all free data in the buffer
     */
    void setEOF();

};

#endif // TASKOUTBUFFER_H
