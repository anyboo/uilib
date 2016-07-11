#include "DownloadVideo.h"


DownloadVideo::DownloadVideo():
m_AcquireDataRunable(DownloadFileNotificationQueue::GetInstance().GetQueue())
{
	// start worker threads
	ThreadPool::defaultPool().start(m_AcquireDataRunable);
}


DownloadVideo::~DownloadVideo()
{
	//SendDataNotification::.wakeUpAll();
	DownloadFileNotificationQueue::GetInstance().GetQueue().wakeUpAll();
	ThreadPool::defaultPool().joinAll();
}


void DownloadVideo::SetDownloadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj)
{
	if ((NULL != dObj) && (0 != SDIObj.size()))
	{
		std::vector<SDK_DOWNLOAD_INFO>::iterator itSDI = SDIObj.begin();
		for (; itSDI != SDIObj.end(); ++itSDI)
		{
			std::vector<DOWNLOADID>::iterator itStr;
			for (itStr = itSDI->vecInfo.begin(); itStr != itSDI->vecInfo.end(); ++itStr)
			{
				//创建多线程，当下载到达100的时候关闭创建的线程
				dObj->Download(itSDI->tChannel, itStr->strFileName);
			}
		}
	}// if
}

void DownloadVideo::GetDownloadInfo()
{

}

bool DownloadVideo::StopDownLoad(Device *dObj)
{
	return dObj->StopDownload();
}

void DownloadVideo::getFlieSizeTest(DWORD &dwDownLoadSize, DWORD &dwTotalSize)
{
	dwDownLoadSize = m_AcquireDataRunable.GetDownloadFileInfo().dwDownLoadSize;
	dwTotalSize = m_AcquireDataRunable.GetDownloadFileInfo().dwTotalSize;
}

//获得已下载文件大小和总文件大小
int DownloadVideo::getDownloadPos(Device *dObj, DWORD dwDownLoadSize, DWORD dwTotalSize)
{
	return 0;
}

//计算下载速度
int CalDownLoadSpeed()
{

	return 0;
}

//已下载时间
time_t DownloadTime()
{
	return 0;
}

//剩余时间
time_t RemianTime()
{
	return 0;
}
