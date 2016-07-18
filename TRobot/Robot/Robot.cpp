// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include <winsock2.h>
#include <windows.h>

#include "catch.hpp"

//#include "Poco/NotificationQueue.h"
//#include "Poco/Runnable.h"
//#include "Poco/Mutex.h"
//#include "Poco/Random.h"
//#include "Poco/Thread.h"
//#include "Poco/ThreadPool.h"
//
//using Poco::NotificationQueue;
//using Poco::Runnable;
//using Poco::FastMutex;
//using Poco::Thread;
//using Poco::ThreadPool;
//
//#include <vector>
////#include "SearchVideo.h"
////#include "Device.h"
//
////#include "DownloadVideo.h"
////#include "SearchFileRunable.h"
//#include "SearchFileWorker.h"
//#include "AcquireSerachFileData.h"
////#include "SearchVideo.h"
////#include "ReciveUIQunue.h"

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
	SECTION("Test login the device")
	{
// 		HKVendor Obj;
// 		Obj.Init();
// 		Obj.Login("192.168.0.92", 8000, "admin", "admin123");
// 		Obj.Search(0, 33, range);
// 		Obj.Download(0, 33, range);
// 		time_range range;
// 		range.start = 1467475200;
// 		range.end = 1467561600;
// 
// 		//ͨ���豸�������ú�����SDK
// 		HKVendor hkObj;
// 		//��ʼ���豸��
// 		Device dObj(&hkObj);
// 		//�����ĵ�¼
// 		dObj.Login("192.168.0.92", 8000, "admin", "admin123");
// 
// 		//��Ƶ�ļ���ѯ��
// 		CSearchVideo svObj;
// 
// 		std::vector<size_t> channelList;
// 		size_t ta = 1;
// 		channelList.push_back(ta);
// 
// 		svObj.SearchFile("192.168.0.92", range, &dObj, channelList);
// 
// 		//�����ļ���ѯ����Ϣ
// 		std::vector<readSearchVideo> RSVObj;
// 		svObj.ReadDataFromTable(RSVObj);

// 		//��
// 		DHVendor Obj;
// 	 	time_range range;
// 		range.start = 1468080000;
// 	 	//range.end = 1466524800;
// 		range.end = 1468166400;
// 	 	//range.end = 1478833871;
// 		SDKDOWNLOADINFO info;
// 		
// 		Obj.Init();
// 		Obj.StartSearchDevice();
// 		long lLogin = Obj.Login("192.168.0.55", 37777, "admin", "112233");
// 		Obj.Search(lLogin, 1, range);
// 
// 		//info.tChannel = 1;
// 		info.trRange = range;
// // 		info.strFileName = "channel01-20160710000000-20160711000000-2";
// // 		info.trRange.start = -1;
// // 		info.trRange.end = -1;
// 		//Obj.Download(info);
// 		//Obj.Download(lLogin, 1, 0, info);
// 		//Obj.Download(lLogin, 0, "channel1-20160710000000-20160711000000-2", 0);
// 		Obj.PlayVideo(lLogin, 1, info);
// 		//Obj.PlayVideo(lLogin, 0,"channel0-20160710000000-20160711000000-0");
// 		//Obj.Logout(lLogin);
 
// 		//����
// 		HKVendor HKObj;
// 		time_range range;
// 		range.start = 1467475200;
// 		//range.end = 1466524800;
// 		range.end = 1467561600;
// 		//range.end = 1478833871;
// 
// 		HKObj.Init();
// 		long lLogin = HKObj.Login("192.168.0.22", 8000, "admin", "12345");
// 		HKObj.Search(lLogin, 0, range);
// 		//HKObj.Download(lLogin, 0, range);
// 		HKObj.Download(lLogin, 1, "ch01_00000000060000100", 0);
// 		//HKObj.PlayVideo(lLogin, 1, range);
// 		//HKObj.PlayVideo(lLogin, 1, "ch0003_00000000008000000");
// 		//HKObj.Logout(lLogin);
	

// 		//��
// 		DHVendor DHObj;
// 		time_range range;
// 		range.start = 1467302400;
// 		range.end = 1467648000;
// 		//��ʼ���豸��
// 		Device dObj(&DHObj);
// 		//�󻪵ĵ�¼
// 		dObj.Login("192.168.0.55", 37777, "admin", "112233");
// 		dObj.Search(0, range);
// 		
// 
// 		SDK_DOWNLOAD_INFO sdi;
// 		sdi.tChannel = 0;
// 		sdi.strFileName.append("channel0-20160701000000-20160705000000-16");
// 		sdi.nID = 2;
// 
// 		std::vector<SDK_DOWNLOAD_INFO> SDIVector;
// 		SDIVector.push_back(sdi);
// 		
// 		//��Ƶ�ļ�������
// 		DownloadVideo dvObj;
// 		dvObj.SetDownloadInfo(&dObj, SDIVector);
// 
// 		DOWNLOAD_OUTPUT_INFO doi;
// 		DOWNLOAD_OUTPUT_INFO doiTemp;
// 		dvObj.GetDownloadInfo(doi);
// // 		std::cout << "----------nDownloadPos:"<<doi.nDownloadPos << std::endl;
// // 		std::cout << "----------nID:" << doi.nID << std::endl;
// // 		std::cout << "----------strFileName:" << doi.strFileName << std::endl;
// // 		std::cout << "----------strFileSize:" << doi.strFileSize << std::endl;
// // 		std::cout << "----------strRemainingTime:" << doi.strRemainingTime << std::endl;
// // 		std::cout << "----------strSpeed:" << doi.strSpeed << std::endl;
// // 		doiTemp = doi;
// // 		dvObj.GetDownloadInfo(doi);
// 		while (!isEqual(doiTemp, doi))
// 		{
// 			std::cout << "----------nDownloadPos:" << doi.nDownloadPos << std::endl;
// 			std::cout << "----------nID:" << doi.nID << std::endl;
// 			std::cout << "----------strFileName:" << doi.strFileName << std::endl;
// 			std::cout << "----------strFileSize:" << doi.strFileSize << std::endl;
// 			std::cout << "----------strRemainingTime:" << doi.strRemainingTime << std::endl;
// 			std::cout << "----------strSpeed:" << doi.strSpeed << std::endl;
// 			std::cout << std::endl;
// 			doiTemp = doi;
// 
// 			dvObj.GetDownloadInfo(doi);
// 		}

/*
NotificationQueue& queue = NotificationQueue::defaultQueue();

AcquireSerachFileData AcquireData(queue);
//ThreadPool::defaultPool().start(AcquireData);


	DHVendor DHObj;
	time_range range;
	range.start = 1467302400;
	range.end = 1467648000;
	//��ʼ���豸��
	Device dObj(&DHObj);
	//�󻪵ĵ�¼
	dObj.Login("192.168.0.55", 37777, "admin", "112233");
	
	std::vector<size_t> channelList;
	channelList.push_back(0);
	channelList.push_back(1);

	ReciveUIQunue queue1;
 	//bool b = true;
 	//queue1.enqueueNotification(new ReciveUINotification(b));
	SearchFileWorker SearchWork(&dObj, range, channelList, queue1);
	
	ThreadPool::defaultPool().start(SearchWork);

	
	//NotificationQueue& queue = NotificationQueue::defaultQueue();
	
	//queue.enqueueNotification(new ReciveUINotification(b));
	
	

	

// 	while (!queue.empty())
// 	{
// 		Thread::sleep(200);
// 	}

	queue.wakeUpAll();
	ThreadPool::defaultPool().joinAll();

	*/

/************************* ��ʼ���̳߳� **********************/
Poco::ThreadPool pool;

/************************* ��ʼ�����Ŵ��ھ�� **********************/
TestWindows::getInstance().Init();

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
//pVendorList.push_back(dzpVendor);
//pVendorList.push_back(dhVendor);
//pVendorList.push_back(hkVendor);

/************************* ��ʼ��IP�б� **********************/
std::cout << CCommonUtrl::getInstance().GetCurTime() << "Scan Port Start!" << std::endl;
NotificationQueue queuePortScan;
PortScan portScan(queuePortScan);
//��ʼɨ��
ThreadPool::defaultPool().start(portScan);

DEVICE_INFO_SIMPLE_LIST listDeviceSimpleInfo;
while (true)
{
	Notification::Ptr pNf(queuePortScan.waitDequeueNotification());
	if (pNf)
	{
		ScanNotification::Ptr pWorkNf = pNf.cast<ScanNotification>();
		if (pWorkNf)
		{
			listDeviceSimpleInfo = GetDeviceInfoSimpleList();
			std::cout << CCommonUtrl::getInstance().GetCurTime() << "Scan Port Stop!" << std::endl;
			break;
		}
	}
}

/************************* �豸��������� **********************/
std::cout << CCommonUtrl::getInstance().GetCurTime() << "Search Device Start!" << std::endl;
NotificationQueue queueSearchDevice; // �豸������Ϣ����
CSearchDevice sd(pVendorList, listDeviceSimpleInfo, queueSearchDevice);
pool.start(sd);
//queueSearchDevice.enqueueNotification(new CNotificationSearchDevice(Notification_Type_Search_Device_Cancel));

/************************* �豸��������� **********************/
NotificationQueue queueDeviceManager; // �豸������Ϣ����
CDeviceManager dm(pVendorList, queueDeviceManager);
pool.start(dm);
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
				break;
			}
		}
	}
}

/************************* �豸��½���ǳ����� **********************/
#if 1
std::cout << CCommonUtrl::getInstance().GetCurTime() << "Search Device Stop!" << std::endl;

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
			pDev->PlayVideo(TestWindows::getInstance().GetHWnd(), 2, timePlay);
		}
	}
}

while (true)
{
	::Sleep(100);
}
#endif
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

/*int main()
{
	CSearchVideo Obj;
	Device dObj;
	DHVendor DHObj;

	dObj.setSDK(&DHObj);
	time_range range;
	range.start = 1466438400;
	range.end = 1466629200;
	std::vector<readSearchVideo> rsv;
}*/