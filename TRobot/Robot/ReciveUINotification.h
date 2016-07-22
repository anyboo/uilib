#ifndef __RECIVEUINOTIFICATION_H__
#define __RECIVEUINOTIFICATION_H__


#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class ReciveUINotification :public Notification
{
public:
	typedef AutoPtr<ReciveUINotification> Ptr;
	ReciveUINotification(bool bCancel);
	~ReciveUINotification();

	bool GetData() const;

private:
	bool m_bCancel;
};

#endif 

