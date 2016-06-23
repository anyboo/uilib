#pragma once
#include "AbstractVendor.h"

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
	virtual void SearchByTime(const std::time_t& start, const std::time_t& end);
	virtual void DownloadByTime(const std::time_t& start, const std::time_t& end);
	virtual void DownloadByName(const std::string& filename);
	virtual void PlayVideo(const std::string& filename);

protected:
	static int WINAPI ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam);
	static int  __stdcall JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
	void MakeStoreLog(JStoreLog& storeLog, 
		const JRecodeType recordType, const int beginNode,  const int endNode, const int ssid,
		const std::time_t& start, const std::time_t& end);
	static void CloseDownload();

	long m_hBhandle;

	/* Login */
	long m_lLoginHandle; // Handle of Login Device
	string m_ip;	// Device IP Address
	size_t m_port;	// Device Port
	int m_NVRChn = 0;

	/* Search */
	JStoreLog m_storeLog;
	JRecodeType m_recordType;
	int	m_iBeginNode;
	int	m_iEndNode;
	int	m_iSsid;
	Store m_store;

	/* Download */
	static long m_lDownloadHandle; // Handle of Download
	long m_lRecHandle;
	static long m_lDownLoadStartTime;
	static long m_lDownLoadTotalTime;
	static int g_iDownLoadPos;

	static eErrCode m_errCode; // Error Code

	void* handle;
};

