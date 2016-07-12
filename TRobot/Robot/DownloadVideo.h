#ifndef __DOWNLOADVIDEO_H__
#define __DOWNLOADVIDEO_H__

#include <vector>
#include <string>

#include "Device.h"

#include "AcquireDataRunable.h"
#include "DownloadFileNotificationQueue.h"

#include "Poco/ThreadPool.h"
using Poco::ThreadPool;

#define DOWNLOAD_FILESIZE 1024
#define MINUTE_DOWNLOAD 60
#define SECOND_DOWNLOAD 60

// typedef struct _DOWNLOADID
// {
// 	int nID;
// 	std::string strFileName;
// }DOWNLOADID, *LPDOWNLOADID;


// typedef struct _SDK_DOWNLOAD_INFO
// {
// 	std::vector<DOWNLOADID> vecInfo;
// 	size_t tChannel;
// }SDK_DOWNLOAD_INFO, *LPSDK_DOWNLOAD_INFO;

typedef struct _SDK_DOWNLOAD_INFO
{
	int nID;
	std::string strFileName;
	size_t tChannel;
}SDK_DOWNLOAD_INFO, *LPSDK_DOWNLOAD_INFO;


typedef struct _DOWNLOAD_OUTPUT_INFO
{
	int nID;
	std::string strFileName;
	std::string strFileSize;
	int nDownloadPos;
	std::string strSpeed;
	int strRemainingTime;
}DOWNLOAD_OUTPUT_INFO, *LPDOWNLOAD_OUTPUT_INFO;

class DownloadVideo
{
public:
	DownloadVideo();
	~DownloadVideo();
	// ‰»Î
	void SetDownloadInfo(Device *dObj, std::vector<SDK_DOWNLOAD_INFO> SDIObj);
	// ‰≥ˆ
	bool GetDownloadInfo(DOWNLOAD_OUTPUT_INFO &DownloadInfo);
	//Õ£÷πœ¬‘ÿ
	bool StopDownLoad(Device *dObj);

	/*void getFlieSizeTest(DWORD &dwDownLoadSize, DWORD &dwTotalSize);*/

private:

	std::string FileSizeToString(int nFileSize);
	std::string DownloadSpeedToString(int nSpeed);
	//std::string RemainingTimeToString(int nRemainingTime);

private:
	AcquireDataRunable m_AcquireDataRunable;
};

#endif 


