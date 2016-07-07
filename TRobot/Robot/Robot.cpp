// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//#include <pthread.h>

#include "Device.h"
#include "SearchDevice.h"
#include "LoginDevice.h"
#include "SearchVideo.h"

#include "JxjVendor.h"
#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
		/************************* 初始化数据库 **********************/
		//获取指针
		QMSqlite *pDb = QMSqlite::getInstance();
		//删除表
		pDb->dropTable(DROP_SEARCH_VIDEO_TABLE);
		//创建记录表
		pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);

		/************************* 初始化SDK厂商 **********************/
		VENDOR_LIST pVendorList;
		CJxjVendor jxjVendor;
		CDZPVendor dzpVendor;
		DHVendor dhVendor;
		HKVendor hkVendor;

		//pVendorList.push_back(&jxjVendor);
		//pVendorList.push_back(&dzpVendor);
		pVendorList.push_back(&dhVendor);
		//pVendorList.push_back(&hkVendor);

		/************************* 初始化IP列表 **********************/
		DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;
		std::string strIP = "";
		int nPort = 0;

		// DZP
#if 0
		NET_DEVICE_INFO_SIMPLE* devInfoSimple1 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple1, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.66";
		nPort = 34567;
		memcpy(devInfoSimple1->szIP, strIP.c_str(), strIP.length());
		devInfoSimple1->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple1);
#endif

		// JXJ
#if 0
		NET_DEVICE_INFO_SIMPLE* devInfoSimple2 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple2, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.89";
		nPort = 3321;
		memcpy(devInfoSimple2->szIP, strIP.c_str(), strIP.length());
		devInfoSimple2->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple2);
#endif

		// HK
#if 0
		NET_DEVICE_INFO_SIMPLE* devInfoSimple3 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple3, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.92";
		nPort = 8000;
		memcpy(devInfoSimple3->szIP, strIP.c_str(), strIP.length());
		devInfoSimple3->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple3);
#endif

		// DH
#if 1
		NET_DEVICE_INFO_SIMPLE* devInfoSimple4 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple4, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.96";
		nPort = 37777;
		memcpy(devInfoSimple4->szIP, strIP.c_str(), strIP.length());
		devInfoSimple4->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple4);
#endif

		/************************* 设备发现 **********************/
		CSearchDevice::getInstance().Search(pVendorList, listDeviceSimpleInfo);
		DEVICE_INFO_LIST devInfoList = CSearchDevice::getInstance().GetDeviceInfoList();

		/************************* 设备登陆登出测试 **********************/
#if 0
		for (auto devInfo : devInfoList)
		{
			CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort);
		}
		for (auto devInfo : devInfoList)
		{
			CLoginDevice::getInstance().Logout(devInfo->szIp);
		}
#endif

		/************************* 文件搜索测试 **********************/
		if (devInfoList.size() > 0)
		{
			NET_DEVICE_INFO* devInfo = devInfoList[0];
			if (CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort))
			{
				time_range timeRangeSearch;
				timeRangeSearch.start = 1467734400;
				timeRangeSearch.end = 1467905003;
				std::vector<size_t> channelList;
				channelList.push_back(1);
				Device* pDev = CLoginDevice::getInstance().GetDevice(devInfo->szIp);
				CSearchVideo::getInstance().SearchFile(pDev, timeRangeSearch, channelList);

				std::vector<readSearchVideo> fileList;
				CSearchVideo::getInstance().ReadDataFromTable(fileList);
			}
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