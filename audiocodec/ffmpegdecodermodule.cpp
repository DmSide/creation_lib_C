#include "ffmpegdecodermodule.h"


bool FfmpegDecoderModule::init(std::ostringstream& error_stream)
{
    if (m_decoder.init(error_stream) == false)
        return false;
    if (!m_decoder.initFormat(error_stream))
        return false;
    int nstreams = -1;
    if (!m_decoder.findStreamInfo(nstreams) || nstreams < 1){
        error_stream<<"(FfmpegDecoderModule::init) Could not find stream info"<<std::endl;
        return false;
    }
    if (!m_decoder.findDecoder(0, m_codec)){
        error_stream<<"(FfmpegDecoderModule::init) Could not find decoder"<<std::endl;
        return false;
    }
    if (resample_on)
    {
        if (!m_resampler.init(1, m_sampleRate, m_codec)){
            error_stream<<"(FfmpegDecoderModule::init) Could not init resampler"<<std::endl;
            return false;
        }
    }
    else if (m_codec.channels() != 1 && !m_codec.setChannelLayout(AV_CH_LAYOUT_MONO)) {
        error_stream<<"(FfmpegDecoderModule::init) Could not set Mono chanel layout"<<std::endl;
        return false;
    }
    return true;
}

bool FfmpegDecoderModule::decode(std::ostringstream& error_stream)
{
    FfmpegPacket packet;
    if (!packet.init()){
         error_stream<<"(FfmpegDecoderModule::decode) Could not allocate the packet"<<std::endl;
        return false;
    }
    while (m_decoder.readNextPacket(packet))
    {

        if (!decodeFrame(packet,error_stream)){
            return false;
        }
    }
    packet.setZero();
    return decodeFrame(packet,error_stream,true);
}

bool FfmpegDecoderModule::decodeFrame(FfmpegPacket & packet,std::ostringstream& error_stream,bool isLastPacket)
{

    if (m_codec.pushPacket(packet)) {
        FfmpegFrame frame;
        if (!frame.init()){
            error_stream<<"(FfmpegDecoderModule::decodeFrame) Could not allocate frame"<<std::endl;
            return false;
        }
        bool have_frame = true;
        while (have_frame)
        {
            have_frame = m_codec.getFrame(frame);
            int nb_samples = frame.nb_samples();
            if (nb_samples == 0 && !isLastPacket)
                continue;
            if (resample_on)
            {
                size_t size = 0;
                unsigned char* data = m_resampler.resampleFrame(packet, frame, size);
                if (!data){
                     error_stream<<"(FfmpegDecoderModule::decodeFrame) resmpler error"<<std::endl;
                    return false;
                }
                bool isComplete = out(data, size,error_stream);
                delete[] data;
                if (!isComplete)
                    return false;
            }
            else {
                int data_size = m_codec.getBytesPerSample();
                int nb_samples = frame.nb_samples();
                int channels = m_codec.channels();
                if (data_size < 0 || nb_samples < 0)
                     error_stream<<"(FfmpegDecoderModule::decodeFrame) frame not data"<<std::endl;
                    return false;
                try {
                    for (int i = 0; i < nb_samples; i++) {
                        for (int ch = 0; ch < channels; ch++) {
                            if (!out(frame.data(ch) + data_size * i, data_size,error_stream))
                                return false;
                        }
                    }
                }
                catch (...)
                {
                     error_stream<<"(FfmpegDecoderModule::decodeFrame) throw error"<<std::endl;
                    return false;
                }
            }
        }
        frame.flush();
        packet.unref();
    }
    return true;
}

FfmpegDecoderModule::FfmpegDecoderModule(TaskJoinModule* next, ThreadSafeQueueBuffer& buffer,int sampleRate) :TaskStartModule(next),m_decoder(buffer),m_sampleRate(sampleRate)
{
    resample_on = sampleRate > 0;
}


FfmpegDecoderModule::~FfmpegDecoderModule()
{
}

bool FfmpegDecoderModule::process(std::ostringstream& error_stream)
{
    if (!init(error_stream))
        return false;
    if (!decode(error_stream))
        return false;
    if (!nextFinish(error_stream))
        return false;

    return true;
}
