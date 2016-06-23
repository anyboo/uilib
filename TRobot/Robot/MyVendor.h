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
	virtual void SearchByTime(const std::time_t& start, const std::time_t& end);
	virtual void DownloadByTime(const std::time_t& start, const std::time_t& end);
	virtual void DownloadByName(const std::string& filename);
	virtual void PlayVideo(const std::string& filename);

protected:
	void* handle;
};

