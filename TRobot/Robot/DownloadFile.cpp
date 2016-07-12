
#include "DownloadFile.h"
#include <assert.h>

FastMutex CDownloadFile::_mutex;

CDownloadFile::CDownloadFile()
{
	m_isDownloadFinish = false;
}

CDownloadFile::~CDownloadFile()
{
}

CDownloadFile::CDownloadFile(Device* pDev, const int channel, const time_range& timeRange, const std::string& filename)
{
	m_pDev = pDev;
	m_channel = channel;
	m_timeRange = timeRange;
	m_fileName = filename;

	if (!filename.empty() || filename != "")
	{
		DownloadFileByName();
	}
	else
	{
		DownloadFileByTime();
	}
}

void CDownloadFile::run()
{
	Poco::Random rnd;
	for (;;)
	{
		Notification::Ptr pNf(CNotificationQueue::getInstance().GetQueue().waitDequeueNotification());
		if (pNf)
		{
			CNotification::Ptr pWorkNf = pNf.cast<CNotification>();
			if (pWorkNf)
			{
				FastMutex::ScopedLock lock(_mutex);
				if (pWorkNf->GetNotificationType() == Notification_Type_Download_End)
				{
					break;
				}
			}
		}
	}
}
void CDownloadFile::DownloadFileByTime()
{
	m_pDev->Download(m_channel, m_timeRange);
}
void CDownloadFile::DownloadFileByName()
{
	assert(m_fileName != "");
	m_pDev->Download(m_channel, m_fileName);
}
