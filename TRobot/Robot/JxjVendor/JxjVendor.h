#pragma once

#include "CommonUtrl.h"

class CJXJVendor :
	public AbstractVendor
{
public:
	CJXJVendor();
	~CJXJVendor();

	void Init();
	long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	void Logout(const long loginHandle);

	void SearchAll(const long loginHandle);
	void Search(const long loginHandle, const size_t channel, const time_range& range);
	void ClearLocalRecordFiles();
	void Download(const long loginHandle, const size_t channel, const time_range& range);
	void PlayVideo(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const std::string& filename);
	void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename);
	bool StopDownload(){ return true; }

	void SetHWnd(const HWND& hWnd){ m_hWnd = hWnd; }
	void SetDownloadPath(const std::string& Root);
	void throwException();

	std::string GetDefUsearName(){ return m_sDefUserName; }
	std::string GetDefPassword(){ return m_sDefPassword; }
	int GetDefPort() { return m_iDefPort; }

	NET_SDK_TYPE GetSDKType(){ return m_eSDKType; }
	bool IsSearchDeviceAPIExist(){ return m_bSearchDeviceAPI; }
	void StartSearchDevice();
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }
	void StopSearchDevice();
	size_t GetMaxChannel(){ return m_iMaxChannel; }

	RECORD_FILE_LIST GetRecordFileList(){ return m_files; }

private:
	/* Init */
	HWND m_hWnd;
	std::string m_sRoot;
	NET_SDK_TYPE m_eSDKType;
	bool m_bSearchDeviceAPI;

	/* Login */
	size_t m_iMaxChannel;
	std::string m_sDefUserName;
	std::string m_sDefPassword;
	int m_iDefPort;

	/* Search Device */
	long m_lSearchDeviceHandle;
	DEVICE_INFO_LIST m_listDeviceInfo;

	/* Search */
	RECORD_FILE_LIST m_files;
	RECORD_FILE_LIST m_files_Unit;

	void* handle;
};
