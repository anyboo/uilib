#pragma once

#include "CommonUtrl.h"

// JXJ SDK
#include "mb_api.h"
#include "j_sdk.h"
#include "stdint.h"
#include "Jtype.h"
#include "JNetSDK.h"
#include "AVPlayer.h"

class CJxjVendor :
	public AbstractVendor
{
public:
	CJxjVendor();
	~CJxjVendor();

	void Init();
	long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	void Logout(const long loginHandle);

	std::string GetDefUsearName(){ return m_sDefUserName; }
	std::string GetDefPassword(){ return m_sDefPassword; }

	void SetNextVendor(AbstractVendor* pVendor) { m_pNextVendor = pVendor; }
	AbstractVendor* GetNextVendor() { return m_pNextVendor; }

	NET_SDK_TYPE GetSDKType(){ return m_eSDKType; }
	void StartSearchDevice();
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }
	void StopSearchDevice();
	size_t GetMaxChannel(){ return m_iMaxChannel; }

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
	static int __stdcall ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam);

	// Search Device Callback
	static int __stdcall fcbJMBNotify(long lHandle, DWORD dwPortocol, int iErr, int iMsgID, LPCTSTR lpszDstID, void* pData, int iDataLen, void* pUserParam);

	// Search Callback
	void MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, const int beginNode, const int endNode, const int ssid, const std::time_t& start, const std::time_t& end);
	std::string MakeFileName(int channel, const std::string& startTime, const std::string& endTime);
	void SearchUnit(const long loginHandle, const size_t channel, const time_range& range);
	void ReFreshVideoList(const long loginHandle, int channel, const time_range& range);
	void AddSearchFileList(int channel);
	bool CheckFileExist(const RecordFile& file, const std::vector<RecordFile>& fileList);
	void WriteFileListToDB();

	// Download Callback
	static int  __stdcall JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
	static void CloseDownload();

	// PlayVideo Callback
	static int  __stdcall JRecStream(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
	static  DWORD PlayThreadFun(LPVOID lpThreadParameter);

private:
	AbstractVendor* m_pNextVendor;

	/* Init */
	std::string m_sLastError;
	std::string m_sRoot;
	NET_SDK_TYPE m_eSDKType;

	/* Login */
	size_t m_iMaxChannel;
	std::string m_sDefUserName;
	std::string m_sDefPassword;

	/* Search Device */
	long m_lSearchDeviceHandle;
	DEVICE_INFO_LIST m_listDeviceInfo;

	/* Search */
	JStoreLog m_storeLog;
	JRecodeType m_recordType;
	int	m_iBeginNode;
	int	m_iEndNode;
	int	m_iSsid;
	std::vector<RecordFile> m_files;

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
