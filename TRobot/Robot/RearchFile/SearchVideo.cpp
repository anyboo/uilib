#include "SearchVideo.h"
#include <cassert>

CSearchVideo::CSearchVideo()
{
	m_TimeRange.start = 0;
	m_TimeRange.end = 0;
	m_nNotify = 0;
	//m_Device(&);

}


CSearchVideo::~CSearchVideo()
{
}

void CSearchVideo::SearchFile(const std::string &ip, const time_range &range)
{
	assert(!ip.empty());
	//m_Device = m_DeviceManager.getDevice(ip);
	
	std::vector<int> channels;//init later
	for (auto channel : channels)
	{
//		m_Device.Search(channel, range);
	}
}

void CSearchVideo::ReadDataFromTable()
{
	//QMSqlite *pDb = QMSqlite::getInstance();

}