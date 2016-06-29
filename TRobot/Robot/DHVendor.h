#pragma once

#include "AbstractVendor.h"
#include "DH_Head.h"

#include <string>
#include <vector>

class DHVendor :
	public AbstractVendor
{
public:
	DHVendor();
	~DHVendor();

	void Init(const std::string& ip, size_t port);
	void Login(const std::string& user, const std::string& password);
	void Logout();
	void SearchAll();
	void Search(const size_t channel, const time_range& range);
	void Download(const size_t channel, const time_range& range);
	void Download(const size_t channel, const std::string& filename);
	void PlayVideo(const size_t channel, const time_range& range);
	void PlayVideo(const size_t channel, const std::string& filename);
	void SetDownloadPath(const std::string& Root);
	void throwException();


private:
	static void CALLBACK BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser);
	static void CALLBACK BTDownLoadFile(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
	static void CALLBACK PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
	static int CALLBACK PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser);

	void SaveSearchFileListToFile();
	void LoadSearchFileListFromFile();
	std::string MakeStrByInteger(int data);
	std::string MakeStrTimeByTimestamp(time_t time);

	std::string GetLastErrorString();
	void timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd);
	void timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH);
	std::vector<time_range> MakeTimeRangeList(const time_range& range);
	void trTOnt(NET_TIME &ntStartTime, NET_TIME &ntEndTime, const time_range range);
	void CreatePath(const size_t channel);

protected:
	HMODULE m_hMod;

	//login & Init
	std::string m_strName;
	int m_dwPort;
	std::string m_strUser;
	std::string m_strPasswords;
	std::string m_strIP;


	NET_DEVICEINFO m_deviceInfo;
	LLONG m_lLoginHandle;
	LONG m_pdownloadfile;
	std::string m_strPath;
	int m_DHChannels;

	std::vector<RecordFile> m_files;
};

