#ifndef __HKVENDOR_H__
#define __HKVENDOR_H__


#include "HK_Head.h"
#include "HCNetSDK.h"
//#include "CommonUtrl.h"

class HKVendor :
	public AbstractVendor
{
public:
	HKVendor();
	~HKVendor();

	virtual void Init();
	virtual long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	virtual void Logout(const long loginHandle);
	virtual void SearchAll(const long loginHandle);
	virtual void Search(const long loginHandle, const size_t channel, const time_range& range);
	virtual void Download(const long loginHandle, const size_t channel, const time_range& range);
	virtual void PlayVideo(const long loginHandle, const size_t channel, const time_range& range);
	virtual void Download(const long loginHandle, const size_t channel, const std::string& filename);
	virtual void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename);
	virtual void SetDownloadPath(const std::string& Root);
	virtual void throwException();

	virtual void StartSearchDevice();
	virtual void StopSearchDevice();
	std::string GetDefUsearName(){ return m_strUser; }
	std::string GetDefPassword(){ return m_strPasswords; }
	NET_SDK_TYPE GetSDKType(){ return m_eSDKType; }
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }
	size_t GetMaxChannel(){ return m_HKChannels; }
	RECORD_FILE_LIST GetRecordFileList(){ return m_files; }

private:
	void timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd);
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
	size_t getChannel(size_t channel);

protected:
	std::string m_strName;
	int m_dwPort;
	std::string m_strUser;
	std::string m_strPasswords;
	std::string m_strIP;
	LONG m_lLoginHandle;
	int m_HKChannels;

	NET_SDK_TYPE m_eSDKType;

	std::vector<RecordFile> m_files;

	/* Search Device */
	DEVICE_INFO_LIST m_listDeviceInfo;

	LOCAL_DEVICE_INFO m_struDeviceInfo;
};

#endif

