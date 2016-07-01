#pragma once

#include "SearchHead.h"

#include "DeviceManager.h"


class CSearchVideo
{
public:
	CSearchVideo();
	~CSearchVideo();

	void SearchFile(const std::string &ip, const time_range &range);

	void ReadDataFromTable();

private:
	time_range m_TimeRange;
	DeviceManager m_DeviceManager;
	Device m_Device;
	int m_nNotify;

};

