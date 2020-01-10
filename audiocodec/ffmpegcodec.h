#ifndef FFMPEGCODEC_H
#define FFMPEGCODEC_H

#include "ffmpegpacket.h"
#include "ffmpegframe.h"

class FfmpegCodec
{
	AVCodecContext* m_codcont;
	AVCodec* m_codec;
public:
	FfmpegCodec();
	~FfmpegCodec();

	bool init(AVCodecID id,bool isDecoder);
	bool open();
	void flush();
	AVCodecContext* getPtr();
	AVCodec* getCodecPtr();
	int channels();
	void setChannelLayoutMono(); 
	void setChannels(int count);
	void setBitRate(int count);
	bool setSampleFormat(AVSampleFormat format);
	void setDefaultChannelLayout();
    bool setChannelLayout(unsigned int layout);
	void setDefaultSampleRate();
	bool setSampleRate(int count);
	bool checkSampleFormat(AVSampleFormat format);
	bool pushPacket(FfmpegPacket& packet);
	bool getFrame(FfmpegFrame& frame);
	void fillFrame(FfmpegFrame & fframe);
	int getBytesPerSample();
	bool sendFrame(FfmpegFrame* frame);
	int receivePacket(FfmpegPacket& packet);
};

#endif // FFMPEGCODEC_H
