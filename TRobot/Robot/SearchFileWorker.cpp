#include "SearchFileWorker.h"

FastMutex SearchFileWorker::m_mutex;

SearchFileWorker::SearchFileWorker(Device* pDevice, const time_range& range, const std::vector<size_t> channelList, ReciveUIQunue& queue) :
m_queue(queue)
{
	m_pDevice = pDevice;
	m_range = range;
	m_channelList = channelList;
	m_bCancel = false;
}


SearchFileWorker::~SearchFileWorker()
{
}

void SearchFileWorker::run()
{
	Poco::Random rnd;
	int nPos = 0;

	std::vector<time_range> trInfor = this->MakeTimeRangeList(m_range);

	int nDay = trInfor.size();
	int nChannelList = m_channelList.size();

	NotificationQueue& queue = NotificationQueue::defaultQueue();
	queue.enqueueNotification(new SearchFileNotification(Notification_Type_Search_File_TotalSize, nDay*nChannelList));

	for (auto channel : m_channelList)
	{
		for (auto day : trInfor)
		{
			m_pDevice->Search(channel, m_range);
// 			try
// 			{
// 				m_pDevice->Search(channel, m_range);
// 			}
// 			catch (std::string &strObj)
// 			{
// // 				if (strObj == "Search file by time failed")
// // 				{
// 					queue.enqueueNotification(new SearchFileNotification(Notification_Type_Search_File_Failure, SEARCHFILE_DEFAULT));
// 				//}
// 			}
			

			nPos++;
			//NotificationQueue& queue = NotificationQueue::defaultQueue();
			queue.enqueueNotification(new SearchFileNotification(Notification_Type_Search_File_Process, nPos));


			if (!m_queue.empty())
			{
				Notification::Ptr pNf(m_queue.waitDequeueNotification());
				if (pNf)
				{
					ReciveUINotification::Ptr pReciveDataNf = pNf.cast<ReciveUINotification>();
					if (pReciveDataNf)
					{
						{
							FastMutex::ScopedLock lock(m_mutex);
							m_bCancel = pReciveDataNf->GetData();
							std::cout << "Cancel£º" << m_bCancel << std::endl;
						}
					}
				}

				if (m_bCancel)
				{
					return;
				}

			}
			
		}
	}

	queue.enqueueNotification(new SearchFileNotification(Notification_Type_Search_File_Finish, SEARCHFILE_DEFAULT));
}

std::vector<time_range> SearchFileWorker::MakeTimeRangeList(const time_range& range)
{
	time_t timeStart = range.start;
	time_t timeEnd = range.end;
	std::vector<time_range> timeRangeList;

	ComTime jStartTime, jStopTime;

	struct tm Tm;

	localtime_s(&Tm, (const time_t*)&timeStart);
	jStartTime.year = Tm.tm_year;
	jStartTime.month = Tm.tm_mon;
	jStartTime.date = Tm.tm_mday;
	jStartTime.hour = Tm.tm_hour;
	jStartTime.minute = Tm.tm_min;
	jStartTime.second = Tm.tm_sec;
	jStartTime.weekday = Tm.tm_wday;

	localtime_s(&Tm, (const time_t*)&timeEnd);
	jStopTime.year = Tm.tm_year;
	jStopTime.month = Tm.tm_mon;
	jStopTime.date = Tm.tm_mday;
	jStopTime.hour = Tm.tm_hour;
	jStopTime.minute = Tm.tm_min;
	jStopTime.second = Tm.tm_sec;
	jStopTime.weekday = Tm.tm_wday;

	if (timeEnd - timeStart <= ONE_DAY)
	{
		if (jStartTime.date == jStopTime.date)
		{
			timeRangeList.push_back(range);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - jStartTime.hour) * ONE_HOUR + (59 - jStartTime.minute) * ONE_MINUTE + (59 - jStartTime.second);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			rangeItem.start = timeStart + diff + 1;
			rangeItem.end = timeEnd;
			timeRangeList.push_back(rangeItem);
		}
	}
	else
	{
		time_t diff = timeEnd - timeStart;
		int day = (diff / ONE_DAY) + (diff % ONE_DAY > 0 ? 1 : 0);

		if (jStartTime.hour == 0 && jStartTime.minute == 0 && jStartTime.second == 0)
		{
			for (size_t i = 0; i < day - 1; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
			}

			time_range rangeItem;
			rangeItem.start = timeStart;
			rangeItem.end = timeEnd;
			timeRangeList.push_back(rangeItem);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - jStartTime.hour) * ONE_HOUR + (59 - jStartTime.minute) * ONE_MINUTE + (59 - jStartTime.second);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			timeStart = timeStart + diff + 1;
			for (size_t i = 0; i < day - 2; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
			}

			if (timeEnd > timeStart + ONE_DAY - 1)
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
				rangeItem.start = timeStart;
				rangeItem.end = timeEnd;
				timeRangeList.push_back(rangeItem);
			}
			else
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeEnd;
				timeRangeList.push_back(rangeItem);
			}
		}
	}

	return timeRangeList;
}
