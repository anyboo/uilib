#pragma once

#include "LoginDevice.h"

#include "Poco/Runnable.h"

using Poco::Runnable;


typedef std::vector<Device*> DEVICE_LIST;

class CSearchDevice 
	: public Runnable
{
public:
	CSearchDevice();
	~CSearchDevice();

	static CSearchDevice& getInstance();

	void Init(const VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfo);
	DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }

protected:
	void InitDeviceList(const VENDOR_LIST& pVendorList);
	void Search(const VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfo);
	void DeleteDeviceList();
	void DeleteDeviceInfoList();

	virtual void run();

private:
	DEVICE_INFO_LIST m_listDeviceInfo;
	DEVICE_LIST m_listDeviceKnown;
	DEVICE_LIST m_listDeviceUnknown;

	VENDOR_LIST m_pVendorList;
	DEVICE_INFO_SIMPLE_LIST m_listDeviceSimpleInfo;
};

