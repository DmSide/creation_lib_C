#include "ffmpegcodec.h"

FfmpegCodec::FfmpegCodec() :m_codcont(NULL), m_codec(NULL)
{
}

FfmpegCodec::~FfmpegCodec()
{
	flush();
}

bool FfmpegCodec::init(AVCodecID id, bool isDecoder)
{
	flush();
	m_codec = isDecoder ? avcodec_find_decoder(id) : avcodec_find_encoder(id);
	if (m_codec == NULL) {
		fprintf(stderr, "Codec not found\n");
		return false;
	}
	m_codcont = avcodec_alloc_context3(m_codec);
	if (!m_codcont) {
		fprintf(stderr, "Could not allocate audio codec context\n");
		return false;
	}
	return true;
}

bool FfmpegCodec::open()
{
	if (m_codcont == NULL)
		return false;
	if (avcodec_open2(m_codcont, m_codec, NULL) < 0) {
		fprintf(stderr, "Could not open codec\n");
		return false;
	}
	return true;
}

void FfmpegCodec::flush()
{
	if (m_codcont) {
		avcodec_free_context(&m_codcont);
		m_codcont = NULL;
	}
	m_codec = NULL;
}


AVCodecContext* FfmpegCodec::getPtr()
{
	return m_codcont;
}

AVCodec * FfmpegCodec::getCodecPtr()
{
	return m_codec;
}

int FfmpegCodec::channels()
{
	if (m_codcont == NULL)
		return -1;
	return m_codcont->channels;
}

void FfmpegCodec::setChannelLayoutMono()
{
	if (m_codcont == NULL)
		return;
	m_codcont->channel_layout = AV_CH_LAYOUT_MONO;
}

void FfmpegCodec::setChannels(int count)
{
	if (m_codcont == NULL)
		return;
	m_codcont->channels = count;
}

void FfmpegCodec::setBitRate(int count)
{
	if (m_codcont == NULL)
		return;
	m_codcont->bit_rate = count;
}

bool FfmpegCodec::setSampleFormat(AVSampleFormat format)
{
	if (m_codcont == NULL)
		return false;
	const enum AVSampleFormat *p = m_codec->sample_fmts;
	while (*p != AV_SAMPLE_FMT_NONE) {
		if (*p == format) {
			m_codcont->sample_fmt = format;
			return true;
		}
		p++;
	}
	return false;
}

void FfmpegCodec::setDefaultChannelLayout()
{
	if (m_codcont == NULL)
		return;
	uint64_t best_ch_layout = 0;
	int best_nb_channels = 0;
	const uint64_t *p = m_codec->channel_layouts;
	if (!p) {
		m_codcont->channel_layout = AV_CH_LAYOUT_STEREO;
		return;
	}
	while (*p) {
		int nb_channels = av_get_channel_layout_nb_channels(*p);
		if (nb_channels > best_nb_channels) {
			best_ch_layout = *p;
			best_nb_channels = nb_channels;
		}
		p++;
	}
	m_codcont->channel_layout = best_ch_layout;
}

bool FfmpegCodec::setChannelLayout(unsigned int layout)
{
	if (m_codcont == NULL)
		return false;
	const uint64_t *p = m_codec->channel_layouts;
	if (!p) {
        m_codcont->channel_layout = layout;
        m_codcont->channels = av_get_channel_layout_nb_channels(layout);
		return true;
	}
	while (*p) {

		if (*p == layout)
		{
			m_codcont->channel_layout = layout;
			m_codcont->channels = av_get_channel_layout_nb_channels(layout);
			return true;
		}

		p++;
	}
	return false;

}

void FfmpegCodec::setDefaultSampleRate()
{
	if (m_codcont == NULL)
		return;
	const int *p = m_codec->supported_samplerates;
	int best_samplerate = 0;
	if (!p) {
		m_codcont->sample_rate = 44100;
		return;
	}

	while (*p) {
		if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate))
			best_samplerate = *p;
		p++;
	}
	m_codcont->sample_rate = best_samplerate;
}

bool FfmpegCodec::setSampleRate(int count)
{
	if (m_codcont == NULL)
		return false;
	const int *p = m_codec->supported_samplerates;
	if (!p) {
		m_codcont->sample_rate = count;
		return true;
	}

	while (*p) {
		if (*p == count)
		{
			m_codcont->sample_rate = count;
			return true;
		}
		p++;
	}
	return false;
}

bool FfmpegCodec::checkSampleFormat(AVSampleFormat format)
{
	if (m_codcont == NULL)
		return false;
	const enum AVSampleFormat *p = m_codec->sample_fmts;
	while (*p != AV_SAMPLE_FMT_NONE) {
		if (*p == format)
			return true;
		p++;
	}
	return false;
}

bool FfmpegCodec::pushPacket(FfmpegPacket& fpacket)
{
	AVPacket* packet = fpacket.getPtr();
	if (packet == NULL || m_codcont == NULL)
		return false;
	if (avcodec_send_packet(m_codcont, packet) != 0)
		return false;
	return true;
}

bool FfmpegCodec::getFrame(FfmpegFrame & fframe)
{
	AVFrame* frame = fframe.getPtr();
	if (frame == NULL || m_codcont == NULL)
		return false;
	if (avcodec_receive_frame(m_codcont, frame) != 0) {
		return false;
	}
	return true;
}

void FfmpegCodec::fillFrame(FfmpegFrame & fframe)
{
	AVFrame* frame = fframe.getPtr();
	if (m_codcont == NULL || frame == NULL)
		return;
	frame->nb_samples = m_codcont->frame_size > 0 ? m_codcont->frame_size : 1024;
	frame->format = m_codcont->sample_fmt;
	frame->channel_layout = m_codcont->channel_layout;
	frame->channels = m_codcont->channels;
}

int FfmpegCodec::getBytesPerSample()
{
	if (m_codcont == NULL)
		return -1;
	return av_get_bytes_per_sample(m_codcont->sample_fmt);
}

bool FfmpegCodec::sendFrame(FfmpegFrame * frame)
{
    if (m_codcont == NULL || avcodec_send_frame(m_codcont, frame == NULL ? NULL : frame->getPtr())) {
		return false;
	}
	return true;
}

int FfmpegCodec::receivePacket(FfmpegPacket & fpacket)
{
	AVPacket* packet = fpacket.getPtr();
	if (packet == NULL || m_codcont == NULL)
		return AVERROR(ENOMEM);
	return avcodec_receive_packet(m_codcont, packet);
}
