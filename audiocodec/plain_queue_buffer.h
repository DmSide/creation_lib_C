#ifndef PLAIN_QUEUE_BUFFER_H
#define PLAIN_QUEUE_BUFFER_H

#include "queue_buffer.h"

/**
 * @brief Implmentation of the QueueBuffer interface
 *
 */
class PlainQueueBuffer final : public QueueBuffer {
private:
    unsigned char * m_buffer;
    size_t m_size;
    size_t m_offset;
    bool m_eof;
    bool m_error;

public:
    PlainQueueBuffer();

    size_t put(const unsigned char * const buf, const size_t size) override;
    size_t read(unsigned char * const buf, const size_t size) override;
    size_t available() override;
    bool isEOF() override;
    void putEOF() override;
    bool isError() override;
    void setError() override;

    ~PlainQueueBuffer();
};

#endif // PLAIN_QUEUE_BUFFER_H

