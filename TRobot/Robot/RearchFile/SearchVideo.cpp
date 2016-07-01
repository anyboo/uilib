#include "SearchVideo.h"
#include <cassert>

CSearchVideo::CSearchVideo()
{
	m_TimeRange.start = 0;
	m_TimeRange.end = 0;
	m_nNotify = 0;
	m_bFlag = false;
	m_Device.setSDK(&m_DHObj);

}


CSearchVideo::~CSearchVideo()
{
}

int CSearchVideo::SearchFile(const std::string &ip, const time_range &range)
{
	assert(!ip.empty());
	m_Device = m_DeviceManager.getDevice(ip);

	
	std::vector<size_t> channelList; //init later
	size_t ta = 0;
	size_t tb = 1;
	channelList.push_back(ta);
	channelList.push_back(tb);

	m_Device.setChannel(10, channelList);
	
	std::vector<size_t> channels = m_Device.getChannelList();//init later
	for (auto channel : channels)
	{
		m_Device.Search(channel, range);
	}

	return SUCCES_REARCH_FILE;
}

void CSearchVideo::ReadDataFromTable(std::vector<readSearchVideo> &RSVObj)
{
	QMSqlite *pDb = QMSqlite::getInstance();
	string strSql = SELECT_ALL_SEARCH_DEVICE;

	bool bFlag = pDb->GetData(strSql, RSVObj);
	
	assert(bFlag);
}