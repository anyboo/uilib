#pragma once

#include "Device.h"
#include "Devicemanager.h"

class CSearchDevice
{
public:
	CSearchDevice();
	~CSearchDevice();

	void Init(std::vector<AbstractVendor*> pVendorList);
	void StartSearchDevice();
	std::vector<NET_DEVICE_INFO*> GetDeviceList(){ return m_listDeviceInfo; }
	void StopSearchDevice();

protected:
	static long m_nRef;
	std::vector<NET_DEVICE_INFO*> m_listDeviceInfo;
};

