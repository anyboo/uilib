// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "TestWindows.h"

#include "Device.h"
#include "SearchDevice.h"
#include "LoginDevice.h"
#include "SearchVideo.h"
#include "DownloadFile.h"
#include "NotificationCenter.h"

#include "JXJVendor.h"
#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
		/************************* 初始化消息中心 **********************/
		Poco::ThreadPool pool;
		pool.start(CNotificationCenter::getInstance());

		/************************* 初始化播放窗口句柄 **********************/
		TestWindows::getInstance().Init();

		/************************* 初始化数据库 **********************/
		//获取指针
		QMSqlite *pDb = QMSqlite::getInstance();
		//删除表
		pDb->dropTable(DROP_SEARCH_VIDEO_TABLE);
		//创建记录表
		pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);

		/************************* 初始化SDK厂商 **********************/
		VENDOR_LIST pVendorList;
		CJXJVendor* jxjVendor = new CJXJVendor();
		CDZPVendor* dzpVendor = new CDZPVendor();
		DHVendor* dhVendor = new DHVendor();
		HKVendor* hkVendor = new HKVendor();

		//pVendorList.push_back(jxjVendor);
		pVendorList.push_back(dzpVendor);
		//pVendorList.push_back(dhVendor);
		//pVendorList.push_back(hkVendor);

		/************************* 初始化IP列表 **********************/
		DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;
		std::string strIP = "";
		int nPort = 0;

		// DZP
#if 1
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
#if 0
		NET_DEVICE_INFO_SIMPLE* devInfoSimple4 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple4, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.96";
		nPort = 37777;
		memcpy(devInfoSimple4->szIP, strIP.c_str(), strIP.length());
		devInfoSimple4->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple4);
#endif

		/************************* 设备发现 **********************/
		CSearchDevice::getInstance().Init(pVendorList, listDeviceSimpleInfo);
		pool.start(CSearchDevice::getInstance());

		DEVICE_INFO_LIST devInfoList = CSearchDevice::getInstance().GetDeviceInfoList();
		while (devInfoList.size() == 0)
		{
			devInfoList = CSearchDevice::getInstance().GetDeviceInfoList();
			::Sleep(1000);
		}

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

		/************************* 文件搜索、下载、播放测试 **********************/
#if 1
		if (devInfoList.size() > 0)
		{
			// 获取设备信息
			NET_DEVICE_INFO* devInfo = devInfoList[0];
			// 登陆设备
			if (CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort))
			{
				// 获取登陆成功后的设备对象
				Device* pDev = CLoginDevice::getInstance().GetDevice(devInfo->szIp);

				// 搜索视频文件
				int m_channel = 0;
				time_range timeRangeSearch;
				timeRangeSearch.start = 1467820800; // 1467770400 - jxj  // 1467734400 - dh  // 1467820800 - dzp
				timeRangeSearch.end = 1467907199; // 1467777599 - jxj  // 1467905003 - dh  // 1467907199 - dzp
				std::vector<size_t> channelList;
				channelList.push_back(m_channel);
				CSearchVideo::getInstance().SearchFile(pDev, timeRangeSearch, channelList);
				// 获取搜索视频文件列表
				std::vector<readSearchVideo> fileList;
				CSearchVideo::getInstance().ReadDataFromTable(fileList);
				// 获取某一文件信息
				readSearchVideo sr;
				std::string fileName;
				int channel;
				time_range timeRangeItem;
				if (fileList.size() > 0)
				{
					sr = fileList[1];
					fileName = sr.get<0>();
					channel = sr.get<1>();
					timeRangeItem.start = sr.get<2>();
					timeRangeItem.end = sr.get<3>();
					CDownloadFile dlFile1(pDev, channel, timeRangeItem);
					//CDownloadFile dlFile1(pDev, channel, timeRangeItem, fileName);

					sr = fileList[2];
					fileName = sr.get<0>();
					channel = sr.get<1>();
					timeRangeItem.start = sr.get<2>();
					timeRangeItem.end = sr.get<3>();
					//CDownloadFile dlFile2(pDev, channel, timeRangeItem);
					//CDownloadFile dlFile2(pDev, channel, timeRangeItem, fileName);

					pool.start(dlFile1);
					//pool.start(dlFile2);					

					while (true)
					{
						::Sleep(100);
					}
				}
			}
		}

		return;
#endif
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