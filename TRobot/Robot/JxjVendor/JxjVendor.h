#pragma once
#include "AbstractVendor.h"

typedef enum
{
	Err_No = 0,
	Err_LoginSuccess, // Login Success
	Err_LoginFail,	// Login Fail
}eErrCode;

class CJxjVendor :
	public AbstractVendor
{
public:
	CJxjVendor();
	~CJxjVendor();

	//virtual void Init(const std::string& ip, size_t port);
	//virtual void Login(const std::string& user, const std::string& password);
	//virtual void Logout();
	//virtual void SearchAll();
	//virtual void SearchByTime(const std::time_t& time);
	//virtual void DownloadByTime(const std::time_t& time);
	//virtual void DownloadByName(const std::string& filename);
	//virtual void PlayVideo(const std::string& filename);

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

	long m_hBhandle;
	static bool m_bIsLoginOk;	// the Flag of Device Login Success 

	string m_ip;	// Device IP Address
	size_t m_port;	// Device Port

	JStoreLog m_storeLog;	
	int	m_iBeginNode;
	int	m_iEndNode;
	int	m_iSsid;
	long m_lLogin; // Handle of Login Device
	long m_downloadHandle; // Handle of Download
	long m_lRecHandle;

	static eErrCode m_errCode; // Error Code

	void* handle;
};

