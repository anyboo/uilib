
#include "PlayVideoWorker.h"


FastMutex CPlayVideoWorker::m_mutex;

CPlayVideoWorker::CPlayVideoWorker(Device* pDev, const RecordFile& file, const HWND hWnd, NotificationQueue& queue) :
m_queue(queue)
{
	m_pDev = pDev;
	m_file = file;
	m_hWnd = hWnd;
}

CPlayVideoWorker::~CPlayVideoWorker()
{
}

void CPlayVideoWorker::run()
{
	m_pDev->PlayVideo(m_hWnd, m_file);

	for (;;)
	{
		::Sleep(100);
		if (!m_queue.empty())
		{
			Notification::Ptr pNf(m_queue.waitDequeueNotification());
			if (pNf)
			{
				CNotificationPlayVideo::Ptr pPlayVideoNf = pNf.cast<CNotificationPlayVideo>();
				if (pPlayVideoNf)
				{
					if (pPlayVideoNf->GetNotificationType() == Notification_Type_Play_Video_Pos)
					{
						FastMutex::ScopedLock lock(m_mutex);
						m_pDev->SetPlayVideoPos(pPlayVideoNf->GetPos());
					}
					else if (pPlayVideoNf->GetNotificationType() == Notification_Type_Play_Video_Stop)
					{
						FastMutex::ScopedLock lock(m_mutex);
						m_pDev->StopPlayVideo();
						break;
					}
				}
			}
		}		
	}
}
