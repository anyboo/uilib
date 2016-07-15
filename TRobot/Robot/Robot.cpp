// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#include "Poco/NotificationQueue.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"


using Poco::NotificationQueue;
using Poco::Runnable;
using Poco::FastMutex;
using Poco::Thread;
using Poco::ThreadPool;


#include <vector>
//#include "SearchVideo.h"

// #include "jxjVendor.h"
//#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

//#include "Device.h"

//#include "DownloadVideo.h"
//#include "SearchFileRunable.h"
#include "SearchFileWorker.h"
#include "AcquireSerachFileData.h"
//#include "SearchVideo.h"
//#include "ReciveUIQunue.h"


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
// 		//通过设备类来调用海康的SDK
// 		HKVendor hkObj;
// 		//初始化设备类
// 		Device dObj(&hkObj);
// 		//海康的登录
// 		dObj.Login("192.168.0.92", 8000, "admin", "admin123");
// 
// 		//视频文件查询类
// 		CSearchVideo svObj;
// 
// 		std::vector<size_t> channelList;
// 		size_t ta = 1;
// 		channelList.push_back(ta);
// 
// 		svObj.SearchFile("192.168.0.92", range, &dObj, channelList);
// 
// 		//保存文件查询的信息
// 		std::vector<readSearchVideo> RSVObj;
// 		svObj.ReadDataFromTable(RSVObj);

// 		//大华
// 		DHVendor Obj;
// 	 	time_range range;
// 		range.start = 1468080000;
// 	 	//range.end = 1466524800;
// 		range.end = 1468166400;
// 	 	//range.end = 1478833871;
// 		Obj.Init();
// 		Obj.StartSearchDevice();
// 		long lLogin = Obj.Login("192.168.0.55", 37777, "admin", "112233");
// 		Obj.Search(lLogin, 1, range);
// 		Obj.Download(lLogin, 1, range);
// 		//Obj.Download(lLogin, 0, "channel0-20160710000000-20160711000000-2", 0);
// 		Obj.PlayVideo(lLogin, 1, range);
// 		//Obj.PlayVideo(lLogin, 0,"channel0-20160710000000-20160711000000-0");
// 		//Obj.Logout(lLogin);
 
		//海康
		HKVendor HKObj;
		time_range range;
		range.start = 1467475200;
		//range.end = 1466524800;
		range.end = 1467561600;
		//range.end = 1478833871;

		HKObj.Init();
		long lLogin = HKObj.Login("192.168.0.22", 8000, "admin", "12345");
		HKObj.Search(lLogin, 1, range);
		//HKObj.Download(lLogin, 0, range);
		HKObj.Download(lLogin, 1, "ch01_00000000060000100", 0);
		//HKObj.PlayVideo(lLogin, 1, range);
		//HKObj.PlayVideo(lLogin, 1, "ch0003_00000000008000000");
		//HKObj.Logout(lLogin);
	

// 		//大华
// 		DHVendor DHObj;
// 		time_range range;
// 		range.start = 1467302400;
// 		range.end = 1467648000;
// 		//初始化设备类
// 		Device dObj(&DHObj);
// 		//大华的登录
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
// 		//视频文件下载类
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
	//初始化设备类
	Device dObj(&DHObj);
	//大华的登录
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