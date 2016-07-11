#pragma once

#include "Poco/NotificationQueue.h"
#include "Poco/SingletonHolder.h"

using Poco::NotificationQueue;

class DownloadFileNotificationQueue :public NotificationQueue
{
public:
	DownloadFileNotificationQueue();
	~DownloadFileNotificationQueue();

	static DownloadFileNotificationQueue& GetInstance();
	NotificationQueue& GetQueue();

private:
	NotificationQueue m_queue;
};

