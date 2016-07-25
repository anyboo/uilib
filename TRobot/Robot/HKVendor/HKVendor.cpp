#include "HKVendor.h"

// HK SDK
#include "HCNetSDK.h"

#pragma comment(lib, "HCNetSDK")

class HK_SDK_INTERFACE
{
public:
	static void HK_timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd);
	static void HK_timeStdToDH(tm *pTimeStd, NET_DVR_TIME *pTimeDH);
	static void HK_trTOndt(NET_DVR_TIME &ndtStartTime, NET_DVR_TIME &ndtEndTime, const time_range range);

	static std::string HK_GetLastErrorString();

	static std::string HK_MakeStrByInteger(int data);
	static std::string HK_MakeStrTimeByTimestamp(time_t time);

	static bool HK_isGetDVRConfig(const long loginHandle);
	static size_t HK_getChannel(const long loginHandle, size_t channel);
};


HKVendor::HKVendor()
{
	m_eSDKType = HK_SDK;
	m_sDefUserName = "admin";
	m_sDefPassword = "12345";
	m_iMaxChannel = 0;
	m_bSearchDeviceAPI = true;

	m_lSearchDeviceHandle = -1;

	m_sRoot = "D:\\DownLoadVideo\\";
}

HKVendor::~HKVendor()
{
	m_iMaxChannel = 0;
	m_lSearchDeviceHandle = -1;

	assert(NET_DVR_Cleanup());
}

void HKVendor::Init()
{
	BOOL bInit = NET_DVR_Init();

	if (!bInit)
	{
		std::cout << "The HK SDK initialization error��" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Init failed");
	}
	else
	{
		std::cout << "The HK SDK initialization success!" << std::endl;
	}

	BOOL bConnect = NET_DVR_SetConnectTime(1000, 30);
	BOOL bReConnect = NET_DVR_SetReconnect(3000);
	assert(bConnect);
	assert(bReConnect);
}

long HKVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID = -1;
	lLoginID  = NET_DVR_Login_V30((char *)ip.c_str(), port, (char *)(user.c_str()), (char *)(password.c_str()), &DeviceInfoTmp);
	
	if (-1 == lLoginID)
	{
		std::cout << "The HK SDK login failed:" << HK_SDK_INTERFACE::HK_GetLastErrorString().c_str() << std::endl;
		//throw std::exception("Login failed");
		return -1;
	}

	m_iMaxChannel = DeviceInfoTmp.byIPChanNum;

	return lLoginID;
}

void HKVendor::Logout(const long loginHandle)
{
	if (loginHandle < 0 && !NET_DVR_Logout(loginHandle))
	{
		std::cout << "The HK SDK logout failed:" << HK_SDK_INTERFACE::HK_GetLastErrorString().c_str() << std::endl;
		throw std::exception("Logout failed");
		return;
	}
}

void HKVendor::SearchAll(const long loginHandle)
{

}

void HKVendor::ClearLocalRecordFiles()
{
	m_files.clear();
}
	

void HKVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	if (-1 == loginHandle)
	{
		std::cout << "Please login first!" << std::endl;
		return;
	}

	if (range.start >= range.end)
	{
		std::cout << "Time range is wrong!" << std::endl;
		return;
	}

	if (!m_FilesChange.empty())
	{
		m_FilesChange.clear();
	}

	size_t sChannel = HK_SDK_INTERFACE::HK_getChannel(loginHandle, channel);

	NET_DVR_TIME ndtStartTime;
	NET_DVR_TIME ndtEndTime;

	HK_SDK_INTERFACE::HK_trTOndt(ndtStartTime, ndtEndTime, range);

	NET_DVR_FILECOND FileCond;
	FileCond.lChannel = sChannel;
	FileCond.dwFileType = 0xff;
	FileCond.dwIsLocked = 0xff;
	FileCond.dwUseCardNo = 0;
	FileCond.struStartTime = ndtStartTime;
	FileCond.struStopTime = ndtEndTime;

	LONG hFindHandle = NET_DVR_FindFile_V30(loginHandle, &FileCond);

	if (-1 == hFindHandle)
	{
		std::cout << "search error:" << HK_SDK_INTERFACE::HK_GetLastErrorString() << endl;
		return;
	}
	else
	{
		NET_DVR_FINDDATA_V30 FindData;  //The find file information
		RecordFile rf;

		int ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
		tm tmStart;
		tm tmEnd;

		while (ret > 0)
		{

			if (NET_DVR_FILE_EXCEPTION == ret)
			{
				std::cout << "Find file Abnormally:" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
				throw SearchFileException(HK_SDK_INTERFACE::HK_GetLastErrorString().c_str());
				break;
			}
			else if (NET_DVR_FILE_NOFIND == ret)
			{
				std::cout << "No video file!" << std::endl;
				break;

			}
			else if (NET_DVR_NOMOREFILE == ret)   //End of the query
			{
				std::cout << "End of the query!" << std::endl;
				break;
			}
			else if (NET_DVR_ISFINDING == ret)  //Finding
			{
				ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
			}
			else if (NET_DVR_FILE_SUCCESS == ret)  //File information successfully
			{
				HK_SDK_INTERFACE::HK_timeDHToStd(&FindData.struStartTime, &tmStart);
				HK_SDK_INTERFACE::HK_timeDHToStd(&FindData.struStopTime, &tmEnd);


				//Save file information
				rf.name = FindData.sFileName;
				rf.channel = channel;
				rf.size = FindData.dwFileSize / 1024 / 1024;

				rf.beginTime = mktime(&tmStart);
				rf.endTime = mktime(&tmEnd);

				m_FilesChange.push_back(rf);
				m_files.push_back(rf);

				ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);

				std::cout << "FileName:" << rf.name << endl << "  " << "FileSize:" << rf.size << "  " << "Channel:" << rf.channel << endl;
			}
		}

		//Close the lookup, release the handle
		NET_DVR_FindClose_V30(hFindHandle);
	}

	// Save Search Video List Result to Config File
	CCommonUtrl::getInstance().SaveSearchFileListToFile(m_FilesChange, Vendor_HK_Abbr);

	// Write File List to DB
	CCommonUtrl::getInstance().WriteFileListToDB(m_FilesChange);
}

void HKVendor::Download(const long loginHandle, const RecordFile& file)
{
	if (-1 == loginHandle)
	{
		std::cout << "Please login frist!" << std::endl;
		return;
	}

	// Init File Starttime and Endtime
	std::string strTimeStart;
	std::string strTimeEnd;
	std::string strTimeStartZero;
	std::string strTimeEndZero;

	struct tm ttime;

	localtime_s(&ttime, &file.beginTime);
	strftime((char *)strTimeStart.data(), 24, "%Y%m%d%H%M%S", &ttime);
	strftime((char *)strTimeStartZero.data(), 24, "%Y%m%d0000", &ttime);
	strftime((char *)strTimeEndZero.data(), 24, "%Y%m%d2359", &ttime);
	localtime_s(&ttime, &file.endTime);
	strftime((char *)strTimeEnd.data(), 24, "%Y%m%d%H%M%S", &ttime);

	std::string strFileName = CCommonUtrl::getInstance().MakeFileName(file.channel, strTimeStart, strTimeEnd, ".mp4");

	std::string strPath = CCommonUtrl::getInstance().MakeDownloadFileFolder(m_sRoot, strTimeStartZero, strTimeEndZero, Vendor_HK, file.channel);

	strPath += strFileName;

	LONG lRet = NET_DVR_GetFileByName(loginHandle, (char *)file.name.c_str(), (char *)strPath.c_str());

	if (0 < lRet)
	{
		std::cout << "Download videos failed��" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Download by Record file failed");
		return;
	}
	else
	{
		NET_DVR_PlayBackControl(lRet, NET_DVR_PLAYSTART, 0, NULL);

		// 				DWORD pos;
		// 				pos = NET_DVR_GetDownloadPos(lRet);
		// 
		// 				while (pos < 100)
		// 				{
		// 					pos = NET_DVR_GetDownloadPos(lRet);
		// 					std::cout << "Pos:" << pos << std::endl;
		// 				}


		std::cout << "Download videos success!" << std::endl;
	}
}

void HKVendor::PlayVideo(const long loginHandle, const RecordFile& file)
{
	if (-1 == loginHandle)
	{
		std::cout << "Streaming video to fail��" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Login handle by Record file failed");
		return;
	}

	NET_DVR_TIME ndtStime;
	NET_DVR_TIME ndtEtime;
	time_range range;
	range.start = file.beginTime;
	range.end = file.endTime;

	HK_SDK_INTERFACE::HK_trTOndt(ndtStime, ndtEtime, range);

	TestWindows Test;
	Test.Init();

	size_t sChannel = HK_SDK_INTERFACE::HK_getChannel(loginHandle, file.channel);
	LONG hPlayHandle = NET_DVR_PlayBackByTime(loginHandle, sChannel, &ndtStime, &ndtEtime, Test.m_hWnd);

	if (hPlayHandle < 0)
	{
		std::cout << "Play the failure��" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}

	if (!NET_DVR_PlayBackControl(hPlayHandle, NET_DVR_PLAYSTART, NULL, 0))
	{
		std::cout << "Play the failure��" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}

	system("PAUSE");
}

bool HKVendor::StopDownload()
{
	return true;
}

void HKVendor::SetDownloadPath(const std::string& Root)
{
	m_sRoot = Root;
}

void HKVendor::throwException()
{

}

void HK_SDK_INTERFACE::HK_timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd)
{
	pTimeStd->tm_year = pTimeDH->dwYear - 1900;
	pTimeStd->tm_mon = pTimeDH->dwMonth - 1;
	pTimeStd->tm_mday = pTimeDH->dwDay;
	pTimeStd->tm_hour = pTimeDH->dwHour;
	pTimeStd->tm_min = pTimeDH->dwMinute;
	pTimeStd->tm_sec = pTimeDH->dwSecond;
}

void HK_SDK_INTERFACE::HK_trTOndt(NET_DVR_TIME &ndtStartTime, NET_DVR_TIME &ndtEndTime, const time_range range)
{
	tm tmStartTime;
	tm tmEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);

	HK_SDK_INTERFACE::HK_timeStdToDH(&tmStartTime, &ndtStartTime);
	HK_SDK_INTERFACE::HK_timeStdToDH(&tmEndTime, &ndtEndTime);
}

void HK_SDK_INTERFACE::HK_timeStdToDH(tm *pTimeStd, NET_DVR_TIME *pTimeDH)
{
	pTimeDH->dwYear = pTimeStd->tm_year + 1900;
	pTimeDH->dwMonth = pTimeStd->tm_mon + 1;
	pTimeDH->dwDay = pTimeStd->tm_mday;
	pTimeDH->dwHour = pTimeStd->tm_hour;
	pTimeDH->dwMinute = pTimeStd->tm_min;
	pTimeDH->dwSecond = pTimeStd->tm_sec;
}

std::string HK_SDK_INTERFACE::HK_MakeStrTimeByTimestamp(time_t time)
{
	char cTime[50];
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime(cTime, 50, "%Y%m%d%H%M%S", &ttime);

	string strTime(cTime);

	return strTime;
}
std::string HK_SDK_INTERFACE::HK_MakeStrByInteger(int data)
{
	char cData[50];

	sprintf_s(cData, 50, "%d", data);

	string strTime(cData);

	return strTime;
}

bool HK_SDK_INTERFACE::HK_isGetDVRConfig(const long loginHandle)
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg;
	memset(&IpAccessCfg, 0, sizeof(IpAccessCfg));
	DWORD  dwReturned;

	return  NET_DVR_GetDVRConfig(loginHandle, NET_DVR_GET_IPPARACFG_V40, 0, &IpAccessCfg, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned);
}

size_t HK_SDK_INTERFACE::HK_getChannel(const long loginHandle, size_t channel)
{
	//��ȡ�豸��������Ϣ�ɹ�������ģ��ͨ��
	if (HK_isGetDVRConfig(loginHandle))
	{
		return channel += 33;
	}
	else
	{
		return channel + 1;
	}
}

void HKVendor::StartSearchDevice()
{

}

void HKVendor::StopSearchDevice()
{

}

bool HKVendor::IsSearchDeviceAPIExist()
{
	return m_bSearchDeviceAPI;
}


std::string HK_SDK_INTERFACE::HK_GetLastErrorString()
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