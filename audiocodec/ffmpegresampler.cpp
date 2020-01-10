#include "ffmpegresampler.h"
extern "C"
{
    #include <libavutil/opt.h>
}
#include <new>

FfmpegResampler::FfmpegResampler():m_swr(NULL)
{
}


FfmpegResampler::~FfmpegResampler()
{
    flush();
}

void FfmpegResampler::flush()
{
    if (m_swr) {
        swr_free(&m_swr);
        m_swr = NULL;
    }
}

bool FfmpegResampler::init(int channel, int rate_of_sample, FfmpegCodec & fcodec)
{
    flush();
    AVCodecContext* codec = fcodec.getPtr();
    if (channel == 0 || codec ==NULL) {
        return false;
    }

    m_swr = swr_alloc();
    if (m_swr == NULL)
        return false;
    av_opt_set_int(m_swr, "in_channel_count", codec->channels, 0);
    av_opt_set_int(m_swr, "out_channel_count", channel, 0);
    av_opt_set_int(m_swr, "in_channel_layout", codec->channel_layout, 0);
    av_opt_set_int(m_swr, "out_channel_layout", av_get_default_channel_layout(channel), 0);
    av_opt_set_int(m_swr, "in_sample_rate", codec->sample_rate, 0);
    av_opt_set_int(m_swr, "out_sample_rate", rate_of_sample, 0);
    av_opt_set_sample_fmt(m_swr, "in_sample_fmt", codec->sample_fmt, 0);
    av_opt_set_sample_fmt(m_swr, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    if (swr_init(m_swr)<0 || !swr_is_initialized(m_swr)) {
        return false;
    }
    m_src_sample_rate = codec->sample_rate;
    dst_sample_rate = rate_of_sample;
    return true;
}

// FIXME: see for example https://www.ffmpeg.org/doxygen/4.0/resampling__audio_8c_source.html

unsigned char* FfmpegResampler::resampleFrame(FfmpegPacket & fpacket, FfmpegFrame & fframe, size_t& size)
{
    AVPacket* packet = fpacket.getPtr();
    AVFrame* frame = fframe.getPtr();
    if (packet == NULL || frame == NULL)
        return NULL;
    uint8_t **dst_data = NULL;
    int flush = packet->buf == NULL?0:frame->nb_samples;
    int dst_nb_samples = av_rescale_rnd(swr_get_delay(m_swr, m_src_sample_rate)+flush, dst_sample_rate, /*44100*/m_src_sample_rate, AV_ROUND_UP);
    if (dst_nb_samples == 0)
    {
        dst_nb_samples = 1;
    }
    if (av_samples_alloc_array_and_samples(&dst_data, NULL, 1, dst_nb_samples, AV_SAMPLE_FMT_S16, 0) < 0)
    {
        return NULL;
    }
    int ret = swr_convert(m_swr, dst_data, dst_nb_samples,flush==0?NULL: (const uint8_t **)frame->data,flush);
    if (ret < 0) {
        if (dst_data) {
            av_freep(&dst_data[0]);
        }
        av_freep(&dst_data);
        return NULL;
    }
    if (ret == 0)
    {
        size = 0;
    } else {
        size = av_samples_get_buffer_size(NULL, 1, ret, AV_SAMPLE_FMT_S16, 1);
    }
    unsigned char* data = new (std::nothrow) unsigned char[size];
    if (data)
    {
        memcpy(data,dst_data[0], size);
    }
    if (dst_data)
        av_freep(&dst_data[0]);
    av_freep(&dst_data);
    return data;
}

bool FfmpegResampler::resample_frame_before_encode(FfmpegFrame& fframe, FfmpegFrame& outf)
{
    AVFrame* frame = &fframe == NULL ? NULL : fframe.getPtr();
//    if (frame == NULL)
//        return false;
    uint8_t **dst_data = NULL;
    int flush = frame == NULL ? 0 : frame->nb_samples;
    int dst_nb_samples = av_rescale_rnd(swr_get_delay(m_swr, m_src_sample_rate)+flush, dst_sample_rate, /*44100*/m_src_sample_rate, AV_ROUND_UP);
    if (av_samples_alloc_array_and_samples(&dst_data, NULL, 1, dst_nb_samples, AV_SAMPLE_FMT_S16, 0) < 0)
        return false;
    int nb_converted_samples = swr_convert(m_swr, dst_data, dst_nb_samples, frame == NULL ? 0 : (const uint8_t **)frame->data,flush);
    if (nb_converted_samples < 0) {
        if (dst_data) {
            av_freep(&dst_data[0]);
        }
        av_freep(&dst_data);
        return false;
    }
    size_t size = av_samples_get_buffer_size(NULL, 1, nb_converted_samples, AV_SAMPLE_FMT_S16, 1);
//    unsigned char* data = new (std::nothrow) unsigned char[size];
//    if (data)
//    {
//        memcpy(data,dst_data[0], size);
//    }


    AVFrame* o_frame = outf.getPtr();
    if (o_frame == NULL)
        return false;

    o_frame->nb_samples = nb_converted_samples;
    o_frame->channel_layout = AV_CH_LAYOUT_MONO;
    o_frame->format = AV_SAMPLE_FMT_S16;
    int ret = av_frame_get_buffer(o_frame, 0);
    if (ret < 0)
        return false;
    memcpy(outf.data(0), dst_data[0], size);

    if (dst_data)
        av_freep(&dst_data[0]);
    av_freep(&dst_data);
    return true;
}

