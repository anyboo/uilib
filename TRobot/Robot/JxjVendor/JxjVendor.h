#pragma once
#include "AbstractVendor.h"

#include <vector>
#include <time.h>
#include <fstream>

typedef enum
{
	Err_No = 0,
	Err_LoginSuccess, // Login Success
	Err_LoginFail,	// Login Fail
	Err_DownloadSuccess, // Download Success
}eErrCode;

typedef enum
{
	IsPlay_Download = 0,
	IsPlay_Play
}eIsPlay;

//单个视频文件信息
struct RecordFile
{
	RecordFile()
	{
		channel = 0;
		size = 0;
		duration = 0;
		strTimeSection = "";
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
		duration = other.duration;
		strTimeSection = other.strTimeSection;
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
		duration = other.duration;
		strTimeSection = other.strTimeSection;
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
	long duration;
	std::string strTimeSection;
	char* pPrivateData;   //私有数据
	__int32 PrivateDataDataSize;//私有数据大小
};

class CJxjVendor :
	public AbstractVendor
{
public:
	CJxjVendor();
	~CJxjVendor();

	virtual void Init(const std::string& ip, size_t port);
	virtual void Login(const std::string& user, const std::string& password);
	virtual void Logout();
	virtual void SearchAll();
	virtual void Search(const size_t channel, const time_range& range);
	virtual void Download(const size_t channel, const time_range& range);
	virtual void PlayVideo(const size_t channel, const time_range& range);

	virtual void Download(const size_t channel, const std::string& filename);
	virtual void PlayVideo(const size_t channel, const std::string& filename);
	virtual void SetDownloadPath(const std::string& Root);
	virtual void throwException();

protected:
	// Login Callback
	static int WINAPI ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam);

	// Search Callback
	void MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, const int beginNode, const int endNode, const int ssid, const std::time_t& start, const std::time_t& end);
	vector<time_range> MakeTimeRangeList(const time_range& range);
	void SearchUnit(const size_t channel, const time_range& range);
	void ReFreshVideoList(int channel, const time_range& range);
	void InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd);
	void AddSearchFileList(int channel);
	bool CheckFileExist(const RecordFile& file, const vector<RecordFile>& fileList);
	time_t MakeTimestampByJTime(JTime jTime);
	string MakeStrTimeByTimestamp(time_t time);
	string MakeStrByInteger(int data);
	void SaveSearchFileListToFile();
	void LoadSearchFileListFromFile();

	// Download Callback
	static int  __stdcall JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
	static void CloseDownload();

	// PlayVideo Callback
	static int  __stdcall JRecStream(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
	static  DWORD PlayThreadFun(LPVOID lpThreadParameter);

	/* Init */
	std::string  m_sLastError;

	string m_strRoot;

	/* Login */
	long m_lLoginHandle; // Handle of Login Device
	string m_ip;	// Device IP Address
	size_t m_port;	// Device Port

	/* Search */
	JStoreLog m_storeLog;
	JRecodeType m_recordType;
	int	m_iBeginNode;
	int	m_iEndNode;
	int	m_iSsid;
	vector<RecordFile> m_files;

	/* Download */
	static long m_lDownloadHandle; // Handle of Download
	static long m_lDownloadFileHandle;
	long m_lRecHandle;
	static long m_lDownLoadStartTime;
	static long m_lDownLoadTotalTime;
	static int g_iDownLoadPos;

	/* PlayVideo */
	static int m_iPlayVideoChannel;

	static eErrCode m_errCode; // Error Code

	void* handle;
};
