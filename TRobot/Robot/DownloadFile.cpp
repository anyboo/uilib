
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
				if (pWorkNf->downloadInfo().pos < 100)
				{
					FastMutex::ScopedLock lock(_mutex);
					//std::cout << "Pos : " << pWorkNf->downloadInfo().pos << " - " << m_fileName << std::endl;
				}
				else
				{
					m_isDownloadFinish = true;
					//break;
				}
			}
		}
		//else break;
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
