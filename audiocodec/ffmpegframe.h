#ifndef FFMPEGFRAME_H
#define FFMPEGFRAME_H
extern "C"
{
    #include <libavutil/frame.h>
}

class FfmpegFrame
{
	AVFrame* m_frame;
public:
	FfmpegFrame();
	~FfmpegFrame();
	void flush();
	bool init();
	AVFrame* getPtr();
	int nb_samples();
	uint8_t* data(int channel);
	bool allocateBuffer();
	bool makeWritable();
};

#endif // FFMPEGFRAME_H
