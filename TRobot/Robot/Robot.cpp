// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//#include <pthread.h>

#include "device.h"
//#include "DeviceManager.h"
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
		DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;
		std::string strIP = "";
		int nPort = 0;

		// DZP
		NET_DEVICE_INFO_SIMPLE* devInfoSimple1 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple1, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.66";
		nPort = 34567;
		memcpy(devInfoSimple1->szIP, strIP.c_str(), strIP.length());
		devInfoSimple1->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple1);

		// JXJ
		NET_DEVICE_INFO_SIMPLE* devInfoSimple2 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple2, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.89";
		nPort = 3321;
		memcpy(devInfoSimple2->szIP, strIP.c_str(), strIP.length());
		devInfoSimple2->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple2);

		// HK
		NET_DEVICE_INFO_SIMPLE* devInfoSimple3 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple3, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.92";
		nPort = 8000;
		memcpy(devInfoSimple3->szIP, strIP.c_str(), strIP.length());
		devInfoSimple3->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple3);

		// DH
		NET_DEVICE_INFO_SIMPLE* devInfoSimple4 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple4, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.96";
		nPort = 37777;
		memcpy(devInfoSimple4->szIP, strIP.c_str(), strIP.length());
		devInfoSimple4->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple4);

		VENDOR_LIST pVendorList;
		CJxjVendor jxjVendor;
		CDZPVendor dzpVendor;
		DHVendor dhVendor;
		HKVendor hkVendor;
		
		pVendorList.push_back(&jxjVendor);
		pVendorList.push_back(&dzpVendor);
		pVendorList.push_back(&dhVendor);
		pVendorList.push_back(&hkVendor);

		CSearchDevice::getInstance().Search(pVendorList, listDeviceSimpleInfo);
		DEVICE_INFO_LIST devInfoList = CSearchDevice::getInstance().GetDeviceInfoList();
		for (auto devInfo : devInfoList)
		{
			CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort);
		}
		for (auto devInfo : devInfoList)
		{
			CLoginDevice::getInstance().Logout(devInfo->szIp);
		}

		if (devInfoList.size() > 0)
		{
			//NET_DEVICE_INFO* devInfo = devInfoList[2];
			//bool isLogin = CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort);
			//if (!isLogin)
			//{
			//	return;
			//}

	/*		time_range timeRangeSearch;
			timeRangeSearch.start = 1467388800;
			timeRangeSearch.end = 1467475199;
			CLoginDevice::getInstance().GetDevice(devInfo->szIp).Search(1, timeRangeSearch);
			RECORD_FILE_LIST listRecordFile = CLoginDevice::getInstance().GetDevice(devInfo->szIp).GetRecordFileList();

			time_range timeRangeFile;
			for (auto recordFile : listRecordFile)
			{
				timeRangeFile.start = recordFile.beginTime; 
				timeRangeFile.end = recordFile.endTime;
				CLoginDevice::getInstance().GetDevice(devInfo->szIp).Download(1, timeRangeFile);
			}

			while (listRecordFile.size() > 0)
			{
				::Sleep(100);
			}*/
		}
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