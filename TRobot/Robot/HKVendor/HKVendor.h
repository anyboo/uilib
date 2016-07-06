#pragma once

#include "CommonUtrl.h"

class HKVendor :
	public AbstractVendor
{
public:
	HKVendor();
	~HKVendor();

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

	std::string GetDefUsearName(){ return m_sDefUserName; }
	std::string GetDefPassword(){ return m_sDefPassword; }

	NET_SDK_TYPE GetSDKType(){ return m_eSDKType; }
	void StartSearchDevice();
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }
	void StopSearchDevice();
	size_t GetMaxChannel(){ return m_iMaxChannel; }

	RECORD_FILE_LIST GetRecordFileList(){ return m_files; }

private:
	/*void timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd);
	void timeStdToDH(tm *pTimeStd, NET_DVR_TIME *pTimeDH);
	void trTOndt(NET_DVR_TIME &ndtStartTime, NET_DVR_TIME &ndtEndTime, const time_range range);

	std::string GetLastErrorString();
	std::vector<time_range> MakeTimeRangeList(const time_range& range);
	void CreatePath(const size_t channel);

	void SaveSearchFileListToFile();
	std::string MakeStrByInteger(int data);
	std::string MakeStrTimeByTimestamp(time_t time);

	void WriteFileListToDB();
	bool isGetDVRConfig();
	size_t getChannel(size_t channel);*/

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

	//std::string m_strName;
	//int m_dwPort;
	//std::string m_strUser;
	//std::string m_strPasswords;
	//std::string m_strIP;
	//LONG m_lLoginHandle;


	
};


