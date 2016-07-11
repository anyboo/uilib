#ifndef __DOWNLOADVIDEO_H__
#define __DOWNLOADVIDEO_H__


#include <vector>
#include <string>

#include "Device.h"

#include "AcquireDataRunable.h"
#include "DownloadFileNotificationQueue.h"

#include "Poco/ThreadPool.h"
using Poco::ThreadPool;


typedef struct _DOWNLOADID
{
	int nID;
	std::string strFileName;
}DOWNLOADID, *LPDOWNLOADID;


typedef struct  
{
	std::vector<DOWNLOADID> vecInfo;
	size_t tChannel;
}SDK_DOWNLOAD_INFO, *LPSDK_DOWNLOAD_INFO;

class DownloadVideo
{
public:
	DownloadVideo();
	~DownloadVideo();
	//输入
	void SetDownloadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj);
	//输出
	void GetDownloadInfo();
	//停止下载
	bool StopDownLoad(Device *dObj);

	void getFlieSizeTest(DWORD &dwDownLoadSize, DWORD &dwTotalSize);

private:
	//获得已下载文件大小和总文件大小
	int getDownloadPos(Device *dObj, DWORD dwDownLoadSize, DWORD dwTotalSize);
	//计算下载速度
	int CalDownLoadSpeed();
	//已下载时间
	time_t DownloadTime();
	//剩余时间
	time_t RemianTime();

private:
	AcquireDataRunable m_AcquireDataRunable;
};

#endif 


