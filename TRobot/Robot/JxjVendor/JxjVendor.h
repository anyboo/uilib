#pragma once
#include "AbstractVendor.h"

#include <vector>
#include <time.h>

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

//������Ƶ�ļ���Ϣ
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
	//ȡ��˽������
	void* getPrivateData() const
	{
		return pPrivateData;
	}
	__int32 getPrivateDataSize() const
	{
		return PrivateDataDataSize;
	}

	__int32 channel;      //ͨ��
	__int64 size;         //�ļ���С(byte)
	std::string  name;    //�ļ�����
	__time64_t beginTime; //����ʱ��
	__time64_t endTime;   //����ʱ��
	char* pPrivateData;   //˽������
	__int32 PrivateDataDataSize;//˽�����ݴ�С
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

	void MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, const int beginNode, const int endNode, const int ssid, const std::time_t& start, const std::time_t& end);
	vector<time_range> MakeTimeRangeList(const time_range& range);
	void SearchUnit(const size_t channel, const time_range& range);
	void ReFreshVideoList(int channel, const time_range& range);
	void InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd);
	void AddSearchFileList(int channel);
	bool CheckFileExist(const RecordFile& file, const vector<RecordFile>& fileList);
	time_t MakeTimestampByJTime(JTime jTime);

	// Download Callback
	static int  __stdcall JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
	// Download Finish Handle
	static void CloseDownload();

	long m_hBhandle;

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
	long m_lRecHandle;
	static long m_lDownLoadStartTime;
	static long m_lDownLoadTotalTime;
	static int g_iDownLoadPos;

	static eErrCode m_errCode; // Error Code

	void* handle;
};
