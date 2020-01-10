#include "cyclic_buffer.h"

Cyclic_buffer::Cyclic_buffer(int size)
{
    buffer = new uint8_t[size];
    r_point = 0;
    w_point = 0;
    wp_right = true;
    buf_size = size;
    is_empty = true;
}

Cyclic_buffer::~Cyclic_buffer()
{
    delete [] buffer;
}

int Cyclic_buffer::resize()
{
    int new_size = 1024 + buf_size;
    uint8_t* tmp_buffer = new uint8_t[new_size];
    if (!wp_right) {
        memcpy(tmp_buffer, buffer + r_point, buf_size - r_point);
        memcpy(tmp_buffer + buf_size - r_point, buffer, w_point);
        w_point = buf_size - r_point + w_point;
        r_point = 0;
        wp_right = true;
    } else {
        memcpy(tmp_buffer, buffer + r_point, w_point - r_point);
        r_point = 0;
        w_point = w_point - r_point;
    }
    delete [] buffer;
    buf_size = new_size;
    buffer = new uint8_t[new_size];
    memcpy(buffer, tmp_buffer, w_point);
    delete [] tmp_buffer;
    return buf_size;
}

int Cyclic_buffer::writeToBuff(uint8_t* data, size_t size)
{
    if (wp_right) {
        if (size <= buf_size - w_point + r_point) {
            if (size > buf_size - w_point) {
                wp_right = !wp_right;
                memcpy(buffer + w_point, data, buf_size - w_point);
                memcpy(buffer, data + buf_size - w_point, size - (buf_size - w_point));
                w_point = size - (buf_size - w_point);
            } else {
                memcpy(buffer + w_point, data, size);
                w_point += size;
            }
        } else {
            resize();
            writeToBuff(data, size);
        }
    } else {
        if (size <= r_point - w_point) {
            memcpy(buffer + w_point, data, size);
            w_point += size;
        } else {
            resize();
            writeToBuff(data, size);
        }
    }
    is_empty = false;

    return size;
}

int Cyclic_buffer::readFromBuff(uint8_t *dest, size_t size)
{
    if (is_empty) {
        return 0;
    }
    if (wp_right) {
        if  (size <= w_point - r_point) {
            memcpy(dest, buffer + r_point, size);
            r_point += size;
        } else {
            memcpy(dest, buffer + r_point, size = w_point - r_point);
            r_point = w_point;
            is_empty = true;
        }
    } else {
        if (size <= buf_size - r_point + w_point) {
            if (size > buf_size - r_point){
                wp_right = !wp_right;
                memcpy(dest, buffer + r_point, buf_size - r_point);
                memcpy(dest + buf_size - r_point, buffer, size = size - (buf_size - r_point));
                r_point = size;
            } else {
                memcpy(dest, buffer+r_point, size);
                r_point += size;
            }
        } else {
            memcpy(dest, buffer + r_point, buf_size - r_point);
            memcpy(dest + buf_size - r_point, buffer, w_point);
            size = buf_size - r_point + w_point;
            r_point = w_point;
            is_empty = true;
        }
    }

    return size;
}

int Cyclic_buffer::getRpoint()
{
    return r_point;
}

int Cyclic_buffer::getWpoint()
{
    return w_point;
}

int Cyclic_buffer::getBufSize()
{
    return buf_size;
}

int Cyclic_buffer::getBufDataSize()
{
    int data_size = 0;
    if (wp_right){
        data_size = w_point - r_point;
    } else {
        data_size = buf_size - r_point + w_point;
    }
    return data_size;
}


int Cyclic_buffer::getFlag()
{
    return wp_right;
}


