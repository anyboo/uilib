
#include "NotificationQueue.h"
#include <Poco/SingletonHolder.h>

CNotificationQueue::CNotificationQueue()
{
}

CNotificationQueue::~CNotificationQueue()
{
}

CNotificationQueue& CNotificationQueue::getInstance()
{
	static Poco::SingletonHolder<CNotificationQueue> shNotificationQueue;
	return *shNotificationQueue.get();
}