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
#include "SearchFileWorker.h"
#include "PlayVideoWorker.h"

#include "JXJVendor.h"
#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

#include "TestWindows.h"

DEVICE_INFO_SIMPLE_LIST GetDeviceInfoSimpleList()
{
	DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;

	vector<ScanPortRecord> scanResults;
	//��ȡָ��
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
		/************************* ��ʼ�����ݿ� **********************/
		//��ȡָ��
		QFileSqlite *pFileDb = QFileSqlite::getInstance();
		//ɾ����
		pFileDb->dropTable(DROP_LOGIN_DEVICE_INFO_TABLE);
		//������¼��
		pFileDb->createTable(CREATE_LOGIN_DEVICE_INFO_TABLE);

		//��ȡָ��
		QMSqlite *pDb = QMSqlite::getInstance();
		//ɾ����
		pDb->dropTable(DROP_SEARCH_VIDEO_TABLE);
		//������¼��
		pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);

		/************************* ��ʼ��SDK���� **********************/
		VENDOR_LIST pVendorList;
		CJXJVendor* jxjVendor = new CJXJVendor();
		CDZPVendor* dzpVendor = new CDZPVendor();
		//DHVendor* dhVendor = new DHVendor();
		//HKVendor* hkVendor = new HKVendor();

		pVendorList.push_back(jxjVendor);
		pVendorList.push_back(dzpVendor);
		/*	pVendorList.push_back(dhVendor);
			pVendorList.push_back(hkVendor);*/

		/************************* ��ʼ��IP�б� **********************/
		DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;
		//std::cout << CCommonUtrl::getInstance().GetCurTime() << "Scan Port Start!" << std::endl;
		//NotificationQueue queuePortScan;
		//PortScan portScan(queuePortScan);
		////��ʼɨ��
		//ThreadPool::defaultPool().start(portScan);
		
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

		/************************* �豸��������� **********************/
		std::cout << CCommonUtrl::getInstance().GetCurTime() << "Search Device Start!" << std::endl;
		NotificationQueue* queueSearchDevice = new NotificationQueue(); // �豸������Ϣ����
		CSearchDevice sd(pVendorList, listDeviceSimpleInfo, *queueSearchDevice);
		ThreadPool::defaultPool().start(sd);
		//queueSearchDevice.enqueueNotification(new CNotificationSearchDevice(Notification_Type_Search_Device_Cancel));

		/************************* �豸��������� **********************/
		NotificationQueue* queueDeviceManager = new NotificationQueue(); // �豸������Ϣ����
		CDeviceManager dm(pVendorList, *queueDeviceManager);
		ThreadPool::defaultPool().start(dm);
		//queueDeviceManager.enqueueNotification(new CNotificationDeviceManager(Notification_Type_Device_Manager_Cancel));

		while (true)
		{
			Notification::Ptr pNf(NotificationQueue::defaultQueue().waitDequeueNotification());
			if (pNf)
			{			
				CNotificationSearchDevice::Ptr pWorkNf = pNf.cast<CNotificationSearchDevice>();				
				if (pWorkNf)
				{
					if (pWorkNf->GetNotificationType() == Notification_Type_Search_Device_Finish)
					{
						std::cout << CCommonUtrl::getInstance().GetCurTime() << "Search Device Stop!" << std::endl;
						break;
					}
				}
			}
		}

		/************************* �豸��½���ǳ����� **********************/
#if 1		
		DEVICE_INFO_LIST devInfoList = CSearchDevice::GetDeviceInfoList();
		for (size_t i = 0; i < devInfoList.size(); i++)
		{
			// ��ȡ�豸��Ϣ
			NET_DEVICE_INFO* devInfo = devInfoList[i];
			std::string ip(devInfo->szIp);
			//if (ip.compare("10.168.0.66") == 0)
			if (ip.compare("192.168.0.89") == 0)
			{
				// ��½�豸
				if (CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort))
				{					
					Device* pDev = CLoginDevice::getInstance().GetDevice(ip);

					// Search File
					int channel = 2;
					time_range timeSearch;
					timeSearch.start = 1469116800; //DZP - 1468771200  JXJ - 1469116800
					timeSearch.end = 1469203199; //DZP - 1468857598  JXJ - 1469203199					
					pDev->Search(channel, timeSearch);					
					std::vector<readSearchVideo> RSVObj;
					std::string strSql = SELECT_ALL_SEARCH_VIDEO;
					pDb->GetData(strSql, RSVObj);

					if (RSVObj.size() > 0)
					{
						readSearchVideo rsv = RSVObj[0];
						RecordFile file = CCommonUtrl::getInstance().MakeDBFileToRecordFile(rsv);													
					
						/************************* �ļ���������� **********************/
						TestWindows::getInstance().Init();
						NotificationQueue* queuePlayVideo = new NotificationQueue(); // �豸������Ϣ����
						CPlayVideoWorker pv(pDev, file, TestWindows::getInstance().GetHWnd(), *queuePlayVideo);
						ThreadPool::defaultPool().start(pv);
	/*					::Sleep(10000);
						queuePlayVideo->enqueueNotification(new CNotificationPlayVideo(Notification_Type_Play_Video_Pos, 50));
						::Sleep(10000);
						queuePlayVideo->enqueueNotification(new CNotificationPlayVideo(Notification_Type_Play_Video_Stop));	*/
						while (true)
						{
							::Sleep(3000);
							//queuePlayVideo->enqueueNotification(new CNotificationPlayVideo(Notification_Type_Play_Video_Pos, 10));
							//::Sleep(100);
						}
					}					
				}
			}
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