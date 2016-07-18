#pragma once

#include "AbstractVendor.h"

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class CNotificationDeviceManager : public Notification
{
public:
	CNotificationDeviceManager(){};
	~CNotificationDeviceManager(){};

	typedef AutoPtr<CNotificationDeviceManager> Ptr;

	CNotificationDeviceManager(const NOTIFICATION_TYPE notificationType):
		_notificationType(notificationType)
	{
	}

	CNotificationDeviceManager(const NOTIFICATION_TYPE notificationType, const std::string& ip) :
		_notificationType(notificationType),
		_ip(ip)
	{
	}

	NOTIFICATION_TYPE GetNotificationType() const
	{
		return _notificationType;
	}

private:
	NOTIFICATION_TYPE _notificationType;
	std::string _ip;
};

