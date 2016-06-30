#pragma once

#include "AbstractVendor.h"
#include "CommonUtrl.h"

class CDZPVendor :
	public AbstractVendor
{
public:
	CDZPVendor();
	~CDZPVendor();

	void Init();
	long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	void Logout(const long loginHandle);

	void SearchAll(const long loginHandle);
	void Search(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const time_range& range);
	void PlayVideo(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const std::string& filename);
	void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename);

	void SetDownloadPath(const std::string& Root);
	void throwException();

protected:
	void SearchUnit(const long loginHandle, const size_t channel, const time_range& range);
	//typedef void(__stdcall *fDownLoadPosCallBack) (long lPlayHandle, long lTotalSize, long lDownLoadSize, long dwUser);
	static void __stdcall DownLoadPosCallBack(long lPlayHandle, long lTotalSize, long lDownLoadSize, long dwUser);

protected:
	std::string  m_sLastError;

	/* Login */

	/* Search */
	std::vector<Record> m_files;

	void* handle;
};

