#pragma once

#include "Poco/Runnable.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Mutex.h"
#include "Poco/AutoPtr.h"

#include "AbstractVendor.h"
#include "NotificationDeviceManager.h"

using Poco::Runnable;
using Poco::NotificationQueue;
using Poco::FastMutex;
using Poco::AutoPtr;

class CDeviceManager :
	public Runnable
{
public:
	CDeviceManager(VENDOR_LIST& pVendorList, NotificationQueue& queue);
	~CDeviceManager();

	virtual void run();

private:
	VENDOR_LIST& m_pVendorList;
	NotificationQueue& m_queue;
	static FastMutex m_mutex;
};

