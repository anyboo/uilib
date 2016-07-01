#pragma once

#include "AbstractVendor.h"
#include "CommonUtrl.h"

#define CALL_METHOC  __cdecl

class CDZPVendor :
	public AbstractVendor
{
public:
	CDZPVendor();
	~CDZPVendor();

	void Init();
	long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password);
	void Logout(const long loginHandle);

	void StartSearchDevice();
	std::vector<NET_DEVICE_INFO*>& GetDeviceList(){ return m_listDeviceInfo; }
	void StopSearchDevice();

	void SearchAll(const long loginHandle);
	void Search(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const time_range& range);
	void PlayVideo(const long loginHandle, const size_t channel, const time_range& range);
	void Download(const long loginHandle, const size_t channel, const std::string& filename);
	void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename);

	void SetDownloadPath(const std::string& Root);
	void throwException();

protected:
	std::string MakeFileName(int channel, const std::string& startTime);
	void SearchUnit(const long loginHandle, const size_t channel, const time_range& range);
	static void __stdcall DownLoadPosCallBack(long lPlayHandle, long lTotalSize, long lDownLoadSize, long dwUser);
	static int __stdcall RealDataCallBack(long lRealHandle, long dwDataType, unsigned char *pBuffer, long lbufsize, long dwUser);
	int getDownloadPos(const long loginHandle);

protected:
	std::string  m_sLastError;

	/* Login */
	/* Search Device */
	long m_lSearchDeviceHandle;
	std::vector<NET_DEVICE_INFO*> m_listDeviceInfo;

	/* Search */
	std::vector<Record> m_files;

	void* handle;
};

