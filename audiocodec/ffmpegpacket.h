#ifndef FFMPEGPACKET_H
#define FFMPEGPACKET_H

extern "C"
{
    #include <libavcodec/avcodec.h>
}

class FfmpegPacket
{
	AVPacket *m_packet;

public:
	FfmpegPacket();
	~FfmpegPacket();
	void flush();
	bool init();
	AVPacket* getPtr();
	void setZero();
	void unref();
	uint8_t * data();
	int size();
};
#endif // FFMPEGPACKET_H
