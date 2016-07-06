#pragma once

#include "Device.h"
#include "LoginDevice.h"

typedef std::vector<Device*> DEVICE_LIST;

class CSearchDevice
{
public:
	CSearchDevice();
	~CSearchDevice();

	static CSearchDevice& getInstance();

	void Init(VENDOR_LIST& pVendorList);
	void Search(const DEVICE_INFO_SIMPLE_LIST& devInfoSimpleList);
	void DeleteDeviceList();
	void DeleteDeviceInfoList();

	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }

protected:
	DEVICE_INFO_LIST m_listDeviceInfo;
	DEVICE_LIST m_listDevice;
};

