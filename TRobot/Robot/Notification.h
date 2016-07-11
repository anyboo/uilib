#pragma once

#include "AbstractVendor.h"

#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;

class CNotification : public Notification
{
public:
	CNotification();
	~CNotification();

	typedef AutoPtr<CNotification> Ptr;
	CNotification(const DownloadInfo& downloadInfo) :
		_downloadInfo(downloadInfo)
	{
	}

	DownloadInfo downloadInfo() const
	{
		return _downloadInfo;
	}

private:
	DownloadInfo _downloadInfo;
};

