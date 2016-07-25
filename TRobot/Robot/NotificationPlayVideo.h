#pragma once

#include "AbstractVendor.h"

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class CNotificationPlayVideo : public Notification
{
public:
	CNotificationPlayVideo(){};
	~CNotificationPlayVideo(){};

	typedef AutoPtr<CNotificationPlayVideo> Ptr;

	CNotificationPlayVideo(const NOTIFICATION_TYPE notificationType) :
		_notificationType(notificationType)
	{
	}

	CNotificationPlayVideo(const NOTIFICATION_TYPE notificationType, int pos) :
		_notificationType(notificationType),
		_pos(pos)
	{
	}

	NOTIFICATION_TYPE GetNotificationType() const
	{
		return _notificationType;
	}

	int GetPos() const{
		return _pos;
	}

private:
	NOTIFICATION_TYPE _notificationType;
	int _pos;
};

