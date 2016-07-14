#ifndef __ACQUIRESEARCHFILEDATA_H__
#define __ACQUIRESEARCHFILEDATA_H__

#include "Poco/NotificationQueue.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/Thread.h"


/*#include "SendDataNotification.h"*/
#include "SearchFileNotification.h"

using Poco::NotificationQueue;
using Poco::Runnable;
using Poco::FastMutex;
using Poco::Thread;

#include <vector>
#include <ctime>

class AcquireSerachFileData :public Runnable
{
public:
	AcquireSerachFileData(NotificationQueue& queue);
	~AcquireSerachFileData();

	void run();

private:
	NotificationQueue& m_queue;
	static FastMutex   m_mutex;
};

#endif 

