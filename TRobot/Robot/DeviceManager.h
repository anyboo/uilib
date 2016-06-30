#pragma once

#include "device.h"
#include <vector>

class CDeviceManager
{
public:
	CDeviceManager();
	~CDeviceManager();

	static CDeviceManager& getInstance();

	void addDevice(Device* pDev);
	Device& getDevice(const std::string ip);
	void deleteDevice(const std::string ip);

private:
	std::vector<Device*> m_vDeviceList;

};

