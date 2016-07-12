#pragma once

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"

using Poco::Runnable;
using Poco::FastMutex;


class CNotificationCenter
	: public Runnable
{
public:
	CNotificationCenter();
	~CNotificationCenter();

	static CNotificationCenter& getInstance();

protected:
	virtual void run();

private:
	static FastMutex _mutex;
};

