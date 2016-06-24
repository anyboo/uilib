#include "stdafx.h"
#include "Device.h"
#include <cassert>

Device::Device(const std::string& ip, size_t port)
{
	assert(_vendor);
}

Device::~Device()
{

}

void Device::setVendor(const AbstractVendor* sdk)
{
	if (!sdk) return;
	assert(_vendor);
	_vendor = const_cast<AbstractVendor*>(sdk);
}

void Device::Login(const std::string& user, const std::string& password)
{
	assert(_vendor);
	_vendor->Login(user, password);
}

void Device::Logout()
{
	assert(_vendor);
	_vendor->Logout();
}

void Device::SearchAll()
{
	assert(_vendor);
	_vendor->SearchAll();
}

void Device::Search(const size_t channel, const time_range& range)
{
	assert(_vendor);
	_vendor->Search(channel, range);
}

void Device::Download(const size_t channel, const time_range& range)
{
	assert(_vendor);
	_vendor->Download(channel, range);
}

void Device::PlayVideo(const size_t channel, const time_range& range)
{
	assert(_vendor);
	_vendor->PlayVideo(channel, range);
}

void Device::SetDownloadPath(const std::string& Root)
{
	savedpath = Root;
}