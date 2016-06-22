#include "stdafx.h"
#include "MyVendor.h"


MyVendor::MyVendor()
{
}


MyVendor::~MyVendor()
{
}


void MyVendor::Init(const std::string& ip, size_t port)
{
	throw std::exception("e");
}

void MyVendor::Login(const std::string& user, const std::string& password)
{

}

void MyVendor::Logout()
{

}

void MyVendor::SearchAll()
{

}

void MyVendor::SearchByTime(const std::time_t& start, const std::time_t& end)
{

}

void MyVendor::DownloadByTime(const std::time_t& start, const std::time_t& end)
{

}

void MyVendor::DownloadByName(const std::string& filename)
{

}

void MyVendor::PlayVideo(const std::string& filename)
{

}

// #include "catch.hpp"
// TEST_CASE_METHOD(MyVendor, "Init SDK","[Init]")
// {
// 	REQUIRE_THROWS(Init("127.0.0.1", 3000));
// 	REQUIRE(handle != nullptr);
// }
// 
// TEST_CASE_METHOD(MyVendor, "Login Device", "[Login]")
// {
// 	REQUIRE_THROWS(Login("user", "password"));
// }
// 
// TEST_CASE_METHOD(MyVendor, "Logout Device", "[Logout]")
// {
// 	REQUIRE_THROWS(Logout());
// }
// 
// TEST_CASE_METHOD(MyVendor, "Search all videos from device", "[SearchAll]")
// {
// 	REQUIRE_THROWS(SearchAll());
// }
