#ifndef __SEARCHFILERUNABLE_H__
#define __SEARCHFILERUNABLE_H__

#include "Poco/NotificationQueue.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/Thread.h"



using Poco::NotificationQueue;
using Poco::Runnable;
using Poco::FastMutex;
using Poco::Thread;

#include <vector>
#include <ctime>
#include <stdint.h>

#include "SearchFileNotification.h"




class SearchFileRunable :public Runnable
{
public:
	SearchFileRunable(NotificationQueue& queue);
	~SearchFileRunable();

	void run();

private:
	NotificationQueue& m_queue;
	static FastMutex   m_mutex;
};

#endif 
