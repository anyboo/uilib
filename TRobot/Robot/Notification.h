#pragma once

#include "AbstractVendor.h"

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

typedef enum
{
	Notification_Type_None = 0,
	Notification_Type_Network,
	Notification_Type_SearchFile,
	Notification_Type_Download_Start,
	Notification_Type_Download_End,
	Notification_Type_PlayVideo_Start,
	Notification_Type_PlayVideo_Parse,
	Notification_Type_PlayVideo_Restart,
	Notification_Type_PlayVideo_End,

}NOTIFICATION_TYPE;

class CNotification : public Notification
{
public:
	CNotification();
	~CNotification();

	typedef AutoPtr<CNotification> Ptr;
	CNotification(const NOTIFICATION_TYPE notificationType) :
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

