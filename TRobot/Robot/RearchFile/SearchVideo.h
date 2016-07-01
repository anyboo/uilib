#pragma once

#include "SearchHead.h"

#include "DeviceManager.h"
#include "DHVendor.h"

class CSearchVideo
{
public:
	CSearchVideo();
	~CSearchVideo();

	void SearchFile(const std::string &ip, const time_range &range);

	void ReadDataFromTable();

private:
	time_range m_TimeRange;
	CDeviceManager m_DeviceManager;
//	Device m_Device;
//	DHVendor m_DHObj;
	int m_nNotify;

};

