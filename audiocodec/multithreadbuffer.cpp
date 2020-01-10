#include "multithreadbuffer.h"
#include <cstring>


MultiThreadBuffer::MultiThreadBuffer()
{
    m_isEOF = false;
    m_isError = false;
    m_buffer = NULL;
    m_size = 0;
    m_offset = 0;
    m_next_size = 0;
}

MultiThreadBuffer::~MultiThreadBuffer()
{
    flush();
}

size_t MultiThreadBuffer::nextSize()
{
    return m_next_size;
}

unsigned char * MultiThreadBuffer::data()
{
    if (m_buffer)
        return m_buffer + m_offset;
    return NULL;
}

void MultiThreadBuffer::setEOF()
{
    m_mutex.lock();
    m_isEOF = true;
    m_next_size = 0;
    m_mutex.unlock();
}

void MultiThreadBuffer::setIsError()
{
    m_mutex.lock();
    m_isError = true;
    m_mutex.unlock();
}

bool MultiThreadBuffer::append(unsigned char* data, size_t size)
{
    m_mutex.lock();
    m_isEOF = false;
    size_t available = m_size - m_offset;
    size_t new_size = available + size;
    unsigned char* buffer = new (std::nothrow) unsigned char[new_size];
    if (buffer == NULL) {
        m_mutex.unlock();
        m_isError = true;
        return false;
    }
    if (m_buffer == NULL) {
        memcpy(buffer, data, size);
    }
    else {
        memcpy(buffer, m_buffer + m_offset, available);
        memcpy(buffer + available, data, size);
        delete[] m_buffer;
    }
    m_buffer = buffer;
    m_size = new_size;
    m_offset = 0;
    if (m_next_size > size)
    {
        m_next_size = m_next_size - size;

    }
    else {
        m_next_size = 0;
    }
    m_mutex.unlock();
    return true;
}

void  MultiThreadBuffer::read(unsigned char * data, size_t size,MultiThreadBufferReadStatus& status)
{
    m_mutex.lock();
    size_t available = m_size - m_offset;
    size_t read_size = size > available ? available : size;
    memcpy(data, m_buffer + m_offset, read_size);
    m_offset += read_size;
    if (read_size <= size)
    {
        if(m_isEOF== false && m_isError == false){
            m_next_size = size - read_size;
        }else{
         status.IsEOF = m_isEOF;
         status.IsError = m_isError;
        }
    }
    status.size = read_size;

    m_mutex.unlock();
}

size_t MultiThreadBuffer::available()
{

    m_mutex.lock();
    size_t result = m_size - m_offset;
    m_mutex.unlock();
    return result;
}

void MultiThreadBuffer::flush()
{
    m_mutex.lock();
    delete[] m_buffer;
    m_isEOF = false;
    m_isError = false;
    m_buffer = NULL;
    m_next_size = 0;
    m_offset = 0;
    m_mutex.unlock();
}

