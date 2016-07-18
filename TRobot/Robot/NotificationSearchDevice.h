#pragma once

#include "AbstractVendor.h"

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class CNotificationSearchDevice : public Notification
{
public:
	CNotificationSearchDevice(){};
	~CNotificationSearchDevice(){};

	typedef AutoPtr<CNotificationSearchDevice> Ptr;

	CNotificationSearchDevice(const NOTIFICATION_TYPE notificationType) :
		_notificationType(notificationType)
	{
	}

	NOTIFICATION_TYPE GetNotificationType() const
	{
		return _notificationType;
	}

private:
	NOTIFICATION_TYPE _notificationType;
};

