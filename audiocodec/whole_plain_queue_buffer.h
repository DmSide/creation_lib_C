#ifndef WHOLE_PLAIN_QUEUE_BUFFER_H
#define WHOLE_PLAIN_QUEUE_BUFFER_H

#include "queue_buffer.h"

/**
 * @brief Implmentation of the QueueBuffer interface that retains the whole file
 *
 */
class WholePlainQueueBuffer final : public QueueBuffer {
private:
    unsigned char * m_buffer;
    size_t m_size;
    size_t m_offset;
    bool m_eof;
    bool m_error;

public:
    WholePlainQueueBuffer();

    size_t put(const unsigned char * const buf, const size_t size) override;
    size_t read(unsigned char * const buf, const size_t size) override;
    size_t available() override;
    bool isEOF() override;
    void putEOF() override;
    bool isError() override;
    void setError() override;

    ~WholePlainQueueBuffer();

    int64_t seek_set(int64_t position) override;

    int64_t seek_cur(int64_t offset) override;
};

#endif // WHOLE_PLAIN_QUEUE_BUFFER_H

