#ifndef FFMPEGDECODERMODULE_H
#define FFMPEGDECODERMODULE_H

#include "taskstartmodule.h"
#include "ffmpegdecoder.h"
#include "ffmpegresampler.h"


#include "threadsafe_queue_buffer.h"

class FfmpegDecoderModule:public TaskStartModule
{
private:
    FfmpegDecoder m_decoder;
    FfmpegCodec m_codec;
    FfmpegResampler m_resampler;
    bool resample_on = true;
    int m_sampleRate;
    bool init(std::ostringstream& error_stream);
    bool decode(std::ostringstream& error_stream);
    bool decodeFrame(FfmpegPacket& packet,std::ostringstream& error_stream,bool isLastPacket=false);
public:
    FfmpegDecoderModule(TaskJoinModule* next, ThreadSafeQueueBuffer & buffer,int sampleRate = 0);
    ~FfmpegDecoderModule();
    bool process(std::ostringstream& error_stream);
};

#endif // FFMPEGDECODERMODULE_H
