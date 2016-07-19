// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN

#include <winsock2.h>
#include <windows.h>

#include "catch.hpp"

#include "LoginDevice.h"
#include "SearchDevice.h"
#include "DeviceManager.h"
#include "portscan.h"

#include "JXJVendor.h"
#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

#include "TestWindows.h"

DEVICE_INFO_SIMPLE_LIST GetDeviceInfoSimpleList()
{
	DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;

	vector<ScanPortRecord> scanResults;
	//获取指针
	QMSqlite *pDb = QMSqlite::getInstance();
	std::string sql = SELECT_ALL_SCAN_PORT;
	pDb->GetData(sql, scanResults);

	for (size_t i = 0; i < scanResults.size(); i++)
	{
		ScanPortRecord spr = scanResults[i];
		if (spr.get<1>() == 80)
		{
			continue;
		}

		NET_DEVICE_INFO_SIMPLE* devInfoSimple = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		std::string ip = spr.get<0>();
		memcpy(devInfoSimple->szIP, ip.c_str(), ip.length());
		devInfoSimple->nPort = spr.get<1>();
		listDeviceSimpleInfo.push_back(devInfoSimple);
	}

	return listDeviceSimpleInfo;
}

TEST_CASE("This is a demo", "[demo]")
{
#if 1
	SECTION("Test login the device")
	{	
		/************************* 初始化线程池 **********************/
		Poco::ThreadPool pool;

		/************************* 初始化播放窗口句柄 **********************/
		TestWindows::getInstance().Init();

		/************************* 初始化数据库 **********************/
		//获取指针
		QFileSqlite *pFileDb = QFileSqlite::getInstance();
		//删除表
		pFileDb->dropTable(DROP_LOGIN_DEVICE_INFO_TABLE);
		//创建记录表
		pFileDb->createTable(CREATE_LOGIN_DEVICE_INFO_TABLE);

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
		//DHVendor* dhVendor = new DHVendor();
		//HKVendor* hkVendor = new HKVendor();

		pVendorList.push_back(jxjVendor);
		pVendorList.push_back(dzpVendor);
	/*	pVendorList.push_back(dhVendor);
		pVendorList.push_back(hkVendor);*/

		/************************* 初始化IP列表 **********************/
		//std::cout << CCommonUtrl::getInstance().GetCurTime() << "Scan Port Start!" << std::endl;
		//NotificationQueue queuePortScan;
		//PortScan portScan(queuePortScan);
		////开始扫描
		//ThreadPool::defaultPool().start(portScan);

		DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;
		//while (true)
		//{
		//	Notification::Ptr pNf(queuePortScan.waitDequeueNotification());
		//	if (pNf)
		//	{
		//		ScanNotification::Ptr pWorkNf = pNf.cast<ScanNotification>();
		//		if (pWorkNf)
		//		{
		//			listDeviceSimpleInfo = GetDeviceInfoSimpleList();
		//			std::cout << CCommonUtrl::getInstance().GetCurTime() << "Scan Port Stop!" << std::endl;
		//			break;
		//		}
		//	}
		//}

		/************************* 设备发现类测试 **********************/
		std::cout << CCommonUtrl::getInstance().GetCurTime() << "Search Device Start!" << std::endl;
		NotificationQueue queueSearchDevice; // 设备发现消息队列
		CSearchDevice sd(pVendorList, listDeviceSimpleInfo, queueSearchDevice);
		pool.start(sd);
		//queueSearchDevice.enqueueNotification(new CNotificationSearchDevice(Notification_Type_Search_Device_Cancel));

		/************************* 设备管理类测试 **********************/
		NotificationQueue queueDeviceManager; // 设备管理消息队列
		CDeviceManager dm(pVendorList, queueDeviceManager);
		pool.start(dm);
		//queueDeviceManager.enqueueNotification(new CNotificationDeviceManager(Notification_Type_Device_Manager_Cancel));

		while (true)
		{
			Notification::Ptr pNf(NotificationQueue::defaultQueue().waitDequeueNotification());
			if (pNf)
			{
				CNotificationSearchDevice::Ptr pWorkNf = pNf.cast<CNotificationSearchDevice>();
				//CNotificationDeviceManager::Ptr pWorkNf = pNf.cast<CNotificationDeviceManager>();
				if (pWorkNf)
				{
					if (pWorkNf->GetNotificationType() == Notification_Type_Search_Device_Finish)
					{
						break;
					}
				}
			}
		}
		
		/************************* 设备登陆、登出测试 **********************/
#if 1
		std::cout << CCommonUtrl::getInstance().GetCurTime() << "Search Device Stop!" << std::endl;

		DEVICE_INFO_LIST devInfoList = CSearchDevice::GetDeviceInfoList();	
		for (size_t i = 0; i < devInfoList.size(); i++)
		{
			// 获取设备信息
			NET_DEVICE_INFO* devInfo = devInfoList[i];
			std::string ip(devInfo->szIp);
			//if (ip.compare("10.168.0.66") == 0)
			if (ip.compare("192.168.0.89") == 0)
			{
				// 登陆设备
				if (CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort))
				{
					::Sleep(100);

					Device* pDev = CLoginDevice::getInstance().GetDevice(ip);
					time_range timeSearch;
					timeSearch.start = 1468771200; //DZP - 1468771200
					timeSearch.end = 1468857599; //DZP - 1468857598
					pDev->Search(2, timeSearch);
					RECORD_FILE_LIST list = pDev->GetRecordFileList();

					time_range timePlay;
					timePlay.start = list[0].beginTime; //DZP - 1468771200
					timePlay.end = list[0].endTime; //DZP - 1468857599
					pDev->PlayVideo(TestWindows::getInstance().GetHWnd(),2, timePlay);
				}
			}			
		}

		while (true)
		{
			::Sleep(100);
		}
		return;
#endif
	}
#endif

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