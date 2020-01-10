#include "whole_plain_queue_buffer.h"

#include <new>
#include <cstring>

WholePlainQueueBuffer::WholePlainQueueBuffer() : m_buffer(nullptr), m_size(0), m_offset(0), m_eof(false), m_error(false) {}

WholePlainQueueBuffer::~WholePlainQueueBuffer() {
    delete[] m_buffer;
}

size_t WholePlainQueueBuffer::available() {
    return m_size - m_offset;
}

bool WholePlainQueueBuffer::isEOF() {
    return m_eof && (available() == 0); // TODO m_size - m_offset instead of available()? there is no difference as long as PlainQueueBuffer is final
}

void WholePlainQueueBuffer::putEOF() {
    m_eof = true;
}

bool WholePlainQueueBuffer::isError() {
    return m_error;
}

void WholePlainQueueBuffer::setError() {
    m_error = true;
}

size_t WholePlainQueueBuffer::put(const unsigned char * const buf, const size_t size) {
    size_t new_size = size + m_size;
    unsigned char* buffer = new (std::nothrow) unsigned char[new_size];
    if (!buffer)
        return 0;
    if (m_buffer)
    {
        std::memcpy(buffer, m_buffer, m_size);
        delete[] m_buffer;
    }
    std::memcpy(buffer + m_size, buf, size);
    m_buffer = buffer;
    m_size = new_size;
//    m_offset = 0;
    return size;
}

size_t WholePlainQueueBuffer::read(unsigned char * const buf, const size_t size) {
    size_t available = m_size - m_offset;
    if (available == 0 || !m_buffer)
        return 0;
    size_t read_size = (available < size) ? available : size;
    std::memcpy(buf, m_buffer + m_offset, read_size);
    m_offset += read_size;
    return read_size;
}

int64_t WholePlainQueueBuffer::seek_set(int64_t position) {
    if (position < m_size) {
        m_offset = position;
        return m_offset;
    }
    return AVERROR(EINVAL);
}

int64_t WholePlainQueueBuffer::seek_cur(int64_t offset) {
    if (offset < 0 && -offset > m_offset) {
        m_offset = 0;
        return m_offset;
    }
    size_t newPos = m_offset + offset;
    if (newPos < m_size) {
        m_offset = newPos;
        return m_size;
    }
    return AVERROR(EINVAL);
}



