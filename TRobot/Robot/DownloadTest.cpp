
#include "DownloadTest.h"

CDownloadTest::CDownloadTest()
{
}

CDownloadTest::~CDownloadTest()
{
}

void CDownloadTest::Test()
{
	jxjVendor.Init("192.168.0.89", 3321);

	jxjVendor.Login("admin", "admin");

	time_range timeRange;
	timeRange.start = 1466265600;
	timeRange.end = 1466697599;
	jxjVendor.Search(0, timeRange);

	std::string filename = "channel00-20160619235245-20160620001144";
	jxjVendor.PlayVideo(0, filename);
}

#include "catch.hpp"

TEST_CASE_METHOD(CDownloadTest, "Init SDK", "[Test]")
{
	REQUIRE_NOTHROW(Test());
}

//TEST_CASE_METHOD(CJxjVendor, "Search videos by time_range from device", "[Download]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//
//	REQUIRE_NOTHROW(Login("admin", "admin"));
//
//	time_range timeRange;
//	timeRange.start = gTimeStart;
//	timeRange.end = gTimeEnd;
//	REQUIRE_NOTHROW(Search(0, timeRange));
//
//#ifdef Test_Filename
//	string filename = "channel00-20160619235245-20160620001144";
//	REQUIRE_NOTHROW(Download(0, filename));
//#else
//	time_t start = 1466351565;
//	time_t end = 1466352704;
//	timeRange.start = start;
//	timeRange.end = end;
//	REQUIRE_NOTHROW(Download(0, timeRange));
//#endif
//}