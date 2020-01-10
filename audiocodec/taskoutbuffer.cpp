#include "taskoutbuffer.h"
#include <cstring>
#include <new>


TaskOutBuffer::TaskOutBuffer() :m_size(0), m_offset(0), m_buffer(NULL)
{
}

TaskOutBuffer::~TaskOutBuffer()
{
    if (m_buffer)
        delete[] m_buffer;
}

size_t TaskOutBuffer::available()
{
    return  m_size - m_offset;
}

bool TaskOutBuffer::append(unsigned char* data, size_t size)
{
    size_t available = m_size - m_offset;
    size_t new_size = size + available;
    unsigned char* buffer = new (std::nothrow) unsigned char[new_size];
    if (!buffer)
        return false;
    if (m_buffer)
    {
        memcpy(buffer, m_buffer + m_offset, available);
        delete[] m_buffer;
    }
    memcpy(buffer + available, data, size);
    m_buffer = buffer;
    m_size = new_size;
    m_offset = 0;
    return true;
}

size_t TaskOutBuffer::read(unsigned char* data, size_t size)
{
    size_t available = m_size - m_offset;
    if (available == 0 || !m_buffer)
        return 0;
    size_t read_size = (available < size) ? available : size;
    memcpy(data, m_buffer+ m_offset, read_size);
    m_offset += read_size;
    return read_size;
}

unsigned char * TaskOutBuffer::data()
{
    if(!m_buffer)
        return NULL;
    return m_buffer+m_offset;
}

void TaskOutBuffer::skip(size_t size)
{
    m_offset += size;
    m_offset = m_offset < m_size ? m_offset : m_size;
}

void TaskOutBuffer::setEOF()
{
    m_offset = m_size;
}

