#ifndef __RECEIVEUINOTIFICATION_H__
#define __RECEIVEUINOTIFICATION_H__


#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class ReceiveUINotification :public Notification
{
public:
	typedef AutoPtr<ReceiveUINotification> Ptr;
	ReceiveUINotification(bool bCancel);
	~ReceiveUINotification();

	bool GetData() const;

private:
	bool m_bCancel;
};

#endif 

