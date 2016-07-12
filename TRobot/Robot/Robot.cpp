// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include "SearchVideo.h"

// #include "jxjVendor.h"
//#include "DZPVendor.h"
#include "DHVendor.h"
#include "HKVendor.h"

#include "Device.h"

#include "DownloadVideo.h"


TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
// 		time_range range;
// 		range.start = 1467475200;
// 		//range.end = 1466524800;
// 		range.end = 1467561600;
// 		//range.end = 1478833871;
 
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
// 	 	range.start = 1467302400;
// 	 	//range.end = 1466524800;
// 	 	range.end = 1467648000;
// 	 	//range.end = 1478833871;
// 		Obj.Init();
// 		Obj.StartSearchDevice();
// 		long lLogin = Obj.Login("192.168.0.55", 37777, "admin", "112233");
// 		Obj.Search(lLogin, 0, range);
// 	//	Obj.Download(lLogin, 0, range);
// 		Obj.Download(lLogin, 0, "channel0-20160701000000-20160701235959-1");
// 		//Obj.PlayVideo(lLogin, range);
// 		//Obj.PlayVideo(lLogin, "channel0-20160621000000-20160621235959-0");
// 		Obj.Logout(lLogin);
	 
// 		//海康
// 		HKVendor HKObj;
// 		time_range range;
// 		range.start = 1467475200;
// 		//range.end = 1466524800;
// 		range.end = 1467561600;
// 		//range.end = 1478833871;
// 
// 		HKObj.Init();
// 		long lLogin = HKObj.Login("192.168.0.92", 8000, "admin", "admin123");
// 		HKObj.Search(lLogin, 1, range);
// 		HKObj.Download(lLogin, 1, range);
// 		//HKObj.Download(lLogin, 1, "ch0001_00000000137000400");
// 		HKObj.PlayVideo(lLogin, 1, range);
// 		//HKObj.PlayVideo(lLogin, 1, "ch0003_00000000008000000");
// 		HKObj.Logout(lLogin);
// 	

		//大华
		DHVendor DHObj;
		time_range range;
		range.start = 1467302400;
		range.end = 1467648000;
		//初始化设备类
		Device dObj(&DHObj);
		//大华的登录
		dObj.Login("192.168.0.55", 37777, "admin", "112233");
		dObj.Search(0, range);
		

		SDK_DOWNLOAD_INFO sdi;
		sdi.tChannel = 0;
		sdi.strFileName.append("channel0-20160702000000-20160702235959-3");
		sdi.nID = 2;

		std::vector<SDK_DOWNLOAD_INFO> SDIVector;
		SDIVector.push_back(sdi);
		
		//视频文件下载类
		DownloadVideo dvObj;
		dvObj.SetDownloadInfo(&dObj, SDIVector);

		DOWNLOAD_OUTPUT_INFO doi;
		DOWNLOAD_OUTPUT_INFO doiTemp;
		dvObj.GetDownloadInfo(doi);
// 		std::cout << "----------nDownloadPos:"<<doi.nDownloadPos << std::endl;
// 		std::cout << "----------nID:" << doi.nID << std::endl;
// 		std::cout << "----------strFileName:" << doi.strFileName << std::endl;
// 		std::cout << "----------strFileSize:" << doi.strFileSize << std::endl;
// 		std::cout << "----------strRemainingTime:" << doi.strRemainingTime << std::endl;
// 		std::cout << "----------strSpeed:" << doi.strSpeed << std::endl;
// 		doiTemp = doi;
// 		dvObj.GetDownloadInfo(doi);
		while (!isEqual(doiTemp, doi))
		//while (true)
		{
			std::cout << "----------nDownloadPos:" << doi.nDownloadPos << std::endl;
			std::cout << "----------nID:" << doi.nID << std::endl;
			std::cout << "----------strFileName:" << doi.strFileName << std::endl;
			std::cout << "----------strFileSize:" << doi.strFileSize << std::endl;
			std::cout << "----------strRemainingTime:" << doi.strRemainingTime << std::endl;
			std::cout << "----------strSpeed:" << doi.strSpeed << std::endl;
			std::cout << std::endl;
			doiTemp = doi;

			dvObj.GetDownloadInfo(doi);
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