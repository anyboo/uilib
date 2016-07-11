#include "DownloadFileNotificationQueue.h"


DownloadFileNotificationQueue::DownloadFileNotificationQueue()
{
}


DownloadFileNotificationQueue::~DownloadFileNotificationQueue()
{
}

DownloadFileNotificationQueue& DownloadFileNotificationQueue::GetInstance()
{
// 	static DownloadFileNotificationQueue instance;
// 	return instance;
	static Poco::SingletonHolder<DownloadFileNotificationQueue> shNotificationQueue;
	return *shNotificationQueue.get();
}

NotificationQueue& DownloadFileNotificationQueue::GetQueue()
{
	return m_queue;
}
