#pragma once

#include "CommonUtrl.h"

#include "SendDataNotification.h"
#include "SearchFileNotification.h"

class DHVendor :
	public AbstractVendor
{
public:
	DHVendor();
	~DHVendor();

	void Init();
	long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	void Logout(const long loginHandle);

	void SearchAll(const long loginHandle);
	void Search(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const time_range& range);
	void PlayVideo(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const std::string& filename, const int nID);
	void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename);
	bool StopDownload();

	void SetDownloadPath(const std::string& Root);
	void throwException();

	std::string GetDefUsearName(){ return m_sDefUserName; }
	std::string GetDefPassword(){ return m_sDefPassword; }
	
	NET_SDK_TYPE GetSDKType(){ return m_eSDKType; }
	void StartSearchDevice();
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }
	void StopSearchDevice();
	size_t GetMaxChannel(){ return m_iMaxChannel; }

	RECORD_FILE_LIST GetRecordFileList(){ return m_files; }

private:
	/* Init */
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
	RECORD_FILE_LIST m_files;
	RECORD_FILE_LIST m_FilesChange;
	int m_nPos;

	/*Download*/
	long m_lDownloadHandle;

	
};