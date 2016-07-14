// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "TestWindows.h"

#include "Device.h"
#include "SearchDevice.h"
#include "DeviceManager.h"
#include "LoginDevice.h"

#include "JXJVendor.h"
#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
		/************************* ��ʼ���̳߳� **********************/
		Poco::ThreadPool pool;

		/************************* ��ʼ�����Ŵ��ھ�� **********************/
		TestWindows::getInstance().Init();

		/************************* ��ʼ�����ݿ� **********************/
		//��ȡָ��
		QFileSqlite *pDb = QFileSqlite::getInstance();
		//ɾ����
		pDb->dropTable(DROP_LOGIN_DEVICE_INFO_TABLE);
		//������¼��
		pDb->createTable(CREATE_LOGIN_DEVICE_INFO_TABLE);

		/************************* ��ʼ��SDK���� **********************/
		VENDOR_LIST pVendorList;
		CJXJVendor* jxjVendor = new CJXJVendor();
		CDZPVendor* dzpVendor = new CDZPVendor();
		DHVendor* dhVendor = new DHVendor();
		HKVendor* hkVendor = new HKVendor();

		//pVendorList.push_back(jxjVendor);
		pVendorList.push_back(dzpVendor);
		//pVendorList.push_back(dhVendor);
		//pVendorList.push_back(hkVendor);

		/************************* ��ʼ��IP�б� **********************/
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
#if 0
		NET_DEVICE_INFO_SIMPLE* devInfoSimple4 = new NET_DEVICE_INFO_SIMPLE;
		memset(devInfoSimple4, 0, sizeof(NET_DEVICE_INFO_SIMPLE));
		strIP = "192.168.0.96";
		nPort = 37777;
		memcpy(devInfoSimple4->szIP, strIP.c_str(), strIP.length());
		devInfoSimple4->nPort = nPort;
		listDeviceSimpleInfo.push_back(devInfoSimple4);
#endif

		/************************* �豸��������� **********************/
		NotificationQueue queueSearchDevice; // �豸������Ϣ����
		CSearchDevice sd(pVendorList, listDeviceSimpleInfo, queueSearchDevice);
		pool.start(sd);

		DEVICE_INFO_LIST devInfoList = CSearchDevice::GetDeviceInfoList();
		while (devInfoList.size() <= 0)
		{
			devInfoList = CSearchDevice::GetDeviceInfoList();
			::Sleep(3000);
		}
		//queueSearchDevice.enqueueNotification(new CNotificationSearchDevice(Notification_Type_Search_Device_Cancel));

		/************************* �豸��������� **********************/
		NotificationQueue queueDeviceManager; // �豸������Ϣ����
		CDeviceManager dm(pVendorList, queueDeviceManager);
		pool.start(dm);

		//queueDeviceManager.enqueueNotification(new CNotificationDeviceManager(Notification_Type_Device_Manager_Cancel));

		/************************* �豸��½���ǳ����� **********************/
#if 1
		for (size_t i = 0; i < devInfoList.size(); i++)
		{
			// ��ȡ�豸��Ϣ
			NET_DEVICE_INFO* devInfo = devInfoList[i];
			// ��½�豸
			if (CLoginDevice::getInstance().Login(devInfo->pVendor, devInfo->szIp, devInfo->nPort))
			{
				// ��ȡ��½�ɹ�����豸����
				//Device* pDev = CLoginDevice::getInstance().GetDevice(devInfo->szIp);
				//::Sleep(100);
				//CLoginDevice::getInstance().Logout(devInfo->szIp);
				::Sleep(100);
			}
		}

		::Sleep(1000);

		std::vector<Device*>& listDevice = CLoginDevice::getInstance().GetDeviceList();
		Device* pDev = listDevice[0];
		CLoginDevice::getInstance().Logout(pDev->getIP());

		//for (size_t i = 0; i < listDevice.size(); i++)
		//{
		//	Device* pDev = listDevice[i];
		//	CLoginDevice::getInstance().Logout(pDev->getIP());
		//}

		while (true)
		{
			::Sleep(100);
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