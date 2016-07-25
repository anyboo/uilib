#pragma once

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/NotificationQueue.h"
#include "Poco/AutoPtr.h"

#include "Device.h"
#include "NotificationPlayVideo.h"

using Poco::Runnable;
using Poco::FastMutex;
using Poco::NotificationQueue;
using Poco::AutoPtr;

class CPlayVideoWorker :
	public Runnable
{
public:
	CPlayVideoWorker(Device* pDev, const RecordFile& file, const HWND hWnd, NotificationQueue& queue);
	~CPlayVideoWorker();

	virtual void run();

private:
	NotificationQueue& m_queue;
	static FastMutex m_mutex;

	Device* m_pDev;
	RecordFile m_file;
	HWND m_hWnd;
};

