#include "ffmpegencodermodule.h"
extern "C"
{
    #include <libavformat/avformat.h>
}

bool FfmpegEncoderModule::get_last_frame_for_encode(TaskOutBuffer& buffer, FfmpegFrame& last_frame,std::ostringstream& error_stream)
{
    int buf_data_size = buffer.available();
    AVFrame* l_frame = last_frame.getPtr();
    l_frame->channels = m_codec.channels();
    l_frame->channel_layout = av_get_default_channel_layout(l_frame->channels);
    l_frame->format = AV_SAMPLE_FMT_S16;
    l_frame->nb_samples = buf_data_size/m_bytesPerSample;
    m_nb_samples = l_frame->nb_samples;
    int ret = av_frame_get_buffer(l_frame, 0);
    if (ret < 0){
        error_stream<<"(FfmpegEncoderModule::get_last_frame_for_encode) error av_frame_get_buffer"<<std::endl;
        return false;
    }
    if (!last_frame.makeWritable()){

        error_stream<<"(FfmpegEncoderModule::get_last_frame_for_encode) error makeWritable"<<std::endl;
        return false;
    }
    for (int i = 0; i != m_nb_samples; i++)
    {
        for (int ch = 0; ch < m_channels; ch++)
        {
            int readSize = buffer.read(last_frame.data(ch) + m_bytesPerSample * i, m_bytesPerSample);
            if (readSize != m_bytesPerSample)
            {
                error_stream<<"(FfmpegEncoderModule::get_last_frame_for_encode) error read data"<<std::endl;
                return false;
            }
        }
    }
    return true;
}

bool FfmpegEncoderModule::readFrame(TaskOutBuffer & buffer,std::ostringstream& error_stream)
{
    if (!m_frame.makeWritable()){
        error_stream<<"(FfmpegEncoderModule::readFrame) error makeWritable"<<std::endl;
        return false;
    }
    for (int i = 0; i != m_nb_samples; i++)
    {
        for (int ch = 0; ch < m_channels; ch++)
        {
            int readSize = buffer.read(m_frame.data(ch) + m_bytesPerSample * i, m_bytesPerSample);
            if (readSize != m_bytesPerSample)
            {
                  error_stream<<"(FfmpegEncoderModule::readFrame) error read data"<<std::endl;
                return false;
            }
        }
    }
    return true;
}


bool FfmpegEncoderModule::encodeFrame(FfmpegFrame* frame,std::ostringstream& error_stream)
{
    if (on_resample) {
        FfmpegFrame outf;
        if(!outf.init())
        {
            error_stream<<"(FfmpegEncoderModule::encodeFrame) Could not allocate frame"<<std::endl;
            return false;
        }
        m_resampler.resample_frame_before_encode(*frame, outf);
        if (!m_codec.sendFrame(&outf)) {

            error_stream<<"(FfmpegEncoderModule::encodeFrame) Error sending the frame to the encoder"<<std::endl;
            return false;
        }
    } else {

        /* send the frame for encoding */
        if (!m_codec.sendFrame(frame)) {
            error_stream<<"(FfmpegEncoderModule::encodeFrame) Error sending the frame to the encoder"<<std::endl;
            return false;
        }
    }
    /* read all the available output packets (in general there may be any
     * number of them */
    int ret = 0;
    while (ret >= 0) {
        ret = m_codec.receivePacket(m_packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;

        else if (ret < 0) {
             error_stream<<"(FfmpegEncoderModule::encodeFrame) Error encoding audio frame"<<std::endl;
            return false;
        }
        out(m_packet.data(), m_packet.size(),error_stream);
        m_packet.unref();
    }
    return true;
}

FfmpegEncoderModule::FfmpegEncoderModule(TaskJoinModule* next, bool rnnoise_on):TaskAppendModule(next)
{
    if (rnnoise_on)
    {
        on_resample = true;
        init(AV_CODEC_ID_PCM_S16LE, AV_SAMPLE_FMT_S16, 48000);
    } else {
        on_resample = false;
        init(AV_CODEC_ID_PCM_S16LE, AV_SAMPLE_FMT_S16, 16000);
    }
}

FfmpegEncoderModule::~FfmpegEncoderModule()
{
}

bool FfmpegEncoderModule::init(AVCodecID id, AVSampleFormat sampleFormat,int sampleRate)
{
    avformat_network_init();
    if (!m_codec.init(id, false))
        return false;
    if (!m_codec.setSampleFormat(sampleFormat))
        return false;
    if (!m_codec.setSampleRate(sampleRate))
        return false;
    if (!m_codec.setChannelLayout(AV_CH_LAYOUT_MONO))
        return false;
    if (!m_codec.open())
        return false;

    /* packet for holding encoded output */

    if (!m_packet.init())
        return false;
    /* frame containing input raw audio */
    if (!m_frame.init())
        return false;
    m_codec.fillFrame(m_frame);

    /* allocate the data buffers */
    if (!m_frame.allocateBuffer())
        return false;
    m_bytesPerSample = m_codec.getBytesPerSample();
    m_nb_samples = m_frame.nb_samples();
    m_channels = m_codec.channels();
    m_frame_size = m_bytesPerSample * m_nb_samples*m_channels;
    if (on_resample) {
        if (!m_resampler.init(1, 16000, m_codec))
            return false;
    }
    return m_frame_size > 0;
}

bool FfmpegEncoderModule::process(TaskOutBuffer & buffer,std::ostringstream& error_stream)
{
    while (buffer.available() >= m_frame_size)
    {
        if (!readFrame(buffer,error_stream))
            return false;

        if(!encodeFrame(&m_frame,error_stream))
            return false;
    }
    return true;
}

bool FfmpegEncoderModule::finish(TaskOutBuffer & buffer,std::ostringstream& error_stream)
{
    while (buffer.available() >= m_frame_size)
    {
        if (!readFrame(buffer,error_stream))
            return false;
        if (!encodeFrame(&m_frame,error_stream))
            return false;
    }

    if (buffer.available() >= m_bytesPerSample)
    {
        FfmpegFrame last_frame;
        if(!last_frame.init())
        {
            error_stream<<"(fmpegEncoderModule::finish) Could not allocate frame"<<std::endl;
            return false;
        }

        if (!get_last_frame_for_encode(buffer, last_frame,error_stream))
            return false;

        if (!encodeFrame(&last_frame,error_stream))
                return false;
    }

    if (!encodeFrame(NULL,error_stream))
        return false;
    return nextFinish(error_stream);
}

