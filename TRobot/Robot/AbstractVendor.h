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

	virtual void Init(const std::string& ip, size_t port) = 0;
	virtual void Login(const std::string& user, const std::string& password) = 0;
	virtual void Logout() = 0;
	virtual void SearchAll() = 0;
	virtual void Search(const size_t channel, const time_range& range) = 0;
	virtual void Download(const size_t channel, const time_range& range) = 0;
	virtual void PlayVideo(const size_t channel, const time_range& range) = 0;

	virtual void Download(const size_t channel, const std::string& filename) = 0;
	virtual void PlayVideo(const size_t channel, const std::string& filename) = 0;
	virtual void SetDownloadPath(const std::string& Root) = 0;
	virtual void throwException() = 0;
};

