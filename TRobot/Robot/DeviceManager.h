#pragma once

#include "device.h"
#include <vector>

class CDeviceManager
{
public:
	CDeviceManager();
	~CDeviceManager();

	static CDeviceManager& getInstance();

	void AddDevice(Device* pDev);
	Device& GetDevice(const std::string ip);
	void DeleteDevice(const std::string ip);

private:
	std::vector<Device*> m_listDevice;

};

