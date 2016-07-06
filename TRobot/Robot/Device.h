#pragma once

#include "AbstractVendor.h"

#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <WinSock.h>

/*
Please make sure type of vendor before you create a Device Object
*/

typedef enum
{
	Login_Status_No = 0,
	Login_Status_Yes

}DeviceLoginStatus;

class AbstractVendor;

class Device
{
public:
	Device(const AbstractVendor* sdk);
	Device();
	~Device();

	void setSDK(AbstractVendor* sdk);
	void Init();
	bool Login(const std::string& ip, size_t port, const std::string& userName = "", const std::string& password = "");
	void Logout();
	void SetDownloadPath(const std::string& root);

	void SearchAll();
	void Search(const size_t channel, const time_range& range);
	void Download(const size_t channel, const time_range& range);
	void PlayVideo(const size_t channel, const time_range& range);
	void Download(const size_t channel, const std::string& fileName);
	void PlayVideo(const size_t channel, const std::string& fileName);

	void setChannel(const std::vector<size_t>& channelList); 
	size_t getMaxChannel(){ return m_iMaxChannel; }
	std::vector<size_t> getChannelList(){ return m_vChannelList; }

	std::string getIP(){ return m_sIP; }
	DeviceLoginStatus getLoginStatus(){ return m_eLoginStatus; }

	void StartSearchDevice();
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_pVendor->GetDeviceInfoList(); }
	void StopSearchDevice();

	void SetNextVendor(AbstractVendor* pVendor) { m_pNextVendor = pVendor; }
	AbstractVendor* GetNextVendor() { return m_pNextVendor; }
	
	RECORD_FILE_LIST GetRecordFileList(){ return m_pVendor->GetRecordFileList(); }
	
private:
	AbstractVendor* m_pVendor;
	AbstractVendor* m_pNextVendor;
	
	std::string m_sIP;
	size_t m_iPort;
	std::string m_sUserName;
	std::string m_sPassword;

	size_t m_iMaxChannel;
	std::vector<size_t> m_vChannelList;

	long m_lLoginHandle;
	DeviceLoginStatus m_eLoginStatus;
};
