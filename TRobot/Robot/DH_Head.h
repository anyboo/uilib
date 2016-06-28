#ifndef __H_HEAD__
#define __H_HEAD__

#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

#include < assert.h> 
#include <tchar.h>

#include "Depend/dhnetsdk.h"

// Json 
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ostreamwrapper.h"
#include "istreamwrapper.h"

using namespace rapidjson;

typedef DWORD(CALL_METHOD *pCLIENT_GetLastError)(void);
typedef BOOL(CALL_METHOD *pCLIENT_QueryChannelName)(LLONG lLoginID, char *pChannelName, int maxlen, int *nChannelCount, int waittime);
typedef BOOL(CALL_METHOD*pCLIENT_Init)(fDisConnect cbDisConnect, LDWORD dwUser);
typedef void (CALL_METHOD*pCLIENT_Cleanup)();
typedef LLONG(CALL_METHOD*pCLIENT_Login)(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error);
typedef BOOL(CALL_METHOD*pCLIENT_Logout)(LLONG lLoginID);
typedef BOOL(CALL_METHOD*pCLIENT_QueryRecordFile)(LLONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, LPNET_RECORDFILE_INFO nriFileinfo, int maxlen, int *filecount, int waittime, BOOL bTime);
typedef LLONG(CALL_METHOD*pCLIENT_DownloadByRecordFile)(LLONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, LDWORD dwUserData);
typedef LLONG(CALL_METHOD*pCLIENT_DownloadByTime)(LLONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char *sSavedFileName, fTimeDownLoadPosCallBack cbTimeDownLoadPos, LDWORD dwUserData);
typedef BOOL(CALL_METHOD *pCLIENT_GetDownloadPos)(LLONG lFileHandle, int *nTotalSize, int *nDownLoadSize);
typedef BOOL(CALL_METHOD*pCLIENT_StopDownload)(LLONG lFileHandle);
typedef LLONG(CALL_METHOD *pCLIENT_PlayBackByRecordFile)(LLONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, LDWORD dwUserData);
typedef LLONG(CALL_METHOD *pCLIENT_PlayBackByTimeEx)(LLONG lLoginID, int nChannelID, LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, LDWORD dwPosUser, fDataCallBack fDownLoadDataCallBack, LDWORD dwDataUser);

typedef BOOL(CALL_METHOD *pCLIENT_SeekPlayBack)(LLONG lPlayHandle, unsigned int offsettime, unsigned int offsetbyte);
typedef BOOL(CALL_METHOD *pCLIENT_PausePlayBack)(LLONG lPlayHandle, BOOL bPause);
typedef BOOL(CALL_METHOD *pCLIENT_StopPlayBack)(LLONG lPlayHandle);


static void CALLBACK PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	if (dwUser == 0)
	{
		return;
	}
}

static int CALLBACK PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser)
{
	if (dwUser == 0)
	{
		return -1;
	}
	
	return 1;
}



//单个视频文件信息
struct RecordFile
{
	RecordFile()
	{
		channel = 0;
		size = 0;
		pPrivateData = nullptr;
		PrivateDataDataSize = 0;
	}

	~RecordFile()
	{
		if (nullptr != pPrivateData)
		{
			delete pPrivateData;
			pPrivateData = nullptr;
			PrivateDataDataSize = 0;
		}
	}

	RecordFile(const RecordFile& other)
	{
		channel = other.channel;
		size = other.size;
		name = other.name;
		beginTime = other.beginTime;
		endTime = other.endTime;
		pPrivateData = nullptr;
		PrivateDataDataSize = 0;
		setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
	}

	RecordFile& operator= (const RecordFile& other)
	{
		if (&other == this)
		{
			return *this;
		}
		channel = other.channel;
		size = other.size;
		name = other.name;
		beginTime = other.beginTime;
		endTime = other.endTime;
		setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
		return *this;
	}

	void setPrivateData(void* pData, int size)
	{
		if (nullptr != pPrivateData)
		{
			delete pPrivateData;
			pPrivateData = nullptr;
			PrivateDataDataSize = 0;
		}


		if (size > 0)
		{
			pPrivateData = new char[size];
			PrivateDataDataSize = size;
			memcpy(pPrivateData, pData, size);
		}
	}
	//取得私有数据
	void* getPrivateData() const
	{
		return pPrivateData;
	}
	__int32 getPrivateDataSize() const
	{
		return PrivateDataDataSize;
	}

	__int32 channel;      //通道
	__int64 size;         //文件大小(byte)
	std::string  name;    //文件名称
	__time64_t beginTime; //本地时间
	__time64_t endTime;   //本地时间
	char* pPrivateData;   //私有数据
	__int32 PrivateDataDataSize;//私有数据大小
};

#endif 