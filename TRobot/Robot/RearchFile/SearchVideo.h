#pragma once

#include "SearchHead.h"

#include "DeviceManager.h"
#include "DHVendor.h"
#include "QMSqlite.h"

#define SUCCES_REARCH_FILE  1


class CSearchVideo
{
public:
	CSearchVideo();
	~CSearchVideo();

	int SearchFile(const std::string &ip, const time_range &range);

	void ReadDataFromTable(std::vector<readSearchVideo> &RSVObj);

private:
	time_range m_TimeRange;
	CDeviceManager m_DeviceManager;
	Device m_Device;
	DHVendor m_DHObj;
	int m_nNotify;
	bool m_bFlag;

};

