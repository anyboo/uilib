#pragma once

#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <WinSock2.h>

#include "AbstractVendor.h"


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
	bool LoginChain(const NET_DEVICE_INFO_SIMPLE* pDevInfoSimple, int& indexVendor);
	bool Login(const std::string& ip, size_t port, const std::string& userName = "", const std::string& password = "");
	void Logout();

	void SetDownloadPath(const std::string& root);
	void SearchAll();
	void Search(const size_t channel, const time_range& range);
	void ClearLocalRecordFiles(){ assert(m_pVendor); return m_pVendor->ClearLocalRecordFiles(); }

	void Download(const RecordFile& recordFile);
	void PlayVideo(HWND hWnd, const RecordFile& recordFile);

	bool StopDownload(){ assert(m_pVendor); return m_pVendor->StopDownload(); }
	void SetPlayVideoPos(int pos){ assert(m_pVendor); m_pVendor->SetPlayVideoPos(pos); }
	void StopPlayVideo(){ assert(m_pVendor); m_pVendor->StopPlayVideo(); }

	// Device Search Model
	void StartSearchDevice();
	void StopSearchDevice();
	DEVICE_INFO_LIST& GetDeviceInfoList(){ assert(m_pVendor); return m_pVendor->GetDeviceInfoList(); }

	std::string getIP(){ return m_sIP; }
	size_t getPort(){ return m_iPort; }
	std::string getUserName(){ return m_sUserName; }
	std::string getPassword(){ return m_sPassword; }
	size_t getMaxChannel(){ return m_iMaxChannel; }
	DeviceLoginStatus getLoginStatus(){ return m_eLoginStatus; } // Get Login Status (Unused)
	void setLoginStatus(DeviceLoginStatus devLoginStatus){ m_eLoginStatus = devLoginStatus; }

	// Chain of Responsibility Pattern
	void SetNextDevice(Device* pDev) { m_pNextDev = pDev; }
	Device* GetNextDevice() { return m_pNextDev; }
	
	NET_SDK_TYPE GetSDKType(){ assert(m_pVendor); return m_pVendor->GetSDKType(); }
	AbstractVendor* GetSDK(){ return m_pVendor; }
	bool IsSearchDeviceAPIExist(){ assert(m_pVendor); return m_pVendor->IsSearchDeviceAPIExist(); }

//protected:
	//RECORD_FILE_LIST GetRecordFileList(){ return m_pVendor->GetRecordFileList(); }
	
private:
	AbstractVendor* m_pVendor;
	Device* m_pNextDev;
	
	std::string m_sIP;
	size_t m_iPort;
	std::string m_sUserName;
	std::string m_sPassword;

	size_t m_iMaxChannel;

	long m_lLoginHandle;
	DeviceLoginStatus m_eLoginStatus;
};
