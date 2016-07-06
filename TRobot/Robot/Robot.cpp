// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "SearchVideo.h"

// #include "jxjVendor.h"
//#include "DZPVendor.h"
#include "DHVendor.h"
//#include "HKVendor.h"

#include "Device.h"

TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
		time_range range;
		range.start = 1467475200;
		//range.end = 1466524800;
		range.end = 1467561600;
		//range.end = 1478833871;
 
// 		HKVendor Obj;
// 		Obj.Init();
// 		Obj.Login("192.168.0.92", 8000, "admin", "admin123");
// 		Obj.Search(0, 33, range);
// 		Obj.Download(0, 33, range);

// 		HKVendor hkObj;
// 		Device dObj(&hkObj);
// 		dObj.Login("192.168.0.92", 8000, "admin", "admin123");
// 
// 		CSearchVideo svObj;
// 		svObj.SearchFile("192.168.0.92", range, &dObj);
// 
// 		std::vector<readSearchVideo> RSVObj;
// 		svObj.ReadDataFromTable(RSVObj);
		

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