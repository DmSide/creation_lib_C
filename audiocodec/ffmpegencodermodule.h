#ifndef FFMPEGENCODERMODULE_H
#define FFMPEGENCODERMODULE_H

#include "taskappendmodule.h"
#include "ffmpegcodec.h"
#include "ffmpegresampler.h"


class FfmpegEncoderModule:public TaskAppendModule,public TaskJoinModule
{

    FfmpegCodec m_codec;
    FfmpegFrame m_frame;
    FfmpegPacket m_packet;
    FfmpegResampler m_resampler;
    int m_bytesPerSample;
    int m_nb_samples;
    int m_channels;
    size_t m_frame_size;
    bool encodeFrame(FfmpegFrame* frame,std::ostringstream& error_stream);
    bool readFrame(TaskOutBuffer& buffer,std::ostringstream& error_stream);
    bool get_last_frame_for_encode(TaskOutBuffer& buffer, FfmpegFrame& last_frame,std::ostringstream& error_stream);
public:
    bool on_resample;
    FfmpegEncoderModule(TaskJoinModule* next, bool rnnoise_on);
    ~FfmpegEncoderModule();
    bool init(AVCodecID id, AVSampleFormat sampleFormat,int sampleRate);
    bool process(TaskOutBuffer& buffer,std::ostringstream& error_stream);
    bool finish(TaskOutBuffer& buffer,std::ostringstream& error_stream);

};

#endif // FFMPEGENCODERMODULE_H
