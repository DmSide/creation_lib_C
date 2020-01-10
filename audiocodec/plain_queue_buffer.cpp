#include "plain_queue_buffer.h"

#include <new>
#include <cstring>

PlainQueueBuffer::PlainQueueBuffer() : m_buffer(nullptr), m_size(0), m_offset(0), m_eof(false), m_error(false) {}

PlainQueueBuffer::~PlainQueueBuffer() {
    delete[] m_buffer;
}

size_t PlainQueueBuffer::available() {
    return m_size - m_offset;
}

bool PlainQueueBuffer::isEOF() {
    return m_eof && (available() == 0); // TODO m_size - m_offset instead of available()? there is no difference as long as PlainQueueBuffer is final
}

void PlainQueueBuffer::putEOF() {
    m_eof = true;
}

bool PlainQueueBuffer::isError() {
    return m_error;
}

void PlainQueueBuffer::setError() {
    m_error = true;
}

size_t PlainQueueBuffer::put(const unsigned char * const buf, const size_t size) {
    size_t available = m_size - m_offset;
    size_t new_size = size + available;
    unsigned char* buffer = new (std::nothrow) unsigned char[new_size];
    if (!buffer)
        return 0;
    if (m_buffer)
    {
        std::memcpy(buffer, m_buffer + m_offset, available);
        delete[] m_buffer;
    }
    std::memcpy(buffer + available, buf, size);
    m_buffer = buffer;
    m_size = new_size;
    m_offset = 0;
    return size;
}

size_t PlainQueueBuffer::read(unsigned char * const buf, const size_t size) {
    size_t available = m_size - m_offset;
    if (available == 0 || !m_buffer)
        return 0;
    size_t read_size = (available < size) ? available : size;
    std::memcpy(buf, m_buffer + m_offset, read_size);
    m_offset += read_size;
    return read_size;
}


