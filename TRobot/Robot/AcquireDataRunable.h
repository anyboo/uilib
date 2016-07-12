#pragma once

#include "Poco/NotificationQueue.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
//#include "Poco/Thread.h"


#include "SendDataNotification.h"

using Poco::NotificationQueue;
using Poco::Runnable;
using Poco::FastMutex;
//using Poco::Thread;

#include <vector>
#include <ctime>


#define  DOWNLOAD_PROGRESS 100

class AcquireDataRunable :public Runnable
{
public:
	AcquireDataRunable(NotificationQueue& queue);
	~AcquireDataRunable();

	void run();

	DOWNLOADFILEINFO GetDownloadFileInfo() const;
	int GetDownloadSpeed() const;
	int GetRemainingTime() const;
	int GetDownloadPos() const;
private:
	DOWNLOADFILEINFO m_DownloadFileInfo;
	//std::vector<DOWNLOADFILEINFO> m_VecList;

	NotificationQueue& _queue;
	static FastMutex   _mutex;
	clock_t m_tStart;
	clock_t m_tEnd;
	int m_nSpeed;
	int m_nRemainingTime;
	int m_nDownloadPos;
	
};

