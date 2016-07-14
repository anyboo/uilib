#pragma once

#include "Device.h"
#include "NotificationNetworkStatus.h"
#include "NotificationSearchDevice.h"

#include "Poco/Runnable.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Mutex.h"
#include "Poco/AutoPtr.h"

using Poco::Runnable;
using Poco::NotificationQueue;
using Poco::FastMutex;
using Poco::AutoPtr;


typedef std::vector<Device*> DEVICE_LIST;

class CSearchDevice 
	: public Runnable
{
public:
	CSearchDevice(const VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfom, NotificationQueue& queue);
	~CSearchDevice();

	static DEVICE_INFO_LIST& GetDeviceInfoList(){ return m_listDeviceInfo; }

protected:
	void InitDeviceList(const VENDOR_LIST& pVendorList);
	void Search(const VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfo);
	void DeleteDeviceList();
	void DeleteDeviceInfoList();

	virtual void run();

private:
	static DEVICE_INFO_LIST m_listDeviceInfo;
	DEVICE_LIST m_listDeviceKnown;
	DEVICE_LIST m_listDeviceUnknown;

	VENDOR_LIST m_pVendorList;
	DEVICE_INFO_SIMPLE_LIST m_listDeviceSimpleInfo;

	NotificationQueue& m_queue;
	static FastMutex m_mutex;
};

