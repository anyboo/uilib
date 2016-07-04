// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "SearchVideo.h"
#include "jxjVendor.h"
#include "DZPVendor.h"
#include "DHVendor.h"

// TEST_CASE("This is a demo", "[demo]")
// {
// 	SECTION("Test login the device")
// 	{
// 		CSearchVideo Obj;
// 		time_range range;
// 		range.start = 1466438400;
// 		range.end = 1466629200;
// 		std::vector<readSearchVideo> rsv;
// 
// 		Obj.SearchFile("192.168.0.96", range);
// 		Obj.ReadDataFromTable(rsv);
// 
// 		time_range timeRange;
// 		time_t gTimeStart = 1466265600; // 2016-6-19 00:00:00 1466352000
// 		time_t gTimeEnd = 1466697599; // 2016-6-23 23:59:59
// 		timeRange.start = gTimeStart;
// 		timeRange.end = gTimeEnd;
// 
// 		//CDeviceManager devMgr;
// 
// 		//CJxjVendor jxjVendor;
// 		//Device jxjDev(&jxjVendor);
// 		//jxjDev.Login("192.168.0.89", 3321, "admin", "admin");
// 		//devMgr.addDevice(&jxjDev);
// 
// 		//CDZPVendor dzpVendor;
// 		//Device dzpDev(&dzpVendor);
// 		//dzpDev.Login("192.168.0.39", 34567, "admin", "");
// 		//devMgr.addDevice(&dzpDev);
// 
// 		//CJxjVendor jxjVendor2;
// 		//Device jxjDev2(&jxjVendor);
// 		//jxjDev2.Login("192.168.0.89", 3321, "admin", "admin");
// 		//devMgr.addDevice(&jxjDev2);
// 
// 		//CDZPVendor dzpVendor2;
// 		//Device dzpDev2(&dzpVendor);
// 		//dzpDev2.Login("192.168.0.39", 34567, "admin", "");
// 		//devMgr.addDevice(&dzpDev2);
// 
// 		//devMgr.deleteDevice(dzpDev.getIP());
// 
// 		//jxjDev.Login("192.168.0.89", 3321, "admin", "admin");
// 		//jxjDev.Search(0, timeRange);
// 
// 		//CDZPVendor dzpVendor;
// 		//Device dzpDev(&dzpVendor);
// 		//dzpDev.Login("192.168.0.39", 34567, "admin", "");
// 		//dzpDev.Search(0, timeRange);
// 
// 		return;
// 	}
// 
// 	SECTION("Test Search videos from the device")
// 	{
// 
// 	}
// 
// 	SECTION("Test Download videos from the device by name")
// 	{
// 
// 	}
// 
// 	SECTION("Test Download videos from the device by time")
// 	{
// 
// 	}
// 
// 	SECTION("Test play videos online")
// 	{
// 
// 	}
// 
// 	SECTION("Test logout the device")
// 	{
// 
// 	}
// }

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