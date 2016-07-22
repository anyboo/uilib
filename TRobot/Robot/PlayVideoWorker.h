#pragma once

#include "Device.h"

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/NotificationQueue.h"

using Poco::Runnable;
using Poco::FastMutex;
using Poco::NotificationQueue;

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

