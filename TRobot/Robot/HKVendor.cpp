
#include "HKVendor.h"


HKVendor::HKVendor()
{
}


HKVendor::~HKVendor()
{
}

void HKVendor::Init()
{

}

long HKVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	return 0;
}

void HKVendor::Logout(const long loginHandle)
{

}

void HKVendor::SearchAll(const long loginHandle)
{

}

void HKVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{

}

void HKVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{

}

void HKVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{

}

void HKVendor::Download(const long loginHandle, const size_t channel, const std::string& filename)
{

}

void HKVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{

}

void HKVendor::SetDownloadPath(const std::string& Root)
{

}

void HKVendor::throwException()
{

}

