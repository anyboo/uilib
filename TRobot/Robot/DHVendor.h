#pragma once
#include "AbstractVendor.h"
#include "DH_Head.h"

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
	void SearchByTime(const std::time_t& start, const std::time_t& end);
	void DownloadByTime(const std::time_t& start, const std::time_t& end);
	void DownloadByTime();
	void DownloadByName(const std::string& filename);
	void PlayVideo(const std::string& filename);
	void PlayVideoByTime(const std::time_t& start, const std::time_t& end);


private:
	string GetLastErrorString();
	void timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd);
	void timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH);
	


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

