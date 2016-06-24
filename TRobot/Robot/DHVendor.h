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

	static void CALLBACK BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser);
	static void CALLBACK PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
	static int CALLBACK PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser);

private:
	string GetLastErrorString();
	void timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd);
	void timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH);
	vector<time_range> MakeTimeRangeList(const time_range& range);

	

protected:
	HMODULE m_hMod;

	string m_strName;
	int m_dwPort;
	string m_strUser;
	string m_strPasswords;
	string m_strIP;
	NET_DEVICEINFO m_deviceInfo;
	map<__int32, string> m_channels;
	LLONG m_lLoginHandle;
	NET_TIME m_stime;
	NET_TIME m_etime;
	LONG m_pdownloadfile;
	string m_strPath;

	vector<RecordFile> m_files;
	vector<int> m_channelVec;

protected:
	pCLIENT_GetLastError m_pGetLastError;
	pCLIENT_QueryChannelName m_pGetChannel;
	pCLIENT_Init m_pInit;
	pCLIENT_Cleanup m_pUninit;
	pCLIENT_Login m_pLogin;
	pCLIENT_Logout m_pLogout;
	pCLIENT_QueryRecordFile m_pSearchFileByTime;
	pCLIENT_DownloadByRecordFile m_pDownloadByFile;
	pCLIENT_DownloadByTime m_pDownloadByTime;
	pCLIENT_GetDownloadPos m_pGetDownloadPos;
	pCLIENT_StopDownload m_pStopDownload;
	pCLIENT_PlayBackByRecordFile m_pPlayBackByFile;
	pCLIENT_PlayBackByTimeEx m_pPlayBackByTimeEx;
	pCLIENT_SeekPlayBack m_pSeekPlayBack;
	pCLIENT_PausePlayBack m_pPausePlayBack;
	pCLIENT_StopPlayBack m_pStopPlayBack;
};

