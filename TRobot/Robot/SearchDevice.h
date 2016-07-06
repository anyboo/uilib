#pragma once

#include "Device.h"
#include "LoginDevice.h"

class CSearchDevice
{
public:
	CSearchDevice();
	~CSearchDevice();

	static CSearchDevice& getInstance();

	//void Init(VENDOR_LIST& pVendorList);
	void Search(VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& devInfoSimpleList);
	void LoginCheck(AbstractVendor* pVendor, const DEVICE_INFO_SIMPLE_LIST& devInfoSimpleList);
	void DeleteDeviceInfoList();
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }

protected:
	DEVICE_INFO_LIST m_listDeviceInfo;
};

