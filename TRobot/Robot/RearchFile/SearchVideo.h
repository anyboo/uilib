#pragma once

#include "Device.h"
#include "QMSqlite.h"


class CSearchVideo
{
public:
	CSearchVideo();
	~CSearchVideo();

	static CSearchVideo& getInstance();

	void SearchFile(Device* pDevice, const time_range& range, const std::vector<size_t> channelList);
	bool ReadDataFromTable(std::vector<readSearchVideo> &RSVObj);

protected:
	void ClearData();
};


