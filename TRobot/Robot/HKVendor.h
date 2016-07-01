#pragma once
#include "AbstractVendor.h"

#include "DH_Head.h"
#include "HCNetSDK.h"


class HKVendor :
	public AbstractVendor
{
public:
	HKVendor();
	~HKVendor();

	virtual void Init();
	virtual long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	virtual void Logout(const long loginHandle);
	virtual void SearchAll(const long loginHandle);
	virtual void Search(const long loginHandle, const size_t channel, const time_range& range);
	virtual void Download(const long loginHandle, const size_t channel, const time_range& range);
	virtual void PlayVideo(const long loginHandle, const size_t channel, const time_range& range);
	virtual void Download(const long loginHandle, const size_t channel, const std::string& filename);
	virtual void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename);
	virtual void SetDownloadPath(const std::string& Root);
	virtual void throwException();
};

