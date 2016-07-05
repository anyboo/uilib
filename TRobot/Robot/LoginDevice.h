#pragma once

#include "Device.h"
#include "DeviceManager.h"

class CLoginDevice
{
public:
	CLoginDevice();
	~CLoginDevice();

	static CLoginDevice& getInstance();

	bool Login(AbstractVendor* pVendor, const std::string& ip, const int port, const std::string& userName = "", const std::string& password = "");
	void Logout(AbstractVendor* pVendor);
};

