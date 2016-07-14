
#include "SearchVideo.h"
#include <Poco/SingletonHolder.h>


CSearchVideo::CSearchVideo()
{
	m_bCancel = false;
}

CSearchVideo::~CSearchVideo()
{
}

CSearchVideo& CSearchVideo::getInstance()
{
	static Poco::SingletonHolder<CSearchVideo> shSearchVideo;
	return *shSearchVideo.get();
}

void CSearchVideo::SearchFile(Device* pDevice, const time_range& range, const std::vector<size_t> channelList)
{
		
	int nChannelList = channelList.size();

	NotificationQueue& queue = NotificationQueue::defaultQueue();
//	queue.enqueueNotification(new SearchFileNotification(EChannel, nChannelList));

	for (auto channel : channelList)
	{
		if (m_bCancel)
		{
			break;
		}

		pDevice->Search(channel, range);
	}

//	queue.enqueueNotification(new SearchFileNotification(EFinish, SEARCH_DEFAULT));

}

void CSearchVideo::CancelSearchFile()
{
	if (!m_bCancel)
	{
		m_bCancel = true;
	}
}

