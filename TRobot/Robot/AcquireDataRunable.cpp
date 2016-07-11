#include "AcquireDataRunable.h"


FastMutex AcquireDataRunable::_mutex;

AcquireDataRunable::AcquireDataRunable(NotificationQueue& queue):
_queue(queue),
m_tStart(0),
m_tEnd(0),
m_nSpeed(0)
{

}


AcquireDataRunable::~AcquireDataRunable()
{
}


void AcquireDataRunable::run()
{
	Poco::Random rnd;
	double  duration = 0;
	m_tStart = clock();
	for (;;)
	{
		Notification::Ptr pNf(_queue.waitDequeueNotification());
		if (pNf)
		{
			SendDataNotification::Ptr pSendDataNf = pNf.cast<SendDataNotification>();
			if (pSendDataNf)
			{
				{
					FastMutex::ScopedLock lock(_mutex);
					m_DownloadFileInfo = pSendDataNf->GetData();

					if (0 != m_DownloadFileInfo.dwTotalSize)
					{
						m_tEnd = clock();
						duration = (double)(m_tEnd - m_tStart) / CLOCKS_PER_SEC;
						std::cout << "duration:" << duration << std::endl;
						m_nSpeed = m_DownloadFileInfo.dwDownLoadSize / duration; //kb/s
						std::cout << "m_nSpeed:" << m_nSpeed << std::endl;
						m_RemainingTime = (m_DownloadFileInfo.dwTotalSize - m_DownloadFileInfo.dwDownLoadSize) / m_nSpeed; //Ãë*S
						std::cout << "Ê£ÓàÊ±¼ä£º" << m_RemainingTime << std::endl;
					}

					std::cout << "dwDownLoadSize:" <<m_DownloadFileInfo.dwDownLoadSize<< std::endl;
					std::cout << "dwTotalSize:" << m_DownloadFileInfo.dwTotalSize << std::endl;
				}
				if (m_DownloadFileInfo.dwDownLoadSize >= m_DownloadFileInfo.dwTotalSize)
				{
					break;
				}
				//Thread::sleep(rnd.next(200));
			}
		}
		else break;
	}
}

DOWNLOADFILEINFO AcquireDataRunable::GetDownloadFileInfo() const
{
	return m_DownloadFileInfo;
}

int AcquireDataRunable::GetDownloadSpeed() const
{
	return m_nSpeed;
}