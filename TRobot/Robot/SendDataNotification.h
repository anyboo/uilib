#ifndef __SENDFATANOTIFICATION_H__
#define __SENDFATANOTIFICATION_H__

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

typedef struct _DOWNLOADFILEINFO
{
	int nID;
	DWORD dwTotalSize;
	DWORD dwDownLoadSize;
}DOWNLOADFILEINFO, *LPDOWNLOADFILEINFO;

class SendDataNotification:public Notification
{
public:
	typedef AutoPtr<SendDataNotification> Ptr;

	SendDataNotification(DOWNLOADFILEINFO DownloadFileInfo);
	~SendDataNotification();

	DOWNLOADFILEINFO GetData() const;
private:
	DOWNLOADFILEINFO m_DownloadFileInfo;
};


#endif

