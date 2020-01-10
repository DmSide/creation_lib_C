#include "ffmpegframe.h"


FfmpegFrame::FfmpegFrame():m_frame(NULL)
{
}

FfmpegFrame::~FfmpegFrame()
{
	flush();
}

void FfmpegFrame::flush()
{
	if (m_frame)
		av_frame_free(&m_frame);
}

bool FfmpegFrame::init()
{
	flush();
	m_frame = av_frame_alloc();
	if (!m_frame)
    {
		return false;
	}
	return true;
}

AVFrame * FfmpegFrame::getPtr()
{
	return m_frame;
}

int FfmpegFrame::nb_samples()
{
	if (m_frame)
		return m_frame->nb_samples;
	return -1;
}

uint8_t * FfmpegFrame::data(int channel)
{
	if (m_frame)
		return m_frame->data[channel];
	return NULL;
}

bool FfmpegFrame::allocateBuffer()
{
	
    if (m_frame == NULL || av_frame_get_buffer(m_frame, 0) < 0) {
		fprintf(stderr, "Could not allocate audio data buffers\n");
		return false;
	}
	return true;
}

bool FfmpegFrame::makeWritable()
{
    if(m_frame == NULL || av_frame_make_writable(m_frame) < 0)
		return false;
	return true;
}
