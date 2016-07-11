#pragma once

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"

#include "Device.h"
#include "Notification.h"
#include "NotificationQueue.h"

using Poco::Runnable;
using Poco::FastMutex;
using Poco::Thread;
using Poco::ThreadPool;

class CDownloadFile :
	public Runnable
{
public:
	CDownloadFile();
	CDownloadFile(Device* pDev, const int channel, const time_range& timeRange, const std::string& filename = "");
	~CDownloadFile();

	void DownloadFileByTime();
	void DownloadFileByName();

	virtual void run();
	bool GetDownloadStatus(){ return m_isDownloadFinish; }

private:
	static FastMutex _mutex;
	bool m_isDownloadFinish;

	Device* m_pDev;
	int m_channel;
	std::string m_fileName;
	time_range m_timeRange;
};

