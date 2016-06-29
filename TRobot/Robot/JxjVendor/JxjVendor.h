#pragma once

#include "AbstractVendor.h"

#include <vector>
#include <time.h>
#include <fstream>

#include "CommonUtrl.h"
// Jxj Sdk
#include "inc\mb_api.h"
#include "inc\JNetSDK.h"
#include "inc\stdint.h"
#include "inc\Jtype.h"
#include "inc\AVPlayer.h"
#include "JxjVendor.h"

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

struct Record
{
	size_t channel;      
	size_t size;         
	std::string  name;   
	std::time_t beginTime; 
	std::time_t endTime;
	unsigned long duration;
	std::string strTimeSection;
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
	void SearchUnit(const size_t channel, const time_range& range);
	void ReFreshVideoList(int channel, const time_range& range);
	void AddSearchFileList(int channel);
	bool CheckFileExist(const Record& file, const std::vector<Record>& fileList);
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

	std::string m_strRoot;

	/* Login */
	long m_lLoginHandle; // Handle of Login Device
	std::string m_ip;	// Device IP Address
	size_t m_port;	// Device Port

	/* Search */
	JStoreLog m_storeLog;
	JRecodeType m_recordType;
	int	m_iBeginNode;
	int	m_iEndNode;
	int	m_iSsid;
	std::vector<Record> m_files;

	/* Download */
	static long m_lDownloadHandle; // Handle of Download
	static long m_lDownloadFileHandle;
	static long m_lRecHandle;
	static long m_lDownLoadStartTime;
	static long m_lDownLoadTotalTime;
	static int g_iDownLoadPos;

	/* PlayVideo */
	static int m_iPlayVideoChannel;

	static eErrCode m_errCode; // Error Code

	void* handle;
};
