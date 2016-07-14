#include "DownloadVideo.h"


DownloadVideo::DownloadVideo():
m_AcquireDataRunable(NotificationQueue::defaultQueue())
{
	// start worker threads
	ThreadPool::defaultPool().start(m_AcquireDataRunable);
}


DownloadVideo::~DownloadVideo()
{
	//SendDataNotification::.wakeUpAll();
	NotificationQueue::defaultQueue().wakeUpAll();
	ThreadPool::defaultPool().joinAll();
}


void DownloadVideo::SetDownloadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj)
{
	if ((NULL != dObj) && (0 != SDIObj.size()))
	{
		m_VecDownloadInputInfo = SDIObj;

		std::vector<SDK_DOWNLOAD_INFO>::iterator itSDI = SDIObj.begin();
		for (; itSDI != SDIObj.end(); ++itSDI)
		{
			dObj->Download(itSDI->tChannel, itSDI->strFileName, itSDI->nID);
		}
	}// if
}

bool DownloadVideo::GetDownloadInfo(DOWNLOAD_OUTPUT_INFO &DownloadInfo)
{
	DownloadInfo.nDownloadPos = m_AcquireDataRunable.GetDownloadPos();
	DownloadInfo.nID = m_AcquireDataRunable.GetID();
	DownloadInfo.strFileSize = this->FileSizeToString(m_AcquireDataRunable.GetDownloadFileInfo().dwTotalSize);
	DownloadInfo.strRemainingTime = m_AcquireDataRunable.GetRemainingTime();
	DownloadInfo.strSpeed = this->DownloadSpeedToString(m_AcquireDataRunable.GetDownloadSpeed());

	std::vector<SDK_DOWNLOAD_INFO>::iterator it = m_VecDownloadInputInfo.begin();

	for (; it != m_VecDownloadInputInfo.end(); ++it)
	{
		if (it->nID == DownloadInfo.nID)
		{
			DownloadInfo.strFileName = it->strFileName;
		}
	}

	if (100 == DownloadInfo.nDownloadPos)
	{
		return true;
	}
	
	return false;
}

bool DownloadVideo::StopDownLoad(Device *dObj)
{
	return dObj->StopDownload();
}

// void DownloadVideo::getFlieSizeTest(DWORD &dwDownLoadSize, DWORD &dwTotalSize)
// {
// 	dwDownLoadSize = m_AcquireDataRunable.GetDownloadFileInfo().dwDownLoadSize;
// 	dwTotalSize = m_AcquireDataRunable.GetDownloadFileInfo().dwTotalSize;
// }


std::string DownloadVideo::FileSizeToString(int nFileSize)
{
	char szBuf[128];
	std::string strFileSize;

	nFileSize = nFileSize / DOWNLOAD_FILESIZE;

	ZeroMemory(szBuf, 128);
	sprintf_s(szBuf, "%dMB", nFileSize);

	strFileSize.append(szBuf);

	return strFileSize;
}

std::string DownloadVideo::DownloadSpeedToString(int nSpeed)
{
	char szBuf[128];
	std::string strSpeed;

	ZeroMemory(szBuf, 128);
	sprintf_s(szBuf, "%dKB/S", nSpeed);

	strSpeed.append(szBuf);

	return strSpeed;
}

// std::string DownloadVideo::RemainingTimeToString(int nRemainingTime)
// {
// 	char szBuf[128];
// 	std::string strRemainingTime;
// 
// 	ZeroMemory(szBuf, 128);
// 
// 	if (nRemainingTime >= 0 && nRemainingTime < SECOND_DOWNLOAD)
// 	{
// 		sprintf_s(szBuf, "00分%d秒", nRemainingTime);
// 	}
// 	else if ((nRemainingTime / SECOND_DOWNLOAD < MINUTE_DOWNLOAD) && (nRemainingTime / SECOND_DOWNLOAD > 0))
// 	{
// 		int nMinute = nRemainingTime / SECOND_DOWNLOAD;
// 		int nSecond = nRemainingTime % SECOND_DOWNLOAD;
// 		sprintf_s(szBuf, "%d分%d秒", nMinute, nSecond);
// 	}
// 	else
// 	{
// 		int nHour = nRemainingTime / SECOND_DOWNLOAD / MINUTE_DOWNLOAD;
// 		int nMinute = (nRemainingTime - nHour * SECOND_DOWNLOAD * MINUTE_DOWNLOAD) / SECOND_DOWNLOAD;
// 		sprintf_s(szBuf, "%d时%d分", nHour, nMinute);
// 	}
// 
// 	strRemainingTime.append(szBuf);
// 
// 	return strRemainingTime;
// }
