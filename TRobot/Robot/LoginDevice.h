#pragma once

#include "Device.h"
#include "DeviceManager.h"

class CLoginDevice
{
public:
	CLoginDevice();
	~CLoginDevice();

	void Login(const std::string& ip, const int port, const std::string& userName, const std::string& password, AbstractVendor* pVendor);
	void Logout(AbstractVendor* pVendor);
};

