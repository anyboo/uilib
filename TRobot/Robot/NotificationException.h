#pragma once

#include "AbstractVendor.h"

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class CNotificationException : public Notification
{
public:
	CNotificationException(){};
	~CNotificationException(){};

	typedef AutoPtr<CNotificationException> Ptr;

	CNotificationException(const NOTIFICATION_TYPE notificationType) :
		_notificationType(notificationType)
	{
	}

	CNotificationException(const NOTIFICATION_TYPE notificationType, const std::string& errMsg) :
		_notificationType(notificationType),
		_errMsg(errMsg)
	{
	}

	NOTIFICATION_TYPE GetNotificationType() const
	{
		return _notificationType;
	}

private:
	NOTIFICATION_TYPE _notificationType;
	std::string _errMsg;
};

