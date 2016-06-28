#pragma once
#include "AbstractVendor.h"

class MyVendor :
	public AbstractVendor
{
public:
	MyVendor();
	~MyVendor();

	virtual void Init(const std::string& ip, size_t port);
	virtual void Login(const std::string& user, const std::string& password);
	virtual void Logout();
	virtual void SearchAll();
	virtual void Search(const size_t channel, const time_range& range);
	virtual void Download(const size_t channel, const time_range& range);
	virtual void Download(const size_t channel, const std::string& filename);
	virtual void PlayVideo(const size_t channel, const time_range& range);
	virtual void PlayVideo(const size_t channel, const std::string& filename);
	virtual void SetDownloadPath(const std::string& Root);
	virtual void throwException();

protected:
	void* handle;
	size_t channelCount;
};

