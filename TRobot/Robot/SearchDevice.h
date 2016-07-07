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
	void Search(const VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfo);
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }

protected:
	void InitDeviceList(const VENDOR_LIST& pVendorList);
	void DeleteDeviceList();
	void DeleteDeviceInfoList();

private:
	DEVICE_INFO_LIST m_listDeviceInfo;
	DEVICE_LIST m_listDeviceKnown;
	DEVICE_LIST m_listDeviceUnknown;
};

