#pragma once

#include "Device.h"
#include "QMSqlite.h"
#include "SearchFileNotification.h"

#include "Poco/NotificationQueue.h"
using Poco::NotificationQueue;



class CSearchVideo
{
public:
	CSearchVideo();
	~CSearchVideo();

	static CSearchVideo& getInstance();

	void SearchFile(Device* pDevice, const time_range& range, const std::vector<size_t> channelList);
	void CancelSearchFile();

private:
	bool m_bCancel;
};


