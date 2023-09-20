/**
 * @file memory_stream.h
 * @author rpvos (mr.rv.asd@gmail.com)
 * @brief Library used to test max485ttl.cpp
 * @version 0.1
 * @date 2023-09-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MEMORY_STREAM_H_
#define MEMORY_STREAM_H_

#include <Stream.h>

class MemoryStream : public Stream
{
public:
    MemoryStream(uint8_t buffersize = 64);
    ~MemoryStream();

    virtual int available() { return input_length_; };

    virtual int read();

    virtual int peek();

    virtual size_t write(uint8_t c);

private:
    uint8_t *buffer_;

    uint8_t buffer_size_;

    uint8_t write_cursor_;

    uint8_t read_cursor_;

    uint8_t input_length_;
};

#endif // MEMORY_STREAM_H_