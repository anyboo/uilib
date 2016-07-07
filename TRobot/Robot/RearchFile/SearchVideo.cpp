#include "SearchVideo.h"
#include <cassert>

CSearchVideo::CSearchVideo()
{
	m_TimeRange.start = 0;
	m_TimeRange.end = 0;
	m_nNotify = 0;
	m_bFlag = false;

}


CSearchVideo::~CSearchVideo()
{
}



int CSearchVideo::SearchFile(const std::string &ip, const time_range &range, Device *pDevice, std::vector<size_t> channelList)
{
	assert(!ip.empty());
	
	//std::vector<size_t> channelList; //init later

	if (!channelList.empty())
	{
		pDevice->setChannel(channelList);
		assert(pDevice->m_pVendor);
	}
	
	std::vector<size_t> channels = pDevice->getChannelList();//init later
	assert(pDevice->m_pVendor);

	//std::vector<size_t>::iterator it;
// 	for (it != channels.begin(); it != channels.end(); ++it)
// 	{
// 		m_Device.Search(*it, range);
// 	}

	for (auto channel : channels)
	{
		pDevice->Search(channel, range);
		assert(pDevice->m_pVendor);
	}

	return SUCCES_REARCH_FILE;
}

void CSearchVideo::ReadDataFromTable(std::vector<readSearchVideo> &RSVObj)
{
	QMSqlite *pDb = QMSqlite::getInstance();
	string strSql = SELECT_ALL_SEARCH_VIDEO;

	bool bFlag = pDb->GetData(strSql, RSVObj);
	//pDb->GetData(strSql, RSVObj);
	//std::cout << "" << bFlag << std::endl;
	//assert(bFlag);
}