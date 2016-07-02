#pragma once

#include "Device.h"

class CSearchDevice
{
public:
	CSearchDevice();
	~CSearchDevice();

	void DeleteDeviceInfoList();
	void Search(std::vector<AbstractVendor*> pVendorList);
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }

protected:
	DEVICE_INFO_LIST m_listDeviceInfo;
};

