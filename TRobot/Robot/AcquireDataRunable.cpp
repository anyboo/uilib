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
				}

				if (0 != m_DownloadFileInfo.dwTotalSize)
				{
					m_tEnd = clock();
					duration = (double)(m_tEnd - m_tStart) / CLOCKS_PER_SEC;
					//std::cout << "ʱ���:" << duration << std::endl;
					m_nSpeed = m_DownloadFileInfo.dwDownLoadSize / duration; //kb/s
					//std::cout << "�����ٶ�:" << m_nSpeed << std::endl;
					m_nRemainingTime = (m_DownloadFileInfo.dwTotalSize - m_DownloadFileInfo.dwDownLoadSize) / m_nSpeed; //��*S
					//std::cout << "ʣ��ʱ�䣺" << m_nRemainingTime << std::endl;
					m_nID = m_DownloadFileInfo.nID;

					if (m_DownloadFileInfo.dwDownLoadSize >= m_DownloadFileInfo.dwTotalSize)
					{
						m_nDownloadPos = DOWNLOAD_PROGRESS;
						m_nSpeed = 0;
					//	std::cout << "���ؽ��ȣ�" << m_nDownloadPos << std::endl;
					//	std::cout << "�����ٶ�:" << m_nSpeed << std::endl;
						break;
					}
					else
					{
						m_nDownloadPos = DOWNLOAD_PROGRESS * m_DownloadFileInfo.dwDownLoadSize / m_DownloadFileInfo.dwTotalSize;
						//std::cout << "���ؽ��ȣ�" << m_nDownloadPos << std::endl;
					}

				}
					//std::cout << "�ļ���С:" <<m_DownloadFileInfo.dwDownLoadSize<< std::endl;
					//std::cout << "���ļ���С:" << m_DownloadFileInfo.dwTotalSize << std::endl;

					//std::cout << "-----------------------------------" << std::endl;

				//Thread::sleep(rnd.next(200));
			}
		}
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

int AcquireDataRunable::GetRemainingTime() const
{
	return m_nRemainingTime;
}

int AcquireDataRunable::GetDownloadPos() const
{
	return m_nDownloadPos;
}

int AcquireDataRunable::GetID() const
{
	return m_nID;
}