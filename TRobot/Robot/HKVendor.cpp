
#include "HKVendor.h"


HKVendor::HKVendor():
m_strName(""),
m_dwPort(0),
m_strUser(""),
m_strPasswords(""),
m_strIP(""),
m_lLoginHandle(0),
m_nChannels(0)
{
}


HKVendor::~HKVendor()
{
}

void HKVendor::Init()
{
	BOOL bInit = NET_DVR_Init();

	if (!bInit)
	{
		std::cout << "��ʼ������" << GetLastErrorString() << std::endl;
		throw std::exception("Init failed");
	}
	else
	{
		std::cout << "��ʼ��SDK�ɹ���" << std::endl;
	}

	BOOL bConnect = NET_DVR_SetConnectTime(1000, 30);
	BOOL bReConnect = NET_DVR_SetReconnect(3000);
	assert(bConnect);
	assert(bReConnect);
}

long HKVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	m_strIP = ip;
	m_dwPort = port;
	m_strUser = user;
	m_strPasswords = password;

	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID = NET_DVR_Login_V30((char *)m_strIP.c_str(), m_dwPort, (char *)(m_strUser.c_str()), (char *)(m_strPasswords.c_str()), &DeviceInfoTmp);
	m_lLoginHandle = lLoginID;
	if (-1 == lLoginID)
	{
		std::cout << "��¼ʧ��(Failed)��" << GetLastErrorString().c_str() << std::endl;
		throw std::exception("Login failed");
	}

	m_nChannels = DeviceInfoTmp.byIPChanNum;

	return lLoginID;
}

void HKVendor::Logout(const long loginHandle)
{
	if (m_lLoginHandle > 0 && !NET_DVR_Logout(m_lLoginHandle))
	{
		std::cout << "�˳�ʧ�ܣ�" << GetLastErrorString().c_str() << std::endl;
		throw std::exception("Logout failed");
		return;
	}
}

void HKVendor::SearchAll(const long loginHandle)
{

}

void HKVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	if (0 >= m_lLoginHandle)
	{
		std::cout << "���ȵ�¼!" << std::endl;
		return;
	}

	if (range.start >= range.end)
	{
		std::cout << "ʱ�䷶Χ����!" << std::endl;
		return;
	}

	std::vector<time_range> trInfor = MakeTimeRangeList(range);

	std::vector<time_range>::iterator it;
	for (it = trInfor.begin(); it != trInfor.end(); ++it)
	{
		NET_DVR_TIME ndtStartTime;
		NET_DVR_TIME ndtEndTime;

		trTOndt(ndtStartTime, ndtEndTime, range);

		NET_DVR_FILECOND FileCond;
		FileCond.lChannel = channel;
		FileCond.dwFileType = 0xff;
		FileCond.dwIsLocked = 0xff;
		FileCond.dwUseCardNo = 0;
		FileCond.struStartTime = ndtStartTime;
		FileCond.struStopTime = ndtEndTime;

		LONG hFindHandle = NET_DVR_FindFile_V30(m_lLoginHandle, &FileCond);
		if (-1 == hFindHandle)
		{
			return;
		}
		else
		{
			NET_DVR_FINDDATA_V30 FindData;  //���ҵ����ļ���Ϣ
			RecordFile rf;

			int ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
			int ItemIndex = 0;
			
			while (ret > 0)
			{

				if (NET_DVR_FILE_EXCEPTION == ret)
				{
					std::cout << "�����ļ�ʱ�쳣!" << GetLastErrorString() << std::endl;
					throw std::exception("SearchFile unusual failed");
					break;
				}
				else if (NET_DVR_FILE_NOFIND == ret)
				{
					std::cout << "û��¼���ļ�" << std::endl;
					break;

				}
				else if (NET_DVR_NOMOREFILE == ret)   //���ҽ���
				{
					std::cout << "��ѯ������" << std::endl;
					break;
				}
				else if (NET_DVR_ISFINDING == ret)  //���ڲ���
				{
					ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
					Sleep(5);
				}
				else if (NET_DVR_FILE_SUCCESS == ret)  //��ȡ�ļ���Ϣ�ɹ�
				{
					//�����ļ���Ϣ
					//rf

					ItemIndex++;
					//				g_vecFileInfo.push_back(FindData);
					//m_files.push_back();
					ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);

				}
			}
			//�رղ��ң��ͷž��
			NET_DVR_FindClose_V30(hFindHandle);
		}

	}
}

void HKVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{

}

void HKVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{

}

void HKVendor::Download(const long loginHandle, const size_t channel, const std::string& filename)
{

}

void HKVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{

}

void HKVendor::SetDownloadPath(const std::string& Root)
{

}

void HKVendor::throwException()
{

}

void HKVendor::timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd)
{
	pTimeStd->tm_year = pTimeDH->dwYear - 1900;
	pTimeStd->tm_mon = pTimeDH->dwMonth - 1;
	pTimeStd->tm_mday = pTimeDH->dwDay;
	pTimeStd->tm_hour = pTimeDH->dwHour;
	pTimeStd->tm_min = pTimeDH->dwMinute;
	pTimeStd->tm_sec = pTimeDH->dwSecond;
}

void HKVendor::trTOndt(NET_DVR_TIME &ndtStartTime, NET_DVR_TIME &ndtEndTime, const time_range range)
{
	tm tmStartTime;
	tm tmEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);

	timeStdToDH(&tmStartTime, &ndtStartTime);
	timeStdToDH(&tmEndTime, &ndtEndTime);
}

void HKVendor::timeStdToDH(tm *pTimeStd, NET_DVR_TIME *pTimeDH)
{
	pTimeDH->dwYear = pTimeStd->tm_year + 1900;
	pTimeDH->dwMonth = pTimeStd->tm_mon + 1;
	pTimeDH->dwDay = pTimeStd->tm_mday;
	pTimeDH->dwHour = pTimeStd->tm_hour;
	pTimeDH->dwMinute = pTimeStd->tm_min;
	pTimeDH->dwSecond = pTimeStd->tm_sec;
}

std::vector<time_range> HKVendor::MakeTimeRangeList(const time_range& range)
{
	time_t timeStart = range.start;
	time_t timeEnd = range.end;
	std::vector<time_range> timeRangeList;

	tm tmStartTime;
	tm tmEndTime;
	NET_DVR_TIME ntStartTime;
	NET_DVR_TIME ntEndTime;

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

std::string HKVendor::GetLastErrorString()
{
	DWORD dwError;
	dwError = NET_DVR_GetLastError();

	switch (dwError)
	{
	case NET_DVR_NOERROR: return "NOERROR";
	case NET_DVR_PASSWORD_ERROR: return "�û����������";
	case NET_DVR_NOENOUGHPRI: return "Ȩ�޲���";
	case NET_DVR_NOINIT: return "û�г�ʼ��";
	case NET_DVR_CHANNEL_ERROR: return "ͨ���Ŵ���";
	case NET_DVR_OVER_MAXLINK: return "���ӵ�DVR�Ŀͻ��˸����������";
	case NET_DVR_VERSIONNOMATCH: return "�汾��ƥ��";
	case NET_DVR_NETWORK_FAIL_CONNECT: return "���ӷ�����ʧ��";
	case NET_DVR_NETWORK_SEND_ERROR: return "�����������ʧ��";
	case NET_DVR_NETWORK_RECV_ERROR: return "�ӷ�������������ʧ��";
	case NET_DVR_NETWORK_RECV_TIMEOUT: return "�ӷ������������ݳ�ʱ";
	case NET_DVR_NETWORK_ERRORDATA: return "���͵���������";
	case NET_DVR_ORDER_ERROR: return "���ô������";
	case NET_DVR_OPERNOPERMIT: return "�޴�Ȩ��";
	case NET_DVR_COMMANDTIMEOUT: return "DVR����ִ�г�ʱ";
	case NET_DVR_ERRORSERIALPORT: return "���ںŴ���";
	case NET_DVR_ERRORALARMPORT: return "�����˿ڴ���";
	case NET_DVR_PARAMETER_ERROR: return "��������";
	case NET_DVR_CHAN_EXCEPTION: return "������ͨ�����ڴ���״̬";
	case NET_DVR_NODISK: return "û��Ӳ��";
	case NET_DVR_ERRORDISKNUM: return "Ӳ�̺Ŵ���";
	case NET_DVR_DISK_FULL: return "������Ӳ����";
	case NET_DVR_DISK_ERROR: return "������Ӳ�̳���";
	case NET_DVR_NOSUPPORT: return "��������֧��";
	case NET_DVR_BUSY: return "������æ";
	case NET_DVR_MODIFY_FAIL: return "�������޸Ĳ��ɹ�";
	case NET_DVR_PASSWORD_FORMAT_ERROR: return "���������ʽ����ȷ";
	case NET_DVR_DISK_FORMATING: return "Ӳ�����ڸ�ʽ����������������";
	case NET_DVR_DVRNORESOURCE: return "DVR��Դ����";
	case NET_DVR_DVROPRATEFAILED: return "DVR����ʧ��";
	case NET_DVR_OPENHOSTSOUND_FAIL: return "��PC����ʧ��";
	case NET_DVR_DVRVOICEOPENED: return "�����������Խ���ռ��";
	case NET_DVR_TIMEINPUTERROR: return "ʱ�����벻��ȷ";
	case NET_DVR_NOSPECFILE: return "�ط�ʱ������û��ָ�����ļ�";
	case NET_DVR_CREATEFILE_ERROR: return "�����ļ�����";
	case NET_DVR_FILEOPENFAIL: return "���ļ�����";
	case NET_DVR_OPERNOTFINISH: return "�ϴεĲ�����û�����";
	case NET_DVR_GETPLAYTIMEFAIL: return "��ȡ��ǰ���ŵ�ʱ�����";
	case NET_DVR_PLAYFAIL: return "���ų���";
	case NET_DVR_FILEFORMAT_ERROR: return "�ļ���ʽ����ȷ";
	case NET_DVR_DIR_ERROR: return "·������";
	case NET_DVR_ALLOC_RESOURCE_ERROR: return "��Դ�������";
	case NET_DVR_AUDIO_MODE_ERROR: return "����ģʽ����";
	case NET_DVR_NOENOUGH_BUF: return "������̫С";
	case NET_DVR_CREATESOCKET_ERROR: return "����SOCKET����";
	case NET_DVR_SETSOCKET_ERROR: return "����SOCKET����";
	case NET_DVR_MAX_NUM: return "�����ﵽ���";
	case NET_DVR_USERNOTEXIST: return "�û�������";
	case NET_DVR_WRITEFLASHERROR: return "дFLASH����";
	case NET_DVR_UPGRADEFAIL: return "DVR����ʧ��";
	case NET_DVR_CARDHAVEINIT: return "���뿨�Ѿ���ʼ����";
	case NET_DVR_PLAYERFAILED: return "���ò��ſ���ĳ������ʧ��";
	case NET_DVR_MAX_USERNUM: return "�豸���û����ﵽ���";
	case NET_DVR_GETLOCALIPANDMACFAIL: return "��ÿͻ��˵�IP��ַ�������ַʧ��";
	case NET_DVR_NOENCODEING: return "��ͨ��û�б���";
	case NET_DVR_IPMISMATCH: return "IP��ַ��ƥ��";
	case NET_DVR_MACMISMATCH: return "MAC��ַ��ƥ��";
	case NET_DVR_UPGRADELANGMISMATCH: return "�����ļ����Բ�ƥ��";
	case NET_DVR_MAX_PLAYERPORT: return "������·���ﵽ���";
	case NET_DVR_NOSPACEBACKUP: return "�����豸��û���㹻�ռ���б���";
	case NET_DVR_NODEVICEBACKUP: return "û���ҵ�ָ���ı����豸";
	case NET_DVR_PICTURE_BITS_ERROR: return "ͼ����λ����������24ɫ";
	case NET_DVR_PICTURE_DIMENSION_ERROR: return "ͼƬ��*���ޣ� ��128*256";
	case NET_DVR_PICTURE_SIZ_ERROR: return "ͼƬ��С���ޣ���100K";
	case NET_DVR_LOADPLAYERSDKFAILED: return "���뵱ǰĿ¼��Player Sdk����";
	case NET_DVR_LOADPLAYERSDKPROC_ERROR: return "�Ҳ���Player Sdk��ĳ���������";
	case NET_DVR_LOADDSSDKFAILED: return "���뵱ǰĿ¼��DSsdk����";
	case NET_DVR_LOADDSSDKPROC_ERROR: return "�Ҳ���DsSdk��ĳ���������";
	case NET_DVR_DSSDK_ERROR: return "����Ӳ�����DsSdk��ĳ������ʧ��";
	case NET_DVR_VOICEMONOPOLIZE: return "��������ռ";
	case NET_DVR_JOINMULTICASTFAILED: return "����ಥ��ʧ��";
	case NET_DVR_CREATEDIR_ERROR: return "������־�ļ�Ŀ¼ʧ��";
	case NET_DVR_BINDSOCKET_ERROR: return "���׽���ʧ��";
	case NET_DVR_SOCKETCLOSE_ERROR: return "socket�����жϣ��˴���ͨ�������������жϻ�Ŀ�ĵز��ɴ�";
	case NET_DVR_USERID_ISUSING: return "ע��ʱ�û�ID���ڽ���ĳ����";
	case NET_DVR_SOCKETLISTEN_ERROR: return "����ʧ��";
	case NET_DVR_PROGRAM_EXCEPTION: return "�����쳣";
	case NET_DVR_WRITEFILE_FAILED: return "д�ļ�ʧ��";
	case NET_DVR_FORMAT_READONLY: return "��ֹ��ʽ��ֻ��Ӳ��";
	case NET_DVR_WITHSAMEUSERNAME: return "�û����ýṹ�д�����ͬ���û���";
	case NET_DVR_DEVICETYPE_ERROR: return "�������ʱ�豸�ͺŲ�ƥ��";
	case NET_DVR_LANGUAGE_ERROR: return "�������ʱ���Բ�ƥ��";
	case NET_DVR_PARAVERSION_ERROR: return "�������ʱ����汾��ƥ��";
	case NET_DVR_IPCHAN_NOTALIVE: return "Ԥ��ʱ���IPͨ��������";
	case NET_DVR_RTSP_SDK_ERROR: return "���ظ���IPCͨѶ��StreamTransClient.dllʧ��";
	case NET_DVR_CONVERT_SDK_ERROR: return "����ת���ʧ��";
	case NET_DVR_IPC_COUNT_OVERFLOW: return "��������ip����ͨ����";
	case NET_DVR_MAX_ADD_NUM: return "��ӱ�ǩ(һ���ļ�Ƭ��64)�ȸ����ﵽ���";
	case NET_DVR_PARAMMODE_ERROR: return "ͼ����ǿ�ǣ�����ģʽ��������Ӳ������ʱ���ͻ��˽����������ʱ����ֵ��";
	case NET_DVR_CODESPITTER_OFFLINE: return "��Ƶ�ۺ�ƽ̨�������������";
	case NET_DVR_BACKUP_COPYING: return "�豸���ڱ���";
	case NET_DVR_CHAN_NOTSUPPORT: return "ͨ����֧�ָò���";
	case NET_DVR_CALLINEINVALID: return "�߶���λ��̫���л򳤶��߲�����б";
	case NET_DVR_CALCANCELCONFLICT: return "ȡ���궨��ͻ����������˹���ȫ�ֵ�ʵ�ʴ�С�ߴ����";
	case NET_DVR_CALPOINTOUTRANGE: return "�궨�㳬����Χ";
	case NET_DVR_FILTERRECTINVALID: return "�ߴ������������Ҫ��";
	case NET_DVR_DDNS_DEVOFFLINE: return "�豸û��ע�ᵽddns��";
	case NET_DVR_DDNS_INTER_ERROR: return "DDNS �������ڲ�����";
	case NET_DVR_FUNCTION_NOT_SUPPORT_OS: return "�˹��ܲ�֧�ָò���ϵͳ";
	case NET_DVR_DEC_CHAN_REBIND: return "����ͨ������ʾ�����������";
	case NET_DVR_INTERCOM_SDK_ERROR: return "���ص�ǰĿ¼�µ������Խ���ʧ��";
	case NET_DVR_NO_CURRENT_UPDATEFILE: return "û����ȷ��������";
	case NET_DVR_USER_NOT_SUCC_LOGIN: return "�û���û��½�ɹ�";
	case NET_DVR_USE_LOG_SWITCH_FILE: return "����ʹ����־�����ļ�";
	case NET_DVR_POOL_PORT_EXHAUST: return "�˿ڳ������ڰ󶨵Ķ˿��Ѻľ�";
	case NET_DVR_PACKET_TYPE_NOT_SUPPORT: return "������װ��ʽ����";
	case NET_DVR_ALIAS_DUPLICATE: return "�����ظ�";
	default:
		return "δ֪����";
	}
}

#include "catch.hpp"

TEST_CASE_METHOD(HKVendor, "Init HK SDK", "[HKVendor]")
{

	time_range range;
	range.start = 1466438400;
	range.end = 1466524800;
	range.end = 1466629200;
	range.end = 1478833871;
	REQUIRE_NOTHROW(Init());
 
 	REQUIRE_NOTHROW(Login("192.168.0.92", 8000, "admin", "admin123"));

	// 	REQUIRE_NOTHROW(Search(0, range));
	// 	//REQUIRE_NOTHROW(Search(1, range));
	// 
	// 	REQUIRE_NOTHROW(Download(0, range));
	// 	//Download(0, "channel0-20160621000000-20160621235959-0");
	// 
	// 	//REQUIRE_NOTHROW(PlayVideo(0, range));
	// 	REQUIRE_NOTHROW(PlayVideo(0, "channel0-20160621000000-20160621235959-0"));
	// 	REQUIRE_NOTHROW(Logout());
}