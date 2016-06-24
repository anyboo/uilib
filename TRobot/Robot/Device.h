#pragma once
#include <string>
#include "AbstractVendor.h"
/*
Please make sure type of vendor before you create a Device Object
*/
class AbstractVendor;
class Device
{
public:
	Device(const std::string& ip, size_t port);
	~Device();

	void setVendor(const AbstractVendor* sdk);
	void Login(const std::string& user, const std::string& password);
	void Logout();
	void SearchAll();
	void Search(const size_t channel, const time_range& range);
	void Download(const size_t channel, const time_range& range);

	void PlayVideo(const size_t channel, const time_range& range);
	void SetDownloadPath(const std::string& Root);
protected:
	AbstractVendor* _vendor;
	void* handle;
	size_t channel;
	std::string user;
	std::string passwd;
	size_t port;
	std::string savedpath;
};
