#pragma once

#include "Device.h"

class CLoginDevice
{
public:
	CLoginDevice();
	~CLoginDevice();

	static CLoginDevice& getInstance();

	bool Login(AbstractVendor* pVendor, const std::string& ip, const int port, const std::string& userName = "", const std::string& password = "");
	void Logout(const std::string& ip);
	Device* GetDevice(const std::string ip);
	std::vector<Device*>& GetDeviceList(){ return m_listDevice; }

protected:
	void AddDevice(Device* pDev);
	void DeleteDevice(const std::string ip);

private:
	std::vector<Device*> m_listDevice;
};

