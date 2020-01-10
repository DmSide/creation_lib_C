#include "ffmpegpacket.h"


FfmpegPacket::FfmpegPacket() :m_packet(NULL)
{
}

FfmpegPacket::~FfmpegPacket()
{
	flush();
}

void FfmpegPacket::flush()
{
	if (m_packet)
		av_packet_free(&m_packet);
}

bool FfmpegPacket::init()
{
	flush();
	m_packet = av_packet_alloc();
	if (m_packet == NULL)
	{

		return false;
	}
	return  true;
}

AVPacket * FfmpegPacket::getPtr()
{
	return m_packet;
}

void FfmpegPacket::setZero()
{
	if (m_packet == NULL)
		return;
	m_packet->data = NULL;
	m_packet->size = 0;
}

void FfmpegPacket::unref()
{
	if (m_packet != NULL)
		av_packet_unref(m_packet);
}

uint8_t * FfmpegPacket::data()
{
	if (m_packet == NULL)
		return NULL;
	return m_packet->data;
}

int FfmpegPacket::size()
{
	if (m_packet == NULL)
		return -1;
	return m_packet->size;
}
