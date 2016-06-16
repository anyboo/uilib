#pragma once
#include <string>
#include <ctime>

class AbstractVendor
{
public:
	AbstractVendor();
	virtual ~AbstractVendor();

	virtual void Init(const std::string& ip, size_t port) = 0;
	virtual void Login(const std::string& user, const std::string& password) = 0;
	virtual void Logout() = 0;
	virtual void SearchAll() = 0;
	virtual void SearchByTime(const std::time_t& time) = 0;
	virtual void DownloadByTime(const std::time_t& time) = 0;
	virtual void DownloadByName(const std::string& filename) = 0;
	virtual void PlayVideo(const std::string& filename) = 0;
};

