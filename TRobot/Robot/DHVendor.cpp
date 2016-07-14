#include "DHVendor.h"

//DH SDK
#include "dhnetsdk.h"

#pragma comment(lib, "dhnetsdk")

NET_DEVICEINFO m_deviceInfo;

void CALLBACK DH_BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser);
void CALLBACK DH_BTDownLoadFile(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
void CALLBACK DH_PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
int CALLBACK DH_PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser);

std::string DH_MakeStrByInteger(int data);
std::string DH_MakeStrTimeByTimestamp(time_t time);

std::string DH_GetLastErrorString();
void DH_timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd);
void DH_timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH);
void DH_trTOnt(NET_TIME &ntStartTime, NET_TIME &ntEndTime, const time_range range);
void DH_CreatePath(const size_t channel);
std::string HK_MakeFileName(size_t channel, const std::string& startTime, const std::string& endTime);



DHVendor::DHVendor()
{
	m_eSDKType = DH_SDK;
	m_sDefUserName = "admin";
	m_sDefPassword = "";
	m_iMaxChannel = 0;
	m_lDownloadHandle = 0;

	m_lSearchDeviceHandle = -1;

	m_nPos = 0;
}

DHVendor::~DHVendor()
{
	m_iMaxChannel = 0;
	m_lSearchDeviceHandle = -1;

	if (!m_files.empty())
	{
		m_files.clear();
	}
}

void DHVendor::Init()
{
	BOOL bInit = CLIENT_Init(0, 0);
	
	if (!bInit)
	{
		std::cout << "��ʼ������" << CLIENT_GetLastError() << std::endl;
		throw std::exception("Init failed");
	}
	else
	{
		std::cout << "��ʼ��SDK�ɹ���" << std::endl;
	}
}

long DHVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	int nError = 0;
	long lLoginHandle = -1;
	lLoginHandle = CLIENT_Login((char *)ip.c_str(), port, (char *)(user.c_str()), (char *)(password.c_str()), &m_deviceInfo, &nError);

	if (0 != nError)
	{
		std::cout << "��¼����(nError)��" << DH_GetLastErrorString().c_str() << std::endl;
		//throw std::exception("Login failed");
		return -1;
	}

	if (0 == lLoginHandle)
	{
		std::cout << "��¼����(lLogin)��" << DH_GetLastErrorString().c_str() << std::endl;
		return -1;
	}

	if (lLoginHandle > 0)
	{
		int nRetLen = 0;
		NET_DEV_CHN_COUNT_INFO stuChn = { sizeof(NET_DEV_CHN_COUNT_INFO) };
		stuChn.stuVideoIn.dwSize = sizeof(stuChn.stuVideoIn);
		stuChn.stuVideoOut.dwSize = sizeof(stuChn.stuVideoOut);

		BOOL bDevState = CLIENT_QueryDevState(lLoginHandle, DH_DEVSTATE_DEV_CHN_COUNT, (char*)&stuChn, stuChn.dwSize, &nRetLen, 1000);

		if (bDevState)
		{
			m_iMaxChannel = stuChn.stuVideoIn.nMaxTotal;
		}
		else
		{
			std::cout << "Get channel failed" << DH_GetLastErrorString() << std::endl;
			throw std::exception("Get channel failed");
			return -1;
		}
	}

	return lLoginHandle;
// 	for (int i = 1; i <= m_DHChannels; i++)
// 	{
// 		char szName[20];
// 		ZeroMemory(szName, sizeof(szName));
// 		cout << szName << "ͨ��" << i << endl;
// 		string strName = szName;
// 		m_channels.insert(std::make_pair(i, strName));
// 		m_channelVec.push_back(i);
// 	}
}

void DHVendor::Logout(const long loginHandle)
{
	if (loginHandle > 0 && !CLIENT_Logout(loginHandle))
	{
		std::cout << "�˳�����" << DH_GetLastErrorString().c_str() << std::endl;
		throw std::exception("Logout failed");
		return;
	}
}

void DHVendor::StartSearchDevice()
{
 	DEVICE_NET_INFO Device[256] = { 0 };
 	int nLength = 0;
 
 	BOOL bRet = CLIENT_SearchDevices((char *)Device, sizeof(DEVICE_NET_INFO) * 256, &nLength, 3000);
 
	if (bRet)
	{
		int i;
		for (i = 0; i < nLength / sizeof(DEVICE_NET_INFO); i++)
		{
			NET_DEVICE_INFO* ndiInfo = new NET_DEVICE_INFO();
			int nLen = 0;
			ndiInfo->nSDKType = DH_SDK;

			nLen = ((strlen(Device[i].szIP)) < MAX_IPADDR_LEN) ? strlen(Device[i].szIP) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo->szIp, Device[i].szIP, nLen);

			ndiInfo->nPort = Device[i].nPort;

			nLen = ((strlen(Device[i].szSubmask)) < MAX_IPADDR_LEN) ? strlen(Device[i].szSubmask) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo->szSubmask, Device[i].szSubmask, nLen);

			nLen = (strlen(Device[i].szMac) < MAX_MACADDR_LEN) ? strlen(Device[i].szMac) : MAX_MACADDR_LEN;
			memcpy(&ndiInfo->szMac, &Device[i].szMac, nLen);

			ndiInfo->pVendor = this;

			m_listDeviceInfo.push_back(ndiInfo);
		}
	}
}

void DHVendor::StopSearchDevice()
{

}

void DHVendor::SearchAll(const long loginHandle)
{

}

void timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd)
{
	pTimeStd->tm_year = pTimeDH->dwYear - 1900;
	pTimeStd->tm_mon = pTimeDH->dwMonth - 1;
	pTimeStd->tm_mday = pTimeDH->dwDay;
	pTimeStd->tm_hour = pTimeDH->dwHour;
	pTimeStd->tm_min = pTimeDH->dwMinute;
	pTimeStd->tm_sec = pTimeDH->dwSecond;
}

void timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH)
{
	pTimeDH->dwYear = pTimeStd->tm_year + 1900;
	pTimeDH->dwMonth = pTimeStd->tm_mon + 1;
	pTimeDH->dwDay = pTimeStd->tm_mday;
	pTimeDH->dwHour = pTimeStd->tm_hour;
	pTimeDH->dwMinute = pTimeStd->tm_min;
	pTimeDH->dwSecond = pTimeStd->tm_sec;
}

void DHVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	if (0 >= loginHandle)
	{
		std::cout << "���ȵ�¼!" << std::endl;
		return;
	}

	if (range.start >= range.end)
	{
		std::cout << "ʱ�䷶Χ����!" << std::endl;
		return;
	}

	std::vector<time_range> trInfor = CCommonUtrl::getInstance().MakeTimeRangeList(range);

	int nDay = trInfor.size();
	NotificationQueue& queue = NotificationQueue::defaultQueue();
	queue.enqueueNotification(new SearchFileNotification(EDay, nDay));

	std::vector<time_range>::iterator it;

	for (it = trInfor.begin(); it != trInfor.end(); ++it)
	{
		m_files.clear();

		NET_TIME ntStime;
		NET_TIME ntEtime;

		DH_trTOnt(ntStime, ntEtime, *it);

		NET_RECORDFILE_INFO ifileinfo[MAX_SEARCH_COUNT];
		ZeroMemory(ifileinfo, sizeof(ifileinfo));
		int iMaxNum = 0;

		BOOL bRet = CLIENT_QueryRecordFile(loginHandle, channel, 0, &ntStime, &ntEtime, 0, ifileinfo, sizeof(ifileinfo), &iMaxNum, 5000, true);

		if (!bRet)
		{
			queue.enqueueNotification(new SearchFileNotification(EFinish, SEARCH_DEFAULT));

			std::cout << "GetRecordFileList ��ѯ¼��ʧ�ܣ�����ԭ��" << DH_GetLastErrorString() << std::endl;
			throw std::exception("Search file by time failed");
		}
		if (iMaxNum <= 0)
		{
			std::cout << "GetRecordFileList ��ѯ¼��ɹ���¼�����Ϊ0" << std::endl;
		}

		NET_RECORDFILE_INFO item;
		RecordFile info;
		tm sTm;
		tm eTm;

		char szTime[512];

		for (int i = 0; i < iMaxNum; i++)
		{
			item = ifileinfo[i];

			timeDHToStd(&item.starttime, &sTm);
			timeDHToStd(&item.endtime, &eTm);

			ZeroMemory(szTime, 512);
			sprintf_s(szTime, "channel%d-%d%02d%02d%02d%02d%02d-%d%02d%02d%02d%02d%02d-%d", channel,
				ntStime.dwYear, ntStime.dwMonth, ntStime.dwDay,
				ntStime.dwHour, ntStime.dwMinute, ntStime.dwSecond,
				ntEtime.dwYear, ntEtime.dwMonth, ntEtime.dwDay, ntEtime.dwHour, ntEtime.dwMinute, ntEtime.dwSecond, i);

			info.channel = item.ch;
			info.size = item.size * 1024;
			info.name = szTime;

			info.beginTime = mktime(&sTm);
			info.endTime = mktime(&eTm);
			info.setPrivateData(&ifileinfo[i], sizeof(NET_RECORDFILE_INFO));
			m_files.push_back(info);

			std::cout << "GetRecordFileList �ļ���:" << info.name <<std::endl<< "  " << "�ļ���С:" << info.size << "  " << "ͨ��:" << info.channel << std::endl;
		}
		
		m_nPos++;
		std::cout << "----------m_nPos:" << m_nPos << std::endl;
		CCommonUtrl::getInstance().WriteFileListToDB(m_files);

		queue.enqueueNotification(new SearchFileNotification(EPos, m_nPos));
	}

	// Save Search Video List Result to Config File
	CCommonUtrl::getInstance().SaveSearchFileListToFile(m_files, Vendor_DH_Abbr);

	// Write File List to DB
	//CCommonUtrl::getInstance().WriteFileListToDB(m_files);
}

void DHVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{
	if (0 >= loginHandle)
	{
		std::cout << "���ȵ�¼!" << std::endl;
		return;
	}

	NET_TIME ntStime;
	NET_TIME ntEtime;

	DH_trTOnt(ntStime, ntEtime, range);

	DH_CreatePath(channel);

	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\��\\ͨ��%d\\channel%d-%d%02d%02d%02d%02d%02d-%d%02d%02d%02d%02d%02d.dav", channel, channel, ntStime.dwYear, ntStime.dwMonth, ntStime.dwDay,
		ntStime.dwHour, ntStime.dwMinute, ntStime.dwSecond, ntEtime.dwYear, ntEtime.dwMonth, ntEtime.dwDay, ntEtime.dwHour, ntEtime.dwMinute, ntEtime.dwSecond);

	long bRet = CLIENT_DownloadByTime(loginHandle, channel, 0, &ntStime, &ntEtime, szTime, DH_BTDownLoadPos, (DWORD)this);
	m_lDownloadHandle = bRet;

	std::cout << "strName:" << szTime << std::endl;

// 	int total, cur;
// 	total = 0;
// 	cur = 0;
// 	BOOL bret = CLIENT_GetDownloadPos(bRet, &total, &cur);
// 	while ((cur / total) != 1)
// 	{
// 		std::cout << "���ȣ�" << (double)(cur / total) << std::endl;
// 	}

	//bool c = CLIENT_PauseLoadPic(bRet, true);

	if (0 == bRet)
	{
		std::cout << "downLoadByRecordFile ����¼��ʧ�ܣ�����ԭ��" << DH_GetLastErrorString() << std::endl;
		throw std::exception("Download by Record file failed");
		return;
	}
	else
	{
		std::cout << "downLoadByRecordFile ����¼��ɹ���" << std::endl;
	}
}

void DHVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{
	if (0 >= loginHandle)
	{
		std::cout << "PlayVideo ���߲���ʧ��ԭ��" << DH_GetLastErrorString() << std::endl;
		throw std::exception("Login handle by Record file failed");
		return;
	}

	NET_TIME ntStime;
	NET_TIME ntEtime;

	DH_trTOnt(ntStime, ntEtime, range);

	TestWindows Test;
	Test.Init();

	BOOL lPlayID = CLIENT_PlayBackByTimeEx(loginHandle, channel, &ntStime, &ntEtime, g_hWnd, DH_PlayCallBack, (DWORD)this, DH_PBDataCallBack, (DWORD)this);

	//system("PAUSE");

	if (!lPlayID)
	{
		std::cout << "����ʧ��ԭ��" << DH_GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}
}

void DHVendor::Download(const long loginHandle, const size_t channel, const std::string& filename, const int nID)
{
	if (0 >= loginHandle)
	{
		std::cout << "���ȵ�¼!" << std::endl;
		return;
	}

	DH_CreatePath(channel);

	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\��\\ͨ��%d\\", channel);

	char szBuf[] = ".dav";
	strcat_s(szTime, (char *)filename.c_str());
	strcat_s(szTime, szBuf);

	std::cout << szTime << std::endl;

	std::vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{
			NET_RECORDFILE_INFO* pData = (NET_RECORDFILE_INFO*)it->getPrivateData();
			long lRet = CLIENT_DownloadByRecordFile(loginHandle, pData, szTime, DH_BTDownLoadFile, nID);
			m_lDownloadHandle = lRet;

// 			int total, cur;
// 			total = 0;
// 			cur = 0;
// 			int n = 0;
// 			BOOL bret = CLIENT_GetDownloadPos(bRet, &total, &cur);
// 			while ((cur / total) != 1)
// 			{
// 				std::cout << "���ȣ�" << (double)(cur / total) << std::endl;
// 				std::cout << "cur:" << cur<< std::endl;
// 				std::cout << "total:" << total << std::endl;
// 				bret = CLIENT_GetDownloadPos(bRet, &total, &cur);
// // 				if (10 == n)
// // 				{
// // 					BOOL b =  CLIENT_StopDownload(bRet);
// // 					std::cout << b << std::endl;	
// // 				}
// // 				n++;
// 			}


// 			bool c = CLIENT_PauseLoadPic(bRet, true);
// 			std::cout<<"��ͣʧ�ܣ�"<<DH_GetLastErrorString() << std::endl;

			if (0 == lRet)
			{
				std::cout << "downLoadByRecordFile ����¼��ʧ�ܣ�����ԭ��" << DH_GetLastErrorString() << std::endl;
				throw std::exception("Download by Record file failed");
				return;
			}
			else
			{
				std::cout << "downLoadByRecordFile ����¼��ɹ���" << std::endl;
			}
		}

	}
}

bool DHVendor::StopDownload()
{
	assert(m_lDownloadHandle);

	if (CLIENT_StopDownload(m_lDownloadHandle))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void DHVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (0 >= loginHandle)
	{
		cout << "PlayVideo ���߲���ʧ��ԭ��" << DH_GetLastErrorString() << endl;
		return;
	}

	std::vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{
			TestWindows Test;
			Test.Init();

			NET_RECORDFILE_INFO* pData = (NET_RECORDFILE_INFO*)it->getPrivateData();
			BOOL lPlayID = CLIENT_PlayBackByRecordFile(loginHandle, pData, g_hWnd, DH_PlayCallBack, (DWORD)this);

			system("PAUSE");

			if (!lPlayID)
			{
				std::cout << "����ʧ��ԭ��" << DH_GetLastErrorString() << std::endl;
				throw std::exception("Play back by time failed");
			}
		}

		if (m_files.size() - 1 == nSize)
		{
			std::cout << "�����ļ��������ڣ�" << std::endl;
		}
		nSize++;
	}


	
}

void DHVendor::SetDownloadPath(const std::string& Root)
{
	m_sRoot = Root;
}

void DHVendor::throwException()
{

}

void CALLBACK DH_BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser)
{
	
}

void CALLBACK DH_BTDownLoadFile(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	DOWNLOADFILEINFO DownloadFileInfo;
	DownloadFileInfo.nID = dwUser;
	DownloadFileInfo.dwDownLoadSize = dwDownLoadSize;
	DownloadFileInfo.dwTotalSize = dwTotalSize;

	DownloadFileNotificationQueue::GetInstance().GetQueue().enqueueNotification(new SendDataNotification(DownloadFileInfo));
}

void CALLBACK DH_PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	
}

int CALLBACK DH_PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser)
{
	if (dwUser == 0)
	{
		return -1;
	}

	return 1;
}


void DH_CreatePath(const size_t channel)
{
	std::string strPath = "D:\\DownLoadVideo\\";

	BOOL bOne = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bOne)
	{
		std::cout << "Error or exit!" << std::endl;
		return;
	}

	strPath.append("��\\");
	BOOL bTwo = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bTwo)
	{
		std::cout << "Error or exit!!" << std::endl;
		return;
	}
	char szChannel[10];
	ZeroMemory(szChannel, 10);
	sprintf_s(szChannel, "ͨ��%d", channel);
	strPath.append(szChannel);
	strPath.append("\\");

	BOOL bThree = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bThree)
	{
		std::cout << "Error!" << std::endl;
		return;
	}
}

std::string DH_MakeFileName(int channel, const std::string& startTime, const std::string& endTime)
{
	std::string strFileName;

	strFileName += "channel";
	if (channel < 10)
	{
		strFileName += "0";
	}
	strFileName += std::to_string(channel);
	strFileName += "-";
	strFileName += startTime.data();
	strFileName += "-";
	strFileName += endTime.data();
	strFileName.append(".dav");

	return strFileName;
}

void DH_trTOnt(NET_TIME &ntStartTime, NET_TIME &ntEndTime, const time_range range)
{
	tm tmStartTime;
	tm tmEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);

	timeStdToDH(&tmStartTime, &ntStartTime);
	timeStdToDH(&tmEndTime, &ntEndTime);
}

std::string DH_MakeStrTimeByTimestamp(time_t time)
{
	char cTime[50];
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime(cTime, 50, "%Y%m%d%H%M%S", &ttime);

	string strTime(cTime);

	return strTime;
}
std::string DH_MakeStrByInteger(int data)
{
	char cData[50];

	sprintf_s(cData, 50, "%d", data);

	string strTime(cData);

	return strTime;
}

std::string DH_GetLastErrorString()
{
	DWORD dwError;
	dwError = CLIENT_GetLastError();

	switch (dwError)
	{
	case NET_NOERROR: return "û�д���";
	case NET_ERROR: return "δ֪����";
	case NET_SYSTEM_ERROR: return "Windowsϵͳ����";
	case NET_NETWORK_ERROR: return "������󣬿�������Ϊ���糬ʱ";
	case NET_DEV_VER_NOMATCH: return "�豸Э�鲻ƥ��";
	case NET_INVALID_HANDLE: return "�����Ч";
	case NET_OPEN_CHANNEL_ERROR: return "��ͨ��ʧ��";
	case NET_CLOSE_CHANNEL_ERROR: return "�ر�ͨ��ʧ��";
	case NET_ILLEGAL_PARAM: return "�û��������Ϸ�";
	case NET_SDK_INIT_ERROR: return "SDK��ʼ������";
	case NET_SDK_UNINIT_ERROR: return "SDK�������";
	case NET_RENDER_OPEN_ERROR: return "����render��Դ����";
	case NET_DEC_OPEN_ERROR: return "�򿪽�������";
	case NET_DEC_CLOSE_ERROR: return "�رս�������";
	case NET_MULTIPLAY_NOCHANNEL: return "�໭��Ԥ���м�⵽ͨ����Ϊ0";
	case NET_TALK_INIT_ERROR: return "¼�����ʼ��ʧ��";
	case NET_TALK_NOT_INIT: return "¼����δ����ʼ��";
	case NET_TALK_SENDDATA_ERROR: return "������Ƶ���ݳ���";
	case NET_REAL_ALREADY_SAVING: return "ʵʱ�����Ѿ����ڱ���״̬";
	case NET_NOT_SAVING: return "δ����ʵʱ����";
	case NET_OPEN_FILE_ERROR: return "���ļ�����";
	case NET_PTZ_SET_TIMER_ERROR: return "������̨���ƶ�ʱ��ʧ��";
	case NET_RETURN_DATA_ERROR: return "�Է������ݵ�У�����";
	case NET_INSUFFICIENT_BUFFER: return "û���㹻�Ļ���";
	case NET_NOT_SUPPORTED: return "��ǰSDKδ֧�ָù���";
	case NET_NO_RECORD_FOUND: return "��ѯ����¼��";
	case NET_NOT_AUTHORIZED: return "�޲���Ȩ��";
	case NET_NOT_NOW: return "��ʱ�޷�ִ��";
	case NET_NO_TALK_CHANNEL: return "δ���ֶԽ�ͨ��";
	case NET_NO_AUDIO: return "δ������Ƶ";
	case NET_NO_INIT: return "����SDKδ����ʼ��";
	case NET_DOWNLOAD_END: return "�����ѽ���";
	case NET_EMPTY_LIST: return "��ѯ���Ϊ��";
	case NET_ERROR_GETCFG_SYSATTR: return "��ȡϵͳ��������ʧ��";
	case NET_ERROR_GETCFG_SERIAL: return "��ȡ���к�ʧ��";
	case NET_ERROR_GETCFG_GENERAL: return "��ȡ��������ʧ��";
	case NET_ERROR_GETCFG_DSPCAP: return "��ȡDSP��������ʧ��";
	case NET_ERROR_GETCFG_NETCFG: return "��ȡ��������ʧ��";
	case NET_ERROR_GETCFG_CHANNAME: return "��ȡͨ������ʧ��";
	case NET_ERROR_GETCFG_VIDEO: return "��ȡ��Ƶ����ʧ��";
	case NET_ERROR_GETCFG_RECORD: return "��ȡ¼������ʧ��";
	case NET_ERROR_GETCFG_PRONAME: return "��ȡ������Э������ʧ��";
	case NET_ERROR_GETCFG_FUNCNAME: return "��ȡ232���ڹ�������ʧ��";
	case NET_ERROR_GETCFG_485DECODER: return "��ȡ����������ʧ��";
	case NET_ERROR_GETCFG_232COM: return "��ȡ232��������ʧ��";
	case NET_ERROR_GETCFG_ALARMIN: return "��ȡ�ⲿ������������ʧ��";
	case NET_ERROR_GETCFG_ALARMDET: return "��ȡ��̬��ⱨ��ʧ��";
	case NET_ERROR_GETCFG_SYSTIME: return "��ȡ�豸ʱ��ʧ��";
	case NET_ERROR_GETCFG_PREVIEW: return "��ȡԤ������ʧ��";
	case NET_ERROR_GETCFG_AUTOMT: return "��ȡ�Զ�ά������ʧ��";
	case NET_ERROR_GETCFG_VIDEOMTRX: return "��ȡ��Ƶ��������ʧ��";
	case NET_ERROR_GETCFG_COVER: return "��ȡ�����ڵ�����ʧ��";
	case NET_ERROR_GETCFG_WATERMAKE: return "��ȡͼ��ˮӡ����ʧ��";
	case NET_ERROR_SETCFG_GENERAL: return "�޸ĳ�������ʧ��";
	case NET_ERROR_SETCFG_NETCFG: return "�޸���������ʧ��";
	case NET_ERROR_SETCFG_CHANNAME: return "�޸�ͨ������ʧ��";
	case NET_ERROR_SETCFG_VIDEO: return "�޸���Ƶ����ʧ��";
	case NET_ERROR_SETCFG_RECORD: return "�޸�¼������ʧ��";
	case NET_ERROR_SETCFG_485DECODER: return "�޸Ľ���������ʧ��";
	case NET_ERROR_SETCFG_232COM: return "�޸�232��������ʧ��";
	case NET_ERROR_SETCFG_ALARMIN: return "�޸��ⲿ���뱨������ʧ��";
	case NET_ERROR_SETCFG_ALARMDET: return "�޸Ķ�̬��ⱨ������ʧ��";
	case NET_ERROR_SETCFG_SYSTIME: return "�޸��豸ʱ��ʧ��";
	case NET_ERROR_SETCFG_PREVIEW: return "�޸�Ԥ������ʧ��";
	case NET_ERROR_SETCFG_AUTOMT: return "�޸��Զ�ά������ʧ��";
	case NET_ERROR_SETCFG_VIDEOMTRX: return "�޸���Ƶ��������ʧ��";
	case NET_ERROR_SETCFG_COVER: return "�޸������ڵ�����ʧ��";
	case NET_ERROR_SETCFG_WATERMAKE: return "�޸�ͼ��ˮӡ����ʧ��";
	case NET_ERROR_SETCFG_WLAN: return "�޸�����������Ϣʧ��";
	case NET_ERROR_SETCFG_WLANDEV: return "ѡ�����������豸ʧ��";
	case NET_ERROR_SETCFG_REGISTER: return "�޸�����ע���������ʧ��";
	case NET_ERROR_SETCFG_CAMERA: return "�޸�����ͷ��������ʧ��";
	case NET_ERROR_SETCFG_INFRARED: return "�޸ĺ��ⱨ������ʧ��";
	case NET_ERROR_SETCFG_SOUNDALARM: return "�޸���Ƶ��������ʧ��";
	case NET_ERROR_SETCFG_STORAGE: return "�޸Ĵ洢λ������ʧ��";
	case NET_AUDIOENCODE_NOTINIT: return "��Ƶ����ӿ�û�гɹ���ʼ��";
	case NET_DATA_TOOLONGH: return "���ݹ���";
	case NET_UNSUPPORTED: return "�豸��֧�ָò���";
	case NET_DEVICE_BUSY: return "�豸��Դ����";
	case NET_SERVER_STARTED: return "�������Ѿ�����";
	case NET_SERVER_STOPPED: return "��������δ�ɹ�����";
	case NET_LISTER_INCORRECT_SERIAL: return "�������к�����";
	case NET_QUERY_DISKINFO_FAILED: return "��ȡӲ����Ϣʧ��";
	case NET_ERROR_GETCFG_SESSION: return "��ȡ����Session��Ϣ";
	case NET_USER_FLASEPWD_TRYTIME: return "����������󳬹����ƴ���";
	case NET_LOGIN_ERROR_PASSWORD: return "���벻��ȷ";
	case NET_LOGIN_ERROR_USER: return "�ʻ�������";
	case NET_LOGIN_ERROR_TIMEOUT: return "�ȴ���¼���س�ʱ";
	case NET_LOGIN_ERROR_RELOGGIN: return "�ʺ��ѵ�¼";
	case NET_LOGIN_ERROR_LOCKED: return "�ʺ��ѱ�����";
	case NET_LOGIN_ERROR_BLACKLIST: return "�ʺ��ѱ���Ϊ������";
	case NET_LOGIN_ERROR_BUSY: return "��Դ���㣬ϵͳæ";
	case NET_LOGIN_ERROR_CONNECT: return "��¼�豸��ʱ���������粢����";
	case NET_LOGIN_ERROR_NETWORK: return "��������ʧ��";
	case NET_LOGIN_ERROR_SUBCONNECT: return "��¼�豸�ɹ������޷�������Ƶͨ������������״��";
	case NET_LOGIN_ERROR_MAXCONNECT: return "�������������";
	case NET_LOGIN_ERROR_PROTOCOL3_ONLY: return "ֻ֧��3��Э��";
	case NET_LOGIN_ERROR_UKEY_LOST: return "δ����U�ܻ�U����Ϣ����";
	case NET_LOGIN_ERROR_NO_AUTHORIZED: return "�ͻ���IP��ַû�е�¼Ȩ��";
	case NET_RENDER_SOUND_ON_ERROR: return "Render�����Ƶ����";
	case NET_RENDER_SOUND_OFF_ERROR: return "Render��ر���Ƶ����";
	case NET_RENDER_SET_VOLUME_ERROR: return "Render�������������";
	case NET_RENDER_ADJUST_ERROR: return "Render�����û����������";
	case NET_RENDER_PAUSE_ERROR: return "Render����ͣ���ų���";
	case NET_RENDER_SNAP_ERROR: return "Render��ץͼ����";
	case NET_RENDER_STEP_ERROR: return "Render�ⲽ������";
	case NET_RENDER_FRAMERATE_ERROR: return "Render������֡�ʳ���";
	case NET_RENDER_DISPLAYREGION_ERROR: return "Render��������ʾ�������";
	case NET_GROUP_EXIST: return "�����Ѵ���";
	case NET_GROUP_NOEXIST: return "����������";
	case NET_GROUP_RIGHTOVER: return "���Ȩ�޳���Ȩ���б�Χ";
	case NET_GROUP_HAVEUSER: return "�������û�������ɾ��";
	case NET_GROUP_RIGHTUSE: return "���ĳ��Ȩ�ޱ��û�ʹ�ã����ܳ���";
	case NET_GROUP_SAMENAME: return "������ͬ���������ظ�";
	case NET_USER_EXIST: return "�û��Ѵ���";
	case NET_USER_NOEXIST: return "�û�������";
	case NET_USER_RIGHTOVER: return "�û�Ȩ�޳�����Ȩ��";
	case NET_USER_PWD: return "�����ʺţ��������޸�����";
	case NET_USER_FLASEPWD: return "���벻��ȷ";
	case NET_USER_NOMATCHING: return "���벻ƥ��";
	case NET_USER_INUSE: return "�˺�����ʹ����";
	case NET_ERROR_GETCFG_ETHERNET: return "��ȡ��������ʧ��";
	case NET_ERROR_GETCFG_WLAN: return "��ȡ����������Ϣʧ��";
	case NET_ERROR_GETCFG_WLANDEV: return "��ȡ���������豸ʧ��";
	case NET_ERROR_GETCFG_REGISTER: return "��ȡ����ע�����ʧ��";
	case NET_ERROR_GETCFG_CAMERA: return "��ȡ����ͷ����ʧ��";
	case NET_ERROR_GETCFG_INFRARED: return "��ȡ���ⱨ������ʧ��";
	case NET_ERROR_GETCFG_SOUNDALARM: return "��ȡ��Ƶ��������ʧ��";
	case NET_ERROR_GETCFG_STORAGE: return "��ȡ�洢λ������ʧ��";
	case NET_ERROR_GETCFG_MAIL: return "��ȡ�ʼ�����ʧ��";
	case NET_CONFIG_DEVBUSY: return "��ʱ�޷�����";
	case NET_CONFIG_DATAILLEGAL: return "�������ݲ��Ϸ�";
	case NET_ERROR_GETCFG_DST: return "��ȡ����ʱ����ʧ��";
	case NET_ERROR_SETCFG_DST: return "��������ʱ����ʧ��";
	case NET_ERROR_GETCFG_VIDEO_OSD: return "��ȡ��ƵOSD��������ʧ��";
	case NET_ERROR_SETCFG_VIDEO_OSD: return "������ƵOSD��������ʧ��";
	case NET_ERROR_GETCFG_GPRSCDMA: return "��ȡCDMA��GPRS��������ʧ��";
	case NET_ERROR_SETCFG_GPRSCDMA: return "����CDMA��GPRS��������ʧ��";
	case NET_ERROR_GETCFG_IPFILTER: return "��ȡIP��������ʧ��";
	case NET_ERROR_SETCFG_IPFILTER: return "����IP��������ʧ��";
	case NET_ERROR_GETCFG_TALKENCODE: return "��ȡ�����Խ���������ʧ��";
	case NET_ERROR_SETCFG_TALKENCODE: return "���������Խ���������ʧ��";
	case NET_ERROR_GETCFG_RECORDLEN: return "��ȡ¼������������ʧ��";
	case NET_ERROR_SETCFG_RECORDLEN: return "����¼������������ʧ��";
	case NET_DONT_SUPPORT_SUBAREA: return "��֧������Ӳ�̷���";
	case NET_ERROR_GET_AUTOREGSERVER: return "��ȡ�豸������ע���������Ϣʧ��";
	case NET_ERROR_CONTROL_AUTOREGISTER: return "����ע���ض���ע�����";
	case NET_ERROR_DISCONNECT_AUTOREGISTER: return "�Ͽ�����ע�����������";
	case NET_ERROR_GETCFG_MMS: return "��ȡmms����ʧ��";
	case NET_ERROR_SETCFG_MMS: return "����mms����ʧ��";
	case NET_ERROR_GETCFG_SMSACTIVATION: return "��ȡ���ż���������������ʧ��";
	case NET_ERROR_SETCFG_SMSACTIVATION: return "���ö��ż���������������ʧ��";
	case NET_ERROR_GETCFG_DIALINACTIVATION: return "��ȡ���ż���������������ʧ��";
	case NET_ERROR_SETCFG_DIALINACTIVATION: return "���ò��ż���������������ʧ��";
	case NET_ERROR_GETCFG_VIDEOOUT: return "��ѯ��Ƶ�����������ʧ��";
	case NET_ERROR_SETCFG_VIDEOOUT: return "������Ƶ�����������ʧ��";
	case NET_ERROR_GETCFG_OSDENABLE: return "��ȡosd����ʹ������ʧ��";
	case NET_ERROR_SETCFG_OSDENABLE: return "����osd����ʹ������ʧ��";
	case NET_ERROR_SETCFG_ENCODERINFO: return "��������ͨ��ǰ�˱����������ʧ��";
	case NET_ERROR_GETCFG_TVADJUST: return "��ȡTV��������ʧ��";
	case NET_ERROR_SETCFG_TVADJUST: return "����TV��������ʧ��";
	case NET_ERROR_CONNECT_FAILED: return "����������ʧ��";
	case NET_ERROR_SETCFG_BURNFILE: return "�����¼�ļ��ϴ�ʧ��";
	case NET_ERROR_SNIFFER_GETCFG: return "��ȡץ��������Ϣʧ��";
	case NET_ERROR_SNIFFER_SETCFG: return "����ץ��������Ϣʧ��";
	case NET_ERROR_DOWNLOADRATE_GETCFG: return "��ѯ����������Ϣʧ��";
	case NET_ERROR_DOWNLOADRATE_SETCFG: return "��������������Ϣʧ��";
	case NET_ERROR_SEARCH_TRANSCOM: return "��ѯ���ڲ���ʧ��";
	case NET_ERROR_GETCFG_POINT: return "��ȡԤ�Ƶ���Ϣ����";
	case NET_ERROR_SETCFG_POINT: return "����Ԥ�Ƶ���Ϣ����";
	case NET_SDK_LOGOUT_ERROR: return "SDKû�������ǳ��豸";
	case NET_ERROR_GET_VEHICLE_CFG: return "��ȡ��������ʧ��";
	case NET_ERROR_SET_VEHICLE_CFG: return "���ó�������ʧ��";
	case NET_ERROR_GET_ATM_OVERLAY_CFG: return "��ȡatm��������ʧ��";
	case NET_ERROR_SET_ATM_OVERLAY_CFG: return "����atm��������ʧ��";
	case NET_ERROR_GET_ATM_OVERLAY_ABILITY: return "��ȡatm��������ʧ��";
	case NET_ERROR_GET_DECODER_TOUR_CFG: return "��ȡ������������Ѳ����ʧ��";
	case NET_ERROR_SET_DECODER_TOUR_CFG: return "���ý�����������Ѳ����ʧ��";
	case NET_ERROR_CTRL_DECODER_TOUR: return "���ƽ�����������Ѳʧ��";
	case NET_GROUP_OVERSUPPORTNUM: return "�����豸֧������û�����Ŀ";
	case NET_USER_OVERSUPPORTNUM: return "�����豸֧������û���Ŀ";
	case NET_ERROR_GET_SIP_CFG: return "��ȡSIP����ʧ��";
	case NET_ERROR_SET_SIP_CFG: return "����SIP����ʧ��";
	case NET_ERROR_GET_SIP_ABILITY: return "��ȡSIP����ʧ��";
	case NET_ERROR_GET_WIFI_AP_CFG: return "��ȡWIFI ap����ʧ��";
	case NET_ERROR_SET_WIFI_AP_CFG: return "����WIFI ap����ʧ��";
	case NET_ERROR_GET_DECODE_POLICY: return "��ȡ�����������ʧ��";
	case NET_ERROR_SET_DECODE_POLICY: return "���ý����������ʧ��";
	case NET_ERROR_TALK_REJECT: return "�ܾ��Խ�";
	case NET_ERROR_TALK_OPENED: return "�Խ��������ͻ��˴�";
	case NET_ERROR_TALK_RESOURCE_CONFLICIT: return "��Դ��ͻ";
	case NET_ERROR_TALK_UNSUPPORTED_ENCODE: return "��֧�ֵ����������ʽ";
	case NET_ERROR_TALK_RIGHTLESS: return "��Ȩ��";
	case NET_ERROR_TALK_FAILED: return "����Խ�ʧ��";
	case NET_ERROR_GET_MACHINE_CFG: return "��ȡ�����������ʧ��";
	case NET_ERROR_SET_MACHINE_CFG: return "���û����������ʧ��";
	case NET_ERROR_GET_DATA_FAILED: return "�豸�޷���ȡ��ǰ��������";
	case NET_ERROR_MAC_VALIDATE_FAILED: return "MAC��ַ��֤ʧ��";
	case NET_ERROR_GET_INSTANCE: return "��ȡ������ʵ��ʧ��";
	case NET_ERROR_JSON_REQUEST: return "���ɵ�jason�ַ�������";
	case NET_ERROR_JSON_RESPONSE: return "��Ӧ��jason�ַ�������";
	case NET_ERROR_VERSION_HIGHER: return "Э��汾���ڵ�ǰʹ�õİ汾";
	case NET_SPARE_NO_CAPACITY: return "�ȱ�����ʧ��, ��������";
	case NET_ERROR_SOURCE_IN_USE: return "��ʾԴ���������ռ��";
	case NET_ERROR_REAVE: return "�߼��û���ռ�ͼ��û���Դ";
	case NET_ERROR_NETFORBID: return "��ֹ����";
	case NET_ERROR_GETCFG_MACFILTER: return "��ȡMAC��������ʧ��";
	case NET_ERROR_SETCFG_MACFILTER: return "����MAC��������ʧ��";
	case NET_ERROR_GETCFG_IPMACFILTER: return "��ȡIP/MAC��������ʧ��";
	case NET_ERROR_SETCFG_IPMACFILTER: return "����IP/MAC��������ʧ��";
	case NET_ERROR_OPERATION_OVERTIME: return "��ǰ������ʱ";
	case NET_ERROR_SENIOR_VALIDATE_FAILED: return "�߼�У��ʧ��";
	case NET_ERROR_DEVICE_ID_NOT_EXIST: return "�豸ID������";
	case NET_ERROR_UNSUPPORTED: return "��֧�ֵ�ǰ����";
	case NET_ERROR_PROXY_DLLLOAD: return "��������ʧ��";
	case NET_ERROR_PROXY_ILLEGAL_PARAM: return "�����û��������Ϸ�";
	case NET_ERROR_PROXY_INVALID_HANDLE: return "��������Ч";
	case NET_ERROR_PROXY_LOGIN_DEVICE_ERROR: return "�������ǰ���豸ʧ��";
	case NET_ERROR_PROXY_START_SERVER_ERROR: return "�����������ʧ��";
	case NET_ERROR_SPEAK_FAILED: return "���󺰻�ʧ��";
	case NET_ERROR_NOT_SUPPORT_F6: return "�豸��֧�ִ�F6�ӿڵ���";
	default: return "δ֪����";
	}
}

// #include "catch.hpp"
// 
// TEST_CASE_METHOD(DHVendor, "Init DH SDK", "[DHVendor]")
//  {
// 
// 	time_range range;
// 	range.start = 1467302400;
// 	//range.end = 1466524800;
// 	range.end = 1467648000;
// 	//range.end = 1478833871;
// 	REQUIRE_NOTHROW(Init());
// 	REQUIRE_NOTHROW(StartSearchDevice());
// 	
// 	REQUIRE_NOTHROW(Login("192.168.0.96", 37777, "admin", ""));
// 
// 	REQUIRE_NOTHROW(Search(0, 0, range));
// 	//REQUIRE_NOTHROW(Search(1, range));
// 
// 	//REQUIRE_NOTHROW(Download(0, 0, range));
// 	REQUIRE_NOTHROW(Download(0, 0, "channel0-20160701000000-20160701235959-0"));
// 	//REQUIRE_NOTHROW(Download(0, 0, "channel0-20160701000000-20160701235959-1"));
// 
// 	//REQUIRE_NOTHROW(PlayVideo(0, range));
// 	//REQUIRE_NOTHROW(PlayVideo(0, "channel0-20160621000000-20160621235959-0"));
// 	//REQUIRE_NOTHROW(Logout());
// 
// }
