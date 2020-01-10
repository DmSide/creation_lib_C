#ifndef QUEUE_BUFFER_H
#define QUEUE_BUFFER_H

#include <cstddef>
#include <cstdint>

extern "C" {
    #include <libavutil/error.h>
}

class QueueBuffer {
public:

    /**
     * @brief appends the contents of the buffer buf of the size size to the end of the buffer
     * @param buf - buffer to be appended
     * @param size - size of the buf
     * @return number of bytes written into QueueBuffer (usually 0 or size)
     */
    virtual size_t put(const unsigned char * const buf, const size_t size) = 0;

    /**
     * read at most size bytes from the QueueBuffer into buffer buf and returns how much bytes were read this way.
     * @param buf - buffer to be appended
     * @param size - size of the buf
     * @return number of bytes read from QueueBuffer (but not more than size)
     */
    virtual size_t read(unsigned char * const buf, const size_t size) = 0;

    /**
     * @brief
     * @return how many bytes can be read right now
     */
    virtual size_t available() = 0;

    /**
     * @brief isEOF
     * @return true if the whole content of the QueueBuffer was already read (no data remaining) and putEOF() was called on this buffer
     */
    virtual bool isEOF() = 0;
    /**
     * @brief putEOF
     * Puts about the end of the data to the buffer. You can only read from the buffer starting from now (no new data can be added).
     * When all remaining data will be read,
     */
    virtual void putEOF() = 0;

    // todo set and return error codes instead of simply bool?
    /**
     * @brief isError
     * @return true if setError() was called on this buffer.
     */
    virtual bool isError() = 0;
    /**
     * @brief setError
     * isError will return true after this call
     */
    virtual void setError() = 0;

    virtual ~QueueBuffer() {}

    virtual int64_t seek_set(int64_t position) {
        return AVERROR(EINVAL);
    }

    virtual int64_t seek_cur(int64_t offset) {
        return AVERROR(EINVAL);
    }
};

#endif // QUEUE_BUFFER_H

