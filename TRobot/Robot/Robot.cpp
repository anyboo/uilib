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
//		time_range timeRangeSearch;
//		time_range timeRangeFile;
//		timeRangeSearch.start = 1466265600;
//		timeRangeSearch.end = 1466697599;
//
//#if 1
//		CJxjVendor jxjVendor;
//		Device jxjDev(&jxjVendor);
//		jxjDev.Init();
//		//jxjDev.Login("192.168.0.89", 3321, "admin", "admin");
//		//jxjDev.Search(0, timeRangeSearch);
//		//timeRangeFile.start = 1466384189;
//		//timeRangeFile.end = 1466385411;
//		//jxjDev.Download(0, timeRangeFile);
//		//jxjDev.PlayVideo(0, timeRangeFile);
//#else
//		CDZPVendor dzpVendor;
//		Device dzpDev(&dzpVendor);
//		dzpDev.Login("192.168.0.39", 34567, "admin", "");
//		dzpDev.Search(0, timeRangeSearch);
//		timeRangeFile.start = 1466266800;
//		timeRangeFile.end = 1466268000;
//		//dzpDev.Download(0, timeRangeFile);
//		dzpDev.PlayVideo(0, timeRangeFile);
//
//		std::string filename = "01-20160619-002000.h264";
//		//dzpDev.Download(0, filename);
//		//dzpDev.PlayVideo(0, filename);
//
//#endif

		std::vector<AbstractVendor*> pVendorList;
		CJxjVendor jxjVendor;
		CDZPVendor dzpVendor;
		//DH::DHVendor dhVendor;
		pVendorList.push_back(&jxjVendor);
		pVendorList.push_back(&dzpVendor);
		//pVendorList.push_back(&dhVendor);

		CSearchDevice searchDev;
		searchDev.Search(pVendorList);
		DEVICE_INFO_LIST devInfoList = searchDev.GetDeviceInfoList();

		if (devInfoList.size() > 0)
		{
			NET_DEVICE_INFO* devInfo = devInfoList[0];
			CLoginDevice loginDev;
			loginDev.Login("", devInfo->nPort, "admin", "", devInfo->pVendor);

			time_range timeRangeSearch;
			timeRangeSearch.start = 1467388800;
			timeRangeSearch.end = 1467475199;
			CDeviceManager::getInstance().getDevice(devInfo->szIp).Search(0, timeRangeSearch);

			//time_range timeRangeFile;
			//timeRangeFile.start = 1467383367; // jxj - 1467383367  dzp - 1467388800
			//timeRangeFile.end = 1467393447; // jxj - 1467393447  dzp - 1467390600
			//CDeviceManager::getInstance().getDevice(devInfo->szIp).Download(0, timeRangeFile);
			//timeRangeFile.start = 1467393447; // jxj - 1467393447  dzp - 1467390600
			//timeRangeFile.end = 1467402167; // jxj - 1467402167  dzp - 1467392400
			//CDeviceManager::getInstance().getDevice(devInfo->szIp).Download(0, timeRangeFile);
		}

		while (1)
		{
			::Sleep(100);
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