#include "ffmpegdecoder.h"
#include <chrono>
#include <thread>
#include <iostream>


FfmpegDecoder::FfmpegDecoder(ThreadSafeQueueBuffer& buffer) :m_format_cont(NULL), m_buffer(&buffer), m_avio_ctx(NULL), m_avio_buf(NULL)
{
}

void FfmpegDecoder::flush()
{

    if (m_avio_ctx) {
        if (m_avio_ctx->buffer) {
            av_freep(&m_avio_ctx->buffer);
        }
        av_freep(&m_avio_ctx);
    }
    if (m_format_cont)
        avformat_close_input(&m_format_cont);
}

FfmpegDecoder::~FfmpegDecoder()
{
    flush();
}

AVFormatContext* FfmpegDecoder::getFormatContex()
{
    return m_format_cont;
}

bool FfmpegDecoder::initFormat(std::ostringstream& error_stream)
{
    int ret = avformat_open_input(&m_format_cont, NULL, NULL, NULL);
    if (ret < 0){
        error_stream<<"(FfmpegDecoder::initFormat) Could not open input format"<< '\n';
        return false;
    }
    return true;
}

bool FfmpegDecoder::findStreamInfo(int& result)
{

    if (avformat_find_stream_info(m_format_cont, NULL) < 0)
        return false;
    result = m_format_cont->nb_streams;
    return true;
}

bool FfmpegDecoder::findDecoder(unsigned int indexStream, FfmpegCodec& fcodec)
{
    if (indexStream<0 || indexStream>m_format_cont->nb_streams)
        return false;
    if (!fcodec.init(m_format_cont->streams[indexStream]->codecpar->codec_id, true))
        return false;

    if (avcodec_parameters_to_context(fcodec.getPtr(), m_format_cont->streams[indexStream]->codecpar) < 0) {
        return false;
    }
    if (!fcodec.open())
        return false;
    return true;

}

bool FfmpegDecoder::readNextPacket(FfmpegPacket & fpacket)
{

    AVPacket* packet = fpacket.getPtr();
    if (packet == NULL)
        return false;
    if (av_read_frame(m_format_cont, packet) != 0)
        return false;
    return true;
}

int read_packet_callback(void *opaque, uint8_t *buf, int buf_size)
{
    ThreadSafeQueueBuffer* qb = (ThreadSafeQueueBuffer *) opaque;
    if (qb == NULL)
         return  AVERROR(ENOMEM);
    qb->wait();

    if (qb->isError())
        return AVERROR(ENOMEM); // TODO return some other error?
    if (qb->isEOF())
        return AVERROR_EOF;

    size_t ret = qb->read(buf,buf_size);
    return ret;
}

int64_t seek_callback(void *opaque, int64_t offset, int whence) {
    whence &= ~AVSEEK_FORCE;

    if (whence != SEEK_CUR && whence != SEEK_SET)
        return AVERROR(EINVAL);

    ThreadSafeQueueBuffer* qb = (ThreadSafeQueueBuffer *) opaque;
    if (whence == SEEK_CUR)
        return qb->seek_cur(offset);
    if (whence == SEEK_SET)
        return qb->seek_set(offset);
    return AVERROR(EINVAL);
}

bool FfmpegDecoder::init(std::ostringstream& error_stream,int buffer_size, int probe_min)
{
    flush();
//    avformat_network_init();
    if (!(m_format_cont = avformat_alloc_context())) {

        error_stream<<"(FfmpegDecoder::init) Could not alloc AVFormatContext" << '\n';
        return false;
    }
    if (probe_min > 0)
        m_format_cont->probesize = probe_min;
//    if (m_buffer)
//        m_buffer->flush();
    size_t avio_ctx_buf_size = buffer_size > 0 ? buffer_size : 4096;
    m_avio_buf = (uint8_t*)av_malloc(avio_ctx_buf_size);
    if (!m_avio_buf) {
        error_stream<<"(FfmpegDecoder::init) Could not alloc avio buffer"<< '\n';
        return false;
    }
    m_avio_ctx = avio_alloc_context(m_avio_buf, avio_ctx_buf_size, 0, m_buffer, &read_packet_callback, NULL, &seek_callback);
    if (!m_avio_ctx) {
        error_stream << "(FfmpegDecoder::init) Could not alloc AVIOContext"<< '\n';
        return false;
    }
    m_format_cont->pb = m_avio_ctx;
    return true;
}

