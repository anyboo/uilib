// Robot.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "jxjVendor.h"
#include "DZPVendor.h"
#include "device.h"
#include "DeviceManager.h"

TEST_CASE("This is a demo", "[demo]")
{
	SECTION("Test login the device")
	{
		time_range timeRange;
		time_t gTimeStart = 1466265600; // 2016-6-19 00:00:00 1466352000 
		time_t gTimeEnd = 1466697599; // 2016-6-23 23:59:59
		timeRange.start = 1466265600;
		timeRange.end = 1466697599;


		//CJxjVendor jxjVendor;
		//Device jxjDev(&jxjVendor);
		//jxjDev.Login("192.168.0.89", 3321, "admin", "admin");
		//jxjDev.Search(0, timeRange);

		CDZPVendor dzpVendor;
		Device dzpDev(&dzpVendor);
		dzpDev.Login("192.168.0.39", 34567, "admin", "");
		dzpDev.Search(0, timeRange);

		timeRange.start = 1466265600;
		timeRange.end = 1466266800;
		dzpDev.Download(0, timeRange);
		while (1)
		{
			::Sleep(100);
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