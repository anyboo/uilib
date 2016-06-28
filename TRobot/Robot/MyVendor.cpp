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

void MyVendor::Search(const size_t channel, const time_range& range)
{

}

void MyVendor::Download(const size_t channel, const time_range& range)
{

}

void MyVendor::Download(const size_t channel, const std::string& filename)
{

}

void MyVendor::PlayVideo(const size_t channel, const time_range& range)
{

}

void MyVendor::PlayVideo(const size_t channel, const std::string& filename)
{

}

void MyVendor::SetDownloadPath(const std::string& Root)
{

}

void MyVendor::throwException()
{

}

#include "catch.hpp"
TEST_CASE_METHOD(MyVendor, "Init SDK","[Init]")
{
// 	REQUIRE_NOTHROW(Init("127.0.0.1", 3000));
// 	REQUIRE(handle != nullptr);
// 	REQUIRE_NOTHROW(Login("user", "password"));
// 	REQUIRE_NOTHROW(SearchAll());
// 	REQUIRE_NOTHROW(Logout());
}
