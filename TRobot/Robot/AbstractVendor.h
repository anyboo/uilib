#pragma once
#include <string>
#include <ctime>

typedef
struct _time_range
{
	std::time_t start;
	std::time_t end;
} time_range, *ptime_range;

class AbstractVendor
{
public:
	virtual void Init() = 0;
	virtual long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password) = 0;
	virtual void Logout(const long loginHandle) = 0;

	virtual void SearchAll(const long loginHandle) = 0;
	virtual void Search(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void Download(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void PlayVideo(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void Download(const long loginHandle, const size_t channel, const std::string& filename) = 0;
	virtual void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename) = 0;

	virtual void SetDownloadPath(const std::string& Root) = 0;
	virtual void throwException() = 0;

};

