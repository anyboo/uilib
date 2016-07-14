#pragma once

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

#include "Device.h"
#include "SearchFileNotification.h"
#include "ReciveUINotification.h"
#include "ReciveUIQunue.h"


#ifndef _TIME_RANGE
#define _TIME_RANGE

typedef struct _time_range
{
	std::time_t start;
	std::time_t end;
} time_range, *ptime_range;

#endif 

#ifndef __COMTIME
#define __COMTIME
typedef struct __ComTime
{
	uint8_t year;					//从1900开始, J_SDK_DEF_BASE_YEAR
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t weekday;				//详情见JWeekDay
}ComTime;
#endif 

#ifndef MAX_SEARCH_COUNT
#define MAX_SEARCH_COUNT 1000
#endif 

#ifndef ONE_DAY
#define ONE_DAY		 (24 * 60 * 60)
#endif 

#ifndef ONE_HOUR
#define ONE_HOUR	 (60 * 60)
#endif 

#ifndef ONE_MINUTE
#define ONE_MINUTE	 (60)
#endif 

class SearchFileWorker :public Runnable
{
public:
	SearchFileWorker(Device* pDevice, const time_range& range, const std::vector<size_t> channelList, ReciveUIQunue& queue);
	~SearchFileWorker();

	void run();

	std::vector<time_range> MakeTimeRangeList(const time_range& range);

private:
	ReciveUIQunue& m_queue;
	static FastMutex   m_mutex;

	time_range m_range;
	std::vector<size_t> m_channelList;
	Device* m_pDevice;

	bool m_bCancel;
};

