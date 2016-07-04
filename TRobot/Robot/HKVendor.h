#ifndef __HKVENDOR_H__
#define __HKVENDOR_H__

#include "CommonUtrl.h"

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
	virtual DEVICE_INFO_LIST& GetDeviceInfoList();
	virtual void StopSearchDevice();
	virtual size_t GetMaxChannel();

private:
	std::string GetLastErrorString();
	//std::vector<time_range> MakeTimeRangeList(const time_range& range);
	void CreatePath(const size_t channel);

	void SaveSearchFileListToFile();
	std::string MakeStrByInteger(int data);
	std::string MakeStrTimeByTimestamp(time_t time);

	void WriteFileListToDB();

protected:
	std::string m_strName;
	int m_dwPort;
	std::string m_strUser;
	std::string m_strPasswords;
	std::string m_strIP;
	LONG m_lLoginHandle;
	int m_nChannels;

	std::vector<RecordFile> m_files;

	/* Search Device */
	DEVICE_INFO_LIST m_listDeviceInfo;
};

#endif

