#pragma once

#include "AbstractVendor.h"
#include "CommonUtrl.h"

class CJxjVendor :
	public AbstractVendor
{
public:
	CJxjVendor();
	~CJxjVendor();

	void Init();
	long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	void Logout(const long loginHandle);

	void SearchAll(const long loginHandle);
	void Search(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const time_range& range);
	void PlayVideo(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const std::string& filename);
	void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename);

	void SetDownloadPath(const std::string& Root);
	void throwException();

protected:
	// Login Callback
	static int WINAPI ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam);

	// Search Callback
	void MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, const int beginNode, const int endNode, const int ssid, const std::time_t& start, const std::time_t& end);
	void SearchUnit(const long loginHandle, const size_t channel, const time_range& range);
	void ReFreshVideoList(const long loginHandle, int channel, const time_range& range);
	void AddSearchFileList(int channel);
	bool CheckFileExist(const Record& file, const std::vector<Record>& fileList);

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
