// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//#include <pthread.h>

#include "device.h"
#include "DeviceManager.h"
#include "SearchDevice.h"
#include "LoginDevice.h"

#include "JxjVendor.h"
#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
		DEVICE_INFO_SIMPLE_LIST devInfoSimpleList;
		NET_DEVICE_INFO_SIMPLE* devInfoSimple = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		//std::string strIP = "192.168.0.89";
		//int nPort = 3321;
		std::string strIP = "192.168.0.39";
		int nPort = 34567;
		memcpy(devInfoSimple->szIP, strIP.c_str(), strIP.length());
		devInfoSimple->nPort = nPort;
		devInfoSimpleList.push_back(devInfoSimple);

		VENDOR_LIST pVendorList;
		CJxjVendor jxjVendor;
		CDZPVendor dzpVendor;
		DHVendor dhVendor;
		HKVendor hkVendor;
		
		//pVendorList.push_back(&jxjVendor);
		pVendorList.push_back(&dzpVendor);
		//pVendorList.push_back(&dhVendor);
		//pVendorList.push_back(&hkVendor);
		pVendorList.push_back(&jxjVendor);
		

		CSearchDevice::getInstance().Init(pVendorList);
		CSearchDevice::getInstance().Search(devInfoSimpleList);
		DEVICE_INFO_LIST devInfoList = CSearchDevice::getInstance().GetDeviceInfoList();

		if (devInfoList.size() > 0)
		{
			NET_DEVICE_INFO* devInfo = devInfoList[1];
			bool isLogin = CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort);
			if (!isLogin)
			{
				return;
			}

			time_range timeRangeSearch;
			timeRangeSearch.start = 1467388800;
			timeRangeSearch.end = 1467475199;
			CDeviceManager::getInstance().getDevice(devInfo->szIp).Search(0, timeRangeSearch);
			RECORD_FILE_LIST listRecordFile = CDeviceManager::getInstance().getDevice(devInfo->szIp).GetRecordFileList();

			time_range timeRangeFile;
			for (auto recordFile : listRecordFile)
			{
				timeRangeFile.start = recordFile.beginTime; 
				timeRangeFile.end = recordFile.endTime;
				CDeviceManager::getInstance().getDevice(devInfo->szIp).Download(0, timeRangeFile);
			}

			while (listRecordFile.size() > 0)
			{
				::Sleep(100);
			}
		}

		return;
	}

	SECTION("Test Search videos from the device")
	{

	}

	SECTION("Test Download videos from the device by name")
	{

	}

	SECTION("Test Download videos from the device by time")
	{

	}

	SECTION("Test play videos online")
	{

	}

	SECTION("Test logout the device")
	{

	}
}