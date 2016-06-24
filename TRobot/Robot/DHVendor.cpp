#include "stdafx.h"
#include "DHVendor.h"

#include <atltime.h>
#include "TestWindows.h"

DHVendor::DHVendor():
m_strName("��"),
m_dwPort(0),
m_strUser(""),
m_strPasswords(""),
m_strIP(""),
m_lLoginHandle(0),
m_hMod(NULL)
{
	memset(&m_deviceInfo, 0, sizeof(m_deviceInfo));

	m_stime.dwYear = 2016;
	m_stime.dwMonth = 6;
	m_stime.dwDay = 21;
	m_stime.dwHour = 0;
	m_stime.dwMinute = 0;
	m_stime.dwSecond = 0;

	m_etime.dwYear = 2016;
	m_etime.dwMonth = 6;
	m_etime.dwDay = 22;
	m_etime.dwHour = 23;
	m_etime.dwMinute = 0;
	m_etime.dwSecond = 0;

	m_hMod = LoadLibraryA("C:\\Users\\TL\\Desktop\\Robot1\\TRobot\\Robot\\DHsdk\\dhnetsdk.dll");
	if (NULL == m_hMod)
	{
		return;
	}

	m_pGetLastError = (pCLIENT_GetLastError)GetProcAddress(m_hMod, "CLIENT_GetLastError");
	assert(m_pGetLastError);
	m_pGetChannel = (pCLIENT_QueryChannelName)GetProcAddress(m_hMod, "CLIENT_QueryChannelName");
	assert(m_pGetChannel);
	m_pInit = (pCLIENT_Init)GetProcAddress(m_hMod, "CLIENT_Init");
	assert(m_pInit);
	m_pUninit = (pCLIENT_Cleanup)GetProcAddress(m_hMod, "CLIENT_Cleanup");
	assert(m_pUninit);
	m_pLogin = (pCLIENT_Login)GetProcAddress(m_hMod, "CLIENT_Login");
	assert(m_pLogin);
	m_pLogout = (pCLIENT_Logout)GetProcAddress(m_hMod, "CLIENT_Logout");
	assert(m_pLogout);
	m_pSearchFileByTime = (pCLIENT_QueryRecordFile)GetProcAddress(m_hMod, "CLIENT_QueryRecordFile");
	assert(m_pSearchFileByTime);
	m_pDownloadByFile = (pCLIENT_DownloadByRecordFile)GetProcAddress(m_hMod, "CLIENT_DownloadByRecordFile");
	assert(m_pDownloadByFile);
	m_pDownloadByTime = (pCLIENT_DownloadByTime)GetProcAddress(m_hMod, "CLIENT_DownloadByTime");
	assert(m_pDownloadByTime);
	m_pGetDownloadPos = (pCLIENT_GetDownloadPos)GetProcAddress(m_hMod, "CLIENT_GetDownloadPos");
	assert(m_pGetDownloadPos);
	m_pStopDownload = (pCLIENT_StopDownload)GetProcAddress(m_hMod, "CLIENT_StopDownload");
	assert(m_pStopDownload);
	m_pPlayBackByFile = (pCLIENT_PlayBackByRecordFile)GetProcAddress(m_hMod, "CLIENT_PlayBackByRecordFile");
	assert(m_pPlayBackByFile);
	m_pPlayBackByTimeEx = (pCLIENT_PlayBackByTimeEx)GetProcAddress(m_hMod, "CLIENT_PlayBackByTimeEx");
	m_pSeekPlayBack = (pCLIENT_SeekPlayBack)GetProcAddress(m_hMod, "CLIENT_SeekPlayBack");
	assert(m_pSeekPlayBack);
	m_pPausePlayBack = (pCLIENT_PausePlayBack)GetProcAddress(m_hMod, "CLIENT_PausePlayBack");
	assert(m_pPausePlayBack);
	m_pStopPlayBack = (pCLIENT_StopPlayBack)GetProcAddress(m_hMod, "CLIENT_StopPlayBack");
	assert(m_pStopPlayBack);
}


DHVendor::~DHVendor()
{
	if (m_hMod)
	{
		//FreeLibrary(m_hMod);
		m_hMod = NULL;
	}
}

void DHVendor::Init(const std::string& ip, size_t port)
{
	BOOL bInit = m_pInit(0, 0);

	cout << "bInit:" << bInit << endl;
	
	m_strIP = ip;
	m_dwPort = port;
	cout << "m_dwPort:" << m_dwPort << endl;
	cout << "m_strIP:" << m_strIP << endl;
	if (!bInit)
	{
		cout << "��ʼ������" << m_pGetLastError() << endl;
	}
	else
	{
		cout << "��ʼ��SDK�ɹ���" << endl;
	}
}

void DHVendor::Login(const std::string& user, const std::string& password)
{
	int nError = 0;
	m_strUser = user;
	m_strPasswords = password;
	m_lLoginHandle = m_pLogin((char *)(m_strIP.c_str()), m_dwPort, (char *)(m_strUser.c_str()), (char *)(m_strPasswords.c_str()), &m_deviceInfo, &nError);

	if (0 != nError)
	{
		cout << "��¼����(nError)��" << GetLastErrorString().c_str() << endl;
	}

	if (0 == m_lLoginHandle)
	{
		cout << "��¼����(lLogin)��" << GetLastErrorString().c_str() << endl;
	}

	char szChannelNames[16 * 32];
	ZeroMemory(szChannelNames, sizeof(szChannelNames));
	int nChannelCount = 0;
	BOOL bRet = m_pGetChannel(m_lLoginHandle, szChannelNames, 16 * 32, &nChannelCount, 500);
	if (!bRet)
	{
		cout << "��¼����(lLogin)��" << GetLastErrorString().c_str() << endl;
		return;
	}
	m_channels.clear();

	char szName[32 + 2];
	for (int i = 1; i <= nChannelCount; i++)
	{
		ZeroMemory(szName, sizeof(szName));
		cout << szName << "ͨ��" << i << endl;
		string strName = szName;
		m_channels.insert(std::make_pair(i, strName));
		m_channelVec.push_back(i);
	}
}

void DHVendor::Logout()
{
	if (m_lLoginHandle > 0 && !m_pLogout(m_lLoginHandle))
	{
		cout << "�˳�����" << GetLastErrorString().c_str() << endl;
		return;
	}
}

void DHVendor::SearchAll()
{

}

#define MAX_SEARCH_COUNT 1000
void DHVendor::SearchByTime(const std::time_t& start, const std::time_t& end)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "���ȵ�¼!" << endl;
		return;
	}

	if (start >= end)
	{
		cout << "ʱ�䷶Χ����!" << endl;
		return;
	}

	m_files.clear();

	auto itr = m_channelVec.begin();
	for (; itr != m_channelVec.end(); itr++)
	{
		int nChannelId = *itr;

		tm STime;
		tm ETime;

		_localtime64_s(&STime, (const time_t*)&start);
		_localtime64_s(&ETime, (const time_t*)&end);

		//timeStdToDH(&STime, &stime);
		//timeStdToDH(&ETime, &etime);
		
		timeDHToStd(&m_stime, &STime);
		timeDHToStd(&m_etime, &ETime);

		char szTime[512];
		ZeroMemory(szTime, 512);

		cout << "GetRecordFileList " << szTime << endl;

		NET_RECORDFILE_INFO ifileinfo[MAX_SEARCH_COUNT];
		ZeroMemory(ifileinfo, sizeof(ifileinfo));
		int iMaxNum = 0;
		BOOL bRet = m_pSearchFileByTime(m_lLoginHandle, nChannelId - 1, 0, &m_stime, &m_etime, 0, ifileinfo, sizeof(ifileinfo), &iMaxNum, 5000, true);

		cout << "bRet:" << bRet << endl;
		if (!bRet)
		{
			cout << "GetRecordFileList ��ѯ¼��ʧ�ܣ�����ԭ��" << GetLastErrorString() << endl;
		}
		if (iMaxNum <= 0)
		{
			cout << "GetRecordFileList ��ѯ¼��ɹ���¼�����Ϊ0" << endl;
		}

		NET_RECORDFILE_INFO item;
		RecordFile info;
		tm sTm;
		tm eTm;
		for (int i = 0; i < iMaxNum; i++)
		{
			item = ifileinfo[i];

			timeDHToStd(&item.starttime, &sTm);
			timeDHToStd(&item.endtime, &eTm);

			info.channel = item.ch + 1;
			info.size = item.size * 1024;
			info.name = item.filename;
			info.beginTime = mktime(&sTm);
			info.endTime = mktime(&eTm);
			info.setPrivateData(&ifileinfo[i], sizeof(NET_RECORDFILE_INFO));
			m_files.push_back(info);

			cout <<  "GetRecordFileList �ļ���:" << item.filename << "  " << "�ļ���С:" << item.size << "  " << "ͨ��:" << item.ch << endl;
		}
	}
}

void DHVendor::DownloadByTime()
{
	bool bFlag = false;

	if (0 >= m_lLoginHandle)
	{
		cout << "���ȵ�¼!" << endl;
		return;
	}
	//char *strName = "download.dav";
	char *strName = "D:\\Projects\\DownLoad\\download.dav";
	auto itr = m_channelVec.begin();

	for (; itr != m_channelVec.end(); itr++)
	{
		int nChannelId = *itr;

		m_pdownloadfile = m_pDownloadByTime(m_lLoginHandle, 0, 0, &m_stime, &m_etime, strName, BTDownLoadPos, (DWORD)this);

		if (0 == m_pdownloadfile)
		{
			cout << "downLoadByRecordFile ����¼��ʧ�ܣ�����ԭ��" << GetLastErrorString() << endl;
			return;
		}
	}

	//if ()
}

void DHVendor::DownloadByTime(const std::time_t& start, const std::time_t& end)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "���ȵ�¼!" << endl;
		return;
	}

	tm STime;
	tm ETime;

	_localtime64_s(&STime, (const time_t*)&start);
	_localtime64_s(&ETime, (const time_t*)&end);

	timeDHToStd(&m_stime, &STime);
	timeDHToStd(&m_etime, &ETime);


	char *strName = "D:\\Projects\\DownLoadVideo\\download.dav";
	auto itr = m_channelVec.begin();
	for (; itr != m_channelVec.end(); itr++)
	{
		int nChannelId = *itr;

		m_pdownloadfile = m_pDownloadByTime(m_lLoginHandle, nChannelId - 1, 0, &m_stime, &m_etime, strName, BTDownLoadPos, (DWORD)this);
		cout << "strName:" << strName << endl;
		if (0 == m_pdownloadfile)
		{
			cout << "downLoadByRecordFile ����¼��ʧ�ܣ�����ԭ��" << GetLastErrorString() << endl;
			return ;
		}
	}
}	

void DHVendor::DownloadByName(const std::string& filename)
{

}

void DHVendor::PlayVideo(const std::string& filename)
{
	
}

void DHVendor::PlayVideoByTime(const std::time_t& start, const std::time_t& end)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "PlayVideo ���߲���ʧ��ԭ��" << GetLastErrorString() << endl;
		return;
	}

	tm STime;
	tm ETime;

	_localtime64_s(&STime, (const time_t*)&start);
	_localtime64_s(&ETime, (const time_t*)&end);

	timeDHToStd(&m_stime, &STime);
	timeDHToStd(&m_etime, &ETime);

	auto itr = m_channelVec.begin();
	for (; itr != m_channelVec.end(); itr++)
	{
		int nChannelId = *itr;
		BOOL lPlayID = m_pPlayBackByTimeEx(m_lLoginHandle, nChannelId, &m_stime, &m_etime, g_hWnd, PlayCallBack, (DWORD)this, PBDataCallBack, (DWORD)this);

		if (!lPlayID)
		{
			cout << "����ʧ��ԭ��" << GetLastErrorString() << endl;
		}
	}
	
}

void DHVendor::timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd)
{
	pTimeStd->tm_year = pTimeDH->dwYear - 1900;
	pTimeStd->tm_mon = pTimeDH->dwMonth - 1;
	pTimeStd->tm_mday = pTimeDH->dwDay;
	pTimeStd->tm_hour = pTimeDH->dwHour;
	pTimeStd->tm_min = pTimeDH->dwMinute;
	pTimeStd->tm_sec = pTimeDH->dwSecond;
}

void DHVendor::timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH)
{
	pTimeDH->dwYear = pTimeStd->tm_year + 1900;
	pTimeDH->dwMonth = pTimeStd->tm_mon + 1;
	pTimeDH->dwDay = pTimeStd->tm_mday;
	pTimeDH->dwHour = pTimeStd->tm_hour;
	pTimeDH->dwMinute = pTimeStd->tm_min;
	pTimeDH->dwSecond = pTimeStd->tm_sec;
}


string DHVendor::GetLastErrorString()
{
	DWORD dwError;
	dwError = m_pGetLastError();

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

#include "catch.hpp"

DHVendor Obj;

TEST_CASE_METHOD(DHVendor, "Init DH SDK", "[Init]")
{
	REQUIRE_NOTHROW(Obj.Init("192.168.0.55", 37777));
	REQUIRE(m_hMod != nullptr);
}

TEST_CASE_METHOD(DHVendor, "Login  DH Device", "[Login]")
{
	//REQUIRE_NOTHROW(Init("192.168.0.55", 37777));
	REQUIRE_NOTHROW(Obj.Login("admin", "112233"));
}

TEST_CASE_METHOD(DHVendor, "Logout DH Device", "[Logout]")
{
	//REQUIRE_THROWS(Logout());
}

TEST_CASE_METHOD(DHVendor, "Search all videos from DH device", "[SearchByTime]")
{
	REQUIRE_THROWS(Obj.SearchByTime(1466438400, 1466524800));
}

TEST_CASE_METHOD(DHVendor, " Download by time from DH device", "[DownloadByTime]")
{
	REQUIRE_THROWS(Obj.DownloadByTime(1466438400, 1466524800));
}

TEST_CASE_METHOD(DHVendor, " PlayVideo  from  filename", "[PlayVideo]")
{
	
	TestWindows Test;
	Test.Init();
	REQUIRE_THROWS(Obj.PlayVideoByTime(1466438400, 1466524800));

}