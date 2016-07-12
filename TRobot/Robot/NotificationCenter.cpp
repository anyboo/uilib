
#include "NotificationCenter.h"
#include <Poco/SingletonHolder.h>

#include "Notification.h"
#include "NotificationQueue.h"

FastMutex CNotificationCenter::_mutex;

CNotificationCenter::CNotificationCenter()
{
}

CNotificationCenter::~CNotificationCenter()
{
}

CNotificationCenter& CNotificationCenter::getInstance()
{
	static Poco::SingletonHolder<CNotificationCenter> shNotificationCenter;
	return *shNotificationCenter.get();
}

void CNotificationCenter::run()
{
	for (;;)
	{
		Notification::Ptr pNf(CNotificationQueue::getInstance().GetQueue().waitDequeueNotification());
		if (pNf)
		{
			CNotification::Ptr pWorkNf = pNf.cast<CNotification>();
			if (pWorkNf)
			{
				FastMutex::ScopedLock lock(_mutex);
				std::cout << "NotificationCenter : " << pWorkNf->GetNotificationType() << std::endl;
			}
		}
	}
}
