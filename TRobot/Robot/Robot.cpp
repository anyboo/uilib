// Robot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "catch.hpp"
#include "MyVendor.h"

TEST_CASE("This is a demo", "[demo]")
{
	MyVendor test;
	SECTION("Test login the device")
	{
		test.Init("127.0.0.1", 3000);
		test.Login("user", "password");
	}

	SECTION("Test Search videos from the device")
	{
		test.SearchAll();
		
		//std::currenttime();
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