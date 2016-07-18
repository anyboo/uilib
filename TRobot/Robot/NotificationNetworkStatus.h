#pragma once

#include "AbstractVendor.h"

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class CNotificationNetworkStatus : public Notification
{
public:
	CNotificationNetworkStatus(){};
	~CNotificationNetworkStatus(){};

	typedef AutoPtr<CNotificationNetworkStatus> Ptr;

	CNotificationNetworkStatus(const NOTIFICATION_TYPE notificationType) :
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

