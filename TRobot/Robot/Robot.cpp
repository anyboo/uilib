// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "device.h"
#include "DeviceManager.h"
#include "SearchDevice.h"
#include "LoginDevice.h"

#include "jxjVendor.h"
#include "DZPVendor.h"
//#include "DHVendor.h"

#pragma comment(lib, "JXJVendor")
#pragma comment(lib, "DZPVendor")


TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
		DEVICE_INFO_SIMPLE_LIST devInfoSimpleList;
		NET_DEVICE_INFO_SIMPLE* devInfoSimple = new NET_DEVICE_INFO_SIMPLE;
		std::string strIP = "192.168.0.89";
		int nPort = 3321;
		memcpy(devInfoSimple->szIP, strIP.c_str(), strIP.length());
		devInfoSimple->nPort = nPort;
		devInfoSimpleList.push_back(devInfoSimple);

		VENDOR_LIST pVendorList;
		CJxjVendor jxjVendor;
		CDZPVendor dzpVendor;
		pVendorList.push_back(&jxjVendor);
		pVendorList.push_back(&dzpVendor);

		CSearchDevice::getInstance().Init(pVendorList);
		CSearchDevice::getInstance().Search(pVendorList, devInfoSimpleList);
		DEVICE_INFO_LIST devInfoList = CSearchDevice::getInstance().GetDeviceInfoList();

		if (devInfoList.size() > 0)
		{
			//NET_DEVICE_INFO* devInfo = devInfoList[0];
			//CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort);

			//time_range timeRangeSearch;
			//timeRangeSearch.start = 1467388800;
			//timeRangeSearch.end = 1467475199;
			//CDeviceManager::getInstance().getDevice(devInfo->szIp).Search(0, timeRangeSearch);

			//time_range timeRangeFile;
			//timeRangeFile.start = 1467383367; // jxj - 1467383367  dzp - 1467388800
			//timeRangeFile.end = 1467393447; // jxj - 1467393447  dzp - 1467390600
			//CDeviceManager::getInstance().getDevice(devInfo->szIp).Download(0, timeRangeFile);
			//timeRangeFile.start = 1467393447; // jxj - 1467393447  dzp - 1467390600
			//timeRangeFile.end = 1467402167; // jxj - 1467402167  dzp - 1467392400
			//CDeviceManager::getInstance().getDevice(devInfo->szIp).Download(0, timeRangeFile);

			//while (1)
			//{
			//	::Sleep(100);
			//}
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