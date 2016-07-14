#include "SearchFileRunable.h"

FastMutex SearchFileRunable::m_mutex;

SearchFileRunable::SearchFileRunable(NotificationQueue& queue):
m_queue(queue)
{

}


SearchFileRunable::~SearchFileRunable()
{
}

void SearchFileRunable::run()
{
	Poco::Random rnd;
	double  duration = 0;

	//ESearchFileInfo eObj;
	int nData = 0;
// 	for (;;)
// 	{
// 		Notification::Ptr pNf(m_queue.waitDequeueNotification());
// 		if (pNf)
// 		{
// 			SearchFileNotification::Ptr pSendDataNf = pNf.cast<SearchFileNotification>();
// 			if (pSendDataNf)
// 			{
// 				{
// 					FastMutex::ScopedLock lock(m_mutex);
// 					//eObj = pSendDataNf->GetSearchID();
// 					nData = pSendDataNf->GetData();
// 
// 				}			
// 			}
// 		}
// 	}
}
