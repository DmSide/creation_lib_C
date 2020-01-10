#ifndef FFMEGRESAMPLER_H
#define FFMEGRESAMPLER_H

extern "C"
{
    #include <libswresample/swresample.h>
}
#include "ffmpegcodec.h"



class FfmpegResampler
{
    SwrContext* m_swr;
    int m_src_sample_rate;
    int dst_sample_rate;
public:

    FfmpegResampler();
    ~FfmpegResampler();
    void flush();
    bool init(int channel,int rate_of_sample,FfmpegCodec& codec);

    unsigned char* resampleFrame(FfmpegPacket& packet, FfmpegFrame& frame, size_t& size);
    bool resample_frame_before_encode(FfmpegFrame& fframe, FfmpegFrame& outf);
};

#endif // FFMEGRESAMPLER_H
