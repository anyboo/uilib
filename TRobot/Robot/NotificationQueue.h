#pragma once

#include "Poco/NotificationQueue.h"

using Poco::NotificationQueue;

class CNotificationQueue
{
public:
	CNotificationQueue();
	~CNotificationQueue();

	static CNotificationQueue& getInstance();

	NotificationQueue& GetQueue(){ return m_queue; }

private:
	NotificationQueue m_queue;
};

