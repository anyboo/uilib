#pragma once
#include "AbstractVendor.h"
#include "DH_Head.h"

#define MAX_SEARCH_COUNT 1000
#define ONE_DAY		 (24 * 60 * 60)
#define ONE_HOUR	 (60 * 60)
#define ONE_MINUTE	 (60)

class DHVendor :
	public AbstractVendor
{
public:
	DHVendor();
	~DHVendor();

	//void Init(const std::string& ip, size_t port);
	//void Login(const std::string& user, const std::string& password);
	//void Logout();
	//void SearchAll();
	//void Search(const size_t channel, const time_range& range);
	//void Download(const size_t channel, const time_range& range);
	//void Download(const size_t channel, const std::string& filename);
	//void PlayVideo(const size_t channel, const time_range& range);
	//void PlayVideo(const size_t channel, const std::string& filename);
	//void SetDownloadPath(const std::string& Root);
//	void throwException();

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


private:
	static void CALLBACK BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser);
	static void CALLBACK BTDownLoadFile(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
	static void CALLBACK PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
	static int CALLBACK PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser);

	void SaveSearchFileListToFile();
	void LoadSearchFileListFromFile();
	string MakeStrByInteger(int data);
	string MakeStrTimeByTimestamp(time_t time);

	string GetLastErrorString();
	void timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd);
	void timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH);
	vector<time_range> MakeTimeRangeList(const time_range& range);
	void trTOnt(NET_TIME &ntStartTime, NET_TIME &ntEndTime, const time_range range);
	void CreatePath(const size_t channel);

protected:
	HMODULE m_hMod;

	//login & Init
	string m_strName;
	int m_dwPort;
	string m_strUser;
	string m_strPasswords;
	string m_strIP;


	NET_DEVICEINFO m_deviceInfo;
	LLONG m_lLoginHandle;
	LONG m_pdownloadfile;
	string m_strPath;
	int m_DHChannels;

	vector<RecordFile> m_files;
};

