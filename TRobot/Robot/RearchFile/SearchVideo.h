#ifndef __SEARCHVIDEO_H__
#define __SEARCHVIDEO_H__

#include "SearchHead.h"

#include "Device.h"
#include "QMSqlite.h"

#include <string>
#include <vector>
#include <ctime>
#include <assert.h> 

#define SUCCES_REARCH_FILE  1


class CSearchVideo
{
public:
	CSearchVideo();
	~CSearchVideo();

	int SearchFile(const std::string &ip, const time_range &range, Device *dObj, std::vector<size_t> channelList);

	void ReadDataFromTable(std::vector<readSearchVideo> &RSVObj);

	

private:
	time_range m_TimeRange;
	int m_nNotify;
	bool m_bFlag;

};

#endif 

