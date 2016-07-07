
#include "SearchVideo.h"
#include <Poco/SingletonHolder.h>


CSearchVideo::CSearchVideo()
{
}

CSearchVideo::~CSearchVideo()
{
}

CSearchVideo& CSearchVideo::getInstance()
{
	static Poco::SingletonHolder<CSearchVideo> shSearchVideo;
	return *shSearchVideo.get();
}


void CSearchVideo::SearchFile(Device* pDevice, const time_range& range, const std::vector<size_t> channelList)
{
	ClearData();

	for (auto channel : channelList)
	{
		pDevice->Search(channel, range);
	}
}

bool CSearchVideo::ReadDataFromTable(std::vector<readSearchVideo>& RSVObj)
{
	RSVObj.clear();

	QMSqlite *pDb = QMSqlite::getInstance();
	std::string strSql = SELECT_ALL_SEARCH_VIDEO;

	return pDb->GetData(strSql, RSVObj);
}

void CSearchVideo::ClearData()
{
	// Delete All Video File Record
	QMSqlite *pDb = QMSqlite::getInstance();
	pDb->cleanData(DELETE_ALL_SEARCH_VIDEO);
}