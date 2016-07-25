#ifndef __SEARCHFILENOTIFICATION_H__
#define __SEARCHFILENOTIFICATION_H__

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

#include "AbstractVendor.h"

using Poco::Notification;
using Poco::AutoPtr;

#define SEARCHFILE_DEFAULT -1

class SearchFileNotification :public Notification
{
public:
	typedef AutoPtr<SearchFileNotification> Ptr;

	SearchFileNotification(NOTIFICATION_TYPE eNotify, int nData);
	~SearchFileNotification();

	NOTIFICATION_TYPE GetNotify();
	int GetData();
private:
	NOTIFICATION_TYPE m_eNotify;
	int m_nSearchPos;
	int m_nTotal;

};

#endif 

