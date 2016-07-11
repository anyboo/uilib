#include "DownloadVideo.h"


DownloadVideo::DownloadVideo()
{
}


DownloadVideo::~DownloadVideo()
{
}


void DownloadVideo::GetDownLoadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj)
{
	if (NULL != dObj)
	{
		std::vector<SDK_DOWNLOAD_INFO>::iterator itSDI;
		for (itSDI != SDIObj.begin(); itSDI != SDIObj.end(); ++itSDI)
		{
			std::vector<std::string>::iterator itStr;
			for (itStr != itSDI->strVector.begin(); itStr != itSDI->strVector.end(); ++itStr)
			{
				//创建多线程，当下载到达100的时候关闭创建的线程
				dObj->Download(itSDI->tChannel, *itStr);
			}
		}
	}// if
}

bool DownloadVideo::StopDownLoad(Device *dObj)
{
	return dObj->StopDownload();
}

//获得已下载文件大小和总文件大小
int DownloadVideo::getDownloadPos(Device *dObj, int dDownLoadSize, int nTotalSize)
{
	if (NULL != dObj)
	{

	}

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
