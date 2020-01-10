#ifndef FFMEGEDECODER_H
#define FFMEGEDECODER_H

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
}
#include <sstream>
#include "ffmpegcodec.h"
#include "ffmpegpacket.h"

#include "threadsafe_queue_buffer.h"

class FfmpegDecoder
{
    AVFormatContext* m_format_cont;

    ThreadSafeQueueBuffer* m_buffer;
    AVIOContext *m_avio_ctx;
    uint8_t *m_avio_buf;
public:
    FfmpegDecoder(ThreadSafeQueueBuffer& buffer);
    ~FfmpegDecoder();

    void flush();
    bool init(std::ostringstream& error_stream,int buffer_size = 4096, int probe_min = 1000);
    AVFormatContext* getFormatContex();
    bool initFormat(std::ostringstream& error_stream);
    bool findStreamInfo(int& result);
    bool findDecoder(unsigned int indexStream, FfmpegCodec& codec);
    bool readNextPacket(FfmpegPacket& packet);
};

#endif // FFMEGEDECODER_H
