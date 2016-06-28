#include "stdafx.h"
#include "DHVendor.h"

#include <ctime>
#include <fstream>
#include "TestWindows.h"

DHVendor::DHVendor():
m_strName("��"),
m_dwPort(0),
m_strUser(""),
m_strPasswords(""),
m_strIP(""),
m_lLoginHandle(0),
m_hMod(NULL),
m_strPath(""),
m_DHChannels(0)
{
	memset(&m_deviceInfo, 0, sizeof(m_deviceInfo));
}


DHVendor::~DHVendor()
{
	if (!m_files.empty())
	{
		m_files.clear();
	}
}

void DHVendor::Init(const std::string& ip, size_t port)
{
	BOOL bInit = CLIENT_Init(0, 0);
	
	m_strIP = ip;
	m_dwPort = port;
	cout << "m_dwPort:" << m_dwPort << endl;
	cout << "m_strIP:" << m_strIP << endl;
	if (!bInit)
	{
		cout << "��ʼ������" << CLIENT_GetLastError() << endl;
		throw std::exception("Init failed");

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
	m_lLoginHandle = CLIENT_Login((char *)m_strIP.c_str(), m_dwPort, (char *)(m_strUser.c_str()), (char *)(m_strPasswords.c_str()), &m_deviceInfo, &nError);

	if (0 != nError)
	{
		cout << "��¼����(nError)��" << GetLastErrorString().c_str() << endl;
		throw std::exception("Login failed");
	}

	if (0 == m_lLoginHandle)
	{
		cout << "��¼����(lLogin)��" << GetLastErrorString().c_str() << endl;
	}

	if (m_lLoginHandle > 0)
	{
		int nRetLen = 0;
		NET_DEV_CHN_COUNT_INFO stuChn = { sizeof(NET_DEV_CHN_COUNT_INFO) };
		stuChn.stuVideoIn.dwSize = sizeof(stuChn.stuVideoIn);
		stuChn.stuVideoOut.dwSize = sizeof(stuChn.stuVideoOut);

		BOOL bDevState = CLIENT_QueryDevState(m_lLoginHandle, DH_DEVSTATE_DEV_CHN_COUNT, (char*)&stuChn, stuChn.dwSize, &nRetLen, 1000);

		if (bDevState)
		{
			m_DHChannels = stuChn.stuVideoIn.nMaxTotal;
		}
		else
		{
			cout << "Get channel failed" << GetLastErrorString() << endl;
			throw std::exception("Get channel failed");
			return;
		}
	}

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

void DHVendor::Logout()
{
	if (m_lLoginHandle > 0 && !CLIENT_Logout(m_lLoginHandle))
	{
		cout << "�˳�����" << GetLastErrorString().c_str() << endl;
		throw std::exception("Logout failed");
		return;
	}
}

void DHVendor::SearchAll()
{

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

void DHVendor::Search(const size_t channel, const time_range& range)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "���ȵ�¼!" << endl;
		return;
	}

	if (range.start >= range.end)
	{
		cout << "ʱ�䷶Χ����!" << endl;
		return;
	}

	//m_files.clear();

	vector<time_range> trInfor = MakeTimeRangeList(range);

	vector<time_range>::iterator it;

	for (it = trInfor.begin(); it != trInfor.end(); ++it)
	{
		NET_TIME ntStime;
		NET_TIME ntEtime;

		trTOnt(ntStime, ntEtime, *it);

		NET_RECORDFILE_INFO ifileinfo[MAX_SEARCH_COUNT];
		ZeroMemory(ifileinfo, sizeof(ifileinfo));
		int iMaxNum = 0;

		BOOL bRet = CLIENT_QueryRecordFile(m_lLoginHandle, channel, 0, &ntStime, &ntEtime, 0, ifileinfo, sizeof(ifileinfo), &iMaxNum, 5000, true);

		if (!bRet)
		{
			cout << "GetRecordFileList ��ѯ¼��ʧ�ܣ�����ԭ��" << GetLastErrorString() << endl;
			throw std::exception("Search file by time failed");
		}
		if (iMaxNum <= 0)
		{
			cout << "GetRecordFileList ��ѯ¼��ɹ���¼�����Ϊ0" << endl;
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

			//cout << "GetRecordFileList �ļ���:" << info.name <<endl<< "  " << "�ļ���С:" << info.size << "  " << "ͨ��:" << info.channel << endl;
		}
	}

	SaveSearchFileListToFile();
	cout << "totals:" << m_files.size() << endl;
}

void DHVendor::Download(const size_t channel, const time_range& range)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "���ȵ�¼!" << endl;
		return;
	}

	NET_TIME ntStime;
	NET_TIME ntEtime;

	trTOnt(ntStime, ntEtime, range);

	CreatePath(channel);

	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\DaHua\\ͨ��%d\\channel%d-%d%02d%02d%02d%02d%02d-%d%02d%02d%02d%02d%02d.dav", channel, channel, ntStime.dwYear, ntStime.dwMonth, ntStime.dwDay,
		ntStime.dwHour, ntStime.dwMinute, ntStime.dwSecond, ntEtime.dwYear, ntEtime.dwMonth, ntEtime.dwDay, ntEtime.dwHour, ntEtime.dwMinute, ntEtime.dwSecond);

	BOOL bRet = CLIENT_DownloadByTime(m_lLoginHandle, channel, 0, &ntStime, &ntEtime, szTime, BTDownLoadPos, (DWORD)this);
	cout << "strName:" << szTime << endl;

	if (0 == bRet)
	{
		cout << "downLoadByRecordFile ����¼��ʧ�ܣ�����ԭ��" << GetLastErrorString() << endl;
		throw std::exception("Download by Record file failed");
		return;
	}
	else
	{
		cout << "downLoadByRecordFile ����¼��ɹ���" << endl;
	}
}

void DHVendor::PlayVideo(const size_t channel, const time_range& range)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "PlayVideo ���߲���ʧ��ԭ��" << GetLastErrorString() << endl;
		throw std::exception("Login handle by Record file failed");
		return;
	}

	NET_TIME ntStime;
	NET_TIME ntEtime;

	trTOnt(ntStime, ntEtime, range);

	TestWindows Test;
	Test.Init();

	BOOL lPlayID = CLIENT_PlayBackByTimeEx(m_lLoginHandle, channel, &ntStime, &ntEtime, g_hWnd, PlayCallBack, (DWORD)this, PBDataCallBack, (DWORD)this);

	system("PAUSE");

	if (!lPlayID)
	{
		cout << "����ʧ��ԭ��" << GetLastErrorString() << endl;
		throw std::exception("Play back by time failed");
	}
}

void DHVendor::Download(const size_t channel, const std::string& filename)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "���ȵ�¼!" << endl;
		return;
	}

	CreatePath(channel);

	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\DaHua\\channel%d\\", channel);

	char szBuf[] = ".dav";
	strcat_s(szTime, (char *)filename.c_str());
	strcat_s(szTime, szBuf);

	cout << szTime << endl;

	vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{
			NET_RECORDFILE_INFO* pData = (NET_RECORDFILE_INFO*)it->getPrivateData();
			BOOL bRet = CLIENT_DownloadByRecordFile(m_lLoginHandle, pData, szTime, BTDownLoadFile, (DWORD)this);

			if (0 == bRet)
			{
				cout << "downLoadByRecordFile ����¼��ʧ�ܣ�����ԭ��" << GetLastErrorString() << endl;
				throw std::exception("Download by Record file failed");
				return;
			}
			else
			{
				cout << "downLoadByRecordFile ����¼��ɹ���" << endl;
			}
		}
		if (m_files.size() - 1 == nSize)
		{
			cout << "�����ļ��������ڣ�" << endl;
		}
		nSize++;
	}
}

void DHVendor::PlayVideo(const size_t channel, const std::string& filename)
{
	if (0 >= m_lLoginHandle)
	{
		cout << "PlayVideo ���߲���ʧ��ԭ��" << GetLastErrorString() << endl;
		return;
	}

	vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{
			TestWindows Test;
			Test.Init();

			NET_RECORDFILE_INFO* pData = (NET_RECORDFILE_INFO*)it->getPrivateData();
			BOOL lPlayID = CLIENT_PlayBackByRecordFile(m_lLoginHandle, pData, g_hWnd, PlayCallBack, (DWORD)this);

			system("PAUSE");

			if (!lPlayID)
			{
				cout << "����ʧ��ԭ��" << GetLastErrorString() << endl;
				throw std::exception("Play back by time failed");
			}
		}

		if (m_files.size() - 1 == nSize)
		{
			cout << "�����ļ��������ڣ�" << endl;
		}
		nSize++;
	}


	
}

void DHVendor::SetDownloadPath(const std::string& Root)
{

}

void DHVendor::throwException()
{

}

void DHVendor::BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser)
{
	
}

void  DHVendor::BTDownLoadFile(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{

}

void DHVendor::PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	if (dwUser == 0)
	{
		return;
	}
}

int DHVendor::PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser)
{
	if (dwUser == 0)
	{
		return -1;
	}

	return 1;
}

void DHVendor::CreatePath(const size_t channel)
{
	string strPath = "D:\\DownLoadVideo\\";

	BOOL bOne = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bOne)
	{
		cout << "Error!" << endl;
		return;
	}

	strPath.append("DaHua\\");
	BOOL bTwo = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bTwo)
	{
		cout << "Error!" << endl;
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
		cout << "Error!" << endl;
		return;
	}
}

void DHVendor::trTOnt(NET_TIME &ntStartTime, NET_TIME &ntEndTime, const time_range range)
{
	tm tmStartTime;
	tm tmEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);

	timeStdToDH(&tmStartTime, &ntStartTime);
	timeStdToDH(&tmEndTime, &ntEndTime);
}

vector<time_range> DHVendor::MakeTimeRangeList(const time_range& range)
{
	time_t timeStart = range.start;
	time_t timeEnd = range.end;
	vector<time_range> timeRangeList;

	tm tmStartTime;
	tm tmEndTime;
	NET_TIME ntStartTime;
	NET_TIME ntEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);


	timeStdToDH(&tmStartTime, &ntStartTime);
	timeStdToDH(&tmEndTime, &ntEndTime);

	if (timeEnd - timeStart <= ONE_DAY)
	{
		if (ntStartTime.dwDay == ntEndTime.dwDay)
		{
			timeRangeList.push_back(range);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - ntStartTime.dwHour) * ONE_HOUR + (59 - ntStartTime.dwMinute) * ONE_MINUTE + (59 - ntStartTime.dwSecond);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			rangeItem.start = timeStart + diff + 1;
			rangeItem.end = timeEnd;
			timeRangeList.push_back(rangeItem);
		}
	}
	else
	{
		time_t diff = timeEnd - timeStart;
		int day = (int)(diff / ONE_DAY + (diff%ONE_DAY > 0 ? 1 : 0));

		if (ntStartTime.dwHour == 0 && ntStartTime.dwMinute == 0 && ntStartTime.dwSecond == 0)
		{
			for (size_t i = 0; i < day - 1; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
			}

			time_range rangeItem;
			rangeItem.start = timeStart;
			rangeItem.end = timeEnd;
			timeRangeList.push_back(rangeItem);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - ntStartTime.dwHour) * ONE_HOUR + (59 - ntStartTime.dwMinute) * ONE_MINUTE + (59 - ntStartTime.dwSecond);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			timeStart = timeStart + diff + 1;
			for (size_t i = 0; i < day - 2; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
			}

			if (timeEnd > timeStart + ONE_DAY - 1)
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
				rangeItem.start = timeStart;
				rangeItem.end = timeEnd;
				timeRangeList.push_back(rangeItem);
			}
			else
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeEnd;
				timeRangeList.push_back(rangeItem);
			}
		}
	}

	return timeRangeList;
}

void DHVendor::SaveSearchFileListToFile()
{
	Document document;
	string configfile = "SearchFileList.config";
	document.Parse(configfile.c_str());
	ofstream ofs(configfile);
	OStreamWrapper osw(ofs);
	Document::AllocatorType& alloc = document.GetAllocator();

	Value root(kObjectType);

	for (size_t i = 0; i < m_files.size(); i++)
	{
		string fileKey = "videoFile";
		Value key(fileKey.c_str(), fileKey.length(), alloc);

		RecordFile file = m_files[i];
		Value name(file.name.c_str(), file.name.length(), alloc);
		Value channel(MakeStrByInteger(file.channel).c_str(), MakeStrByInteger(file.channel).length(), alloc);
		Value beginTime(MakeStrTimeByTimestamp(file.beginTime).c_str(), MakeStrTimeByTimestamp(file.beginTime).length(), alloc);
		Value endTime(MakeStrTimeByTimestamp(file.endTime).c_str(), MakeStrTimeByTimestamp(file.endTime).length(), alloc);
		Value size(MakeStrByInteger(file.size / 1024 / 1024).c_str(), MakeStrByInteger(file.size / 1024 / 1024).length(), alloc);

		Value a(kArrayType);
		a.PushBack(name, alloc).PushBack(channel, alloc).PushBack(beginTime, alloc).PushBack(endTime, alloc).PushBack(size, alloc);
		root.AddMember(key.Move(), a.Move(), alloc);
	}

	Writer<OStreamWrapper> writer(osw);
	root.Accept(writer);
}

void DHVendor::LoadSearchFileListFromFile()
{
	string configfile = "SearchFileList.config";
	ifstream ifs(configfile);
	IStreamWrapper isw(ifs);
	Document d;
	d.ParseStream(isw);
	size_t file_size = isw.Tell();
	if (isw.Tell() == 0)
	{
		return;
	}

	typedef Value::ConstMemberIterator Iter;
	for (Iter it = d.MemberBegin(); it != d.MemberEnd(); ++it)
	{
		string keyName = it->name.GetString();
		const Value& a = d[keyName.c_str()];

		assert(a.IsArray());
		if (!a.IsArray() || a.Size() < 5)
			continue;

		string fileName = a[0].GetString();
		string channel = a[1].GetString();
		string beginTime = a[2].GetString();
		string endTime = a[3].GetString();
		string size = a[4].GetString();
	}
}

string DHVendor::MakeStrTimeByTimestamp(time_t time)
{
	char cTime[50];
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime(cTime, 50, "%Y%m%d%H%M%S", &ttime);

	string strTime(cTime);

	return strTime;
}
string DHVendor::MakeStrByInteger(int data)
{
	char cData[50];

	sprintf_s(cData, 50, "%d", data);

	string strTime(cData);

	return strTime;
}

string DHVendor::GetLastErrorString()
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

#include "catch.hpp"

TEST_CASE_METHOD(DHVendor, "Init DH SDK", "[DHVendor]")
{

	time_range range;
	range.start = 1466438400;
	//range.end = 1466524800;
	range.end = 1466629200;
	//range.end = 1478833871;
	REQUIRE_NOTHROW(Init("192.168.0.96", 37777));

	REQUIRE_NOTHROW(Login("admin", ""));

	REQUIRE_NOTHROW(Search(0, range));
	//REQUIRE_NOTHROW(Search(1, range));

	REQUIRE_NOTHROW(Download(0, range));
	//Download(0, "channel0-20160621000000-20160621235959-0");

	//REQUIRE_NOTHROW(PlayVideo(0, range));
	REQUIRE_NOTHROW(PlayVideo(0, "channel0-20160621000000-20160621235959-0"));
	REQUIRE_NOTHROW(Logout());

}
