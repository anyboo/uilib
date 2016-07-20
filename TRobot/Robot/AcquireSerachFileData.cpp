#include "AcquireSerachFileData.h"

FastMutex AcquireSerachFileData::m_mutex;

AcquireSerachFileData::AcquireSerachFileData(NotificationQueue& queue) :
m_queue(queue)
{

}


AcquireSerachFileData::~AcquireSerachFileData()
{
}

void AcquireSerachFileData::run()
{
	Poco::Random rnd;

	NOTIFICATION_TYPE eNotify;
	int nData;


	for (;;)
	{
		Notification::Ptr pNf(m_queue.waitDequeueNotification());
		if (pNf)
		{
			SearchFileNotification::Ptr pAcquireDataNf = pNf.cast<SearchFileNotification>();

			if (pAcquireDataNf)
			{
				{
					FastMutex::ScopedLock lock(m_mutex);
					eNotify = pAcquireDataNf->GetNotify();
					nData = pAcquireDataNf->GetData();
				}

				switch (eNotify)
				{
				case Notification_Type_Search_File_Process:
					std::cout << "nPos:" <<nData<< std::endl;
					break;
				case Notification_Type_Search_File_TotalSize:
					std::cout << "Total:" << nData << std::endl;
					break;
				case Notification_Type_Search_File_Finish:
					std::cout << "Finish:" << nData << std::endl;
				case Notification_Type_Search_File_Failure:
					std::cout << "Fail:" << nData << std::endl;
					break;
				default:
					std::cout << "Error:" << std::endl;
					break;
				}				
			}
		}
	} //for
}
