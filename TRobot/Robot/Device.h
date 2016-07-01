#pragma once

#include "AbstractVendor.h"

#include <string>
#include <vector>

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

	void Init();
	void Login(const std::string& ip, size_t port, const std::string& userName, const std::string& password);
	void Logout();
	void SetDownloadPath(const std::string& root);

	void StartSearchDevice();
	std::vector<NET_DEVICE_INFO*> GetDeviceList(){ return m_pVendor->GetDeviceList(); }
	void StopSearchDevice();

	void SearchAll();
	void Search(const size_t channel, const time_range& range);
	void Download(const size_t channel, const time_range& range);
	void PlayVideo(const size_t channel, const time_range& range);
	void Download(const size_t channel, const std::string& fileName);
	void PlayVideo(const size_t channel, const std::string& fileName);

	void setChannel(const size_t maxChannel, const std::vector<size_t>& channelList); 
	size_t getMaxChannel(){ return m_iMaxChannel; }
	std::vector<size_t> getChannelList(){ return m_vChannelList; }
	std::string getIP(){ return m_sIP; }
	DeviceLoginStatus getLoginStatus(){ return m_eLoginStatus; }
	
private:
	AbstractVendor* m_pVendor;
	
	std::string m_sIP;
	size_t m_iPort;
	std::string m_sUserName;
	std::string m_sPassword;

	size_t m_iMaxChannel;
	std::vector<size_t> m_vChannelList;

	long m_lLoginHandle;
	DeviceLoginStatus m_eLoginStatus;
};
