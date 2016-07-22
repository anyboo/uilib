#include "DZPVendor.h"

#include "Poco/NotificationQueue.h"

// JXJ DZP
#include "netsdk.h"
#include "H264Play.h"

using Poco::NotificationQueue;

#pragma comment(lib, "NetSdk")
#pragma comment(lib, "H264Play")


class DZP_SDK_INTERFACE
{
public:
	//static std::string DZP_MakeFileName(int channel, const std::string& startTime);
	static void DZP_SearchUnit(const long loginHandle, const size_t channel, const time_range& range, RECORD_FILE_LIST& recordFiles);
	static void __stdcall DZP_DownLoadPosCallBack(long lPlayHandle, long lTotalSize, long lDownLoadSize, long dwUser);
	static int __stdcall DZP_RealDataCallBack(long lRealHandle, long dwDataType, unsigned char *pBuffer, long lbufsize, long dwUser);
	static int DZP_GetDownloadPos(const int fileHandle);
	static H264_DVR_FILE_DATA DZP_MakeRecordFileToH264Data(const RecordFile& file);
	static SDK_SYSTEM_TIME DZP_MakeH264TimeToSDKTime(H264_DVR_TIME h264_time);

	static int m_lFileHandle;
};

int DZP_SDK_INTERFACE::m_lFileHandle = 0;

std::string GZLL_GetLastErrorString(int error)
{
	switch (error)
	{
	case H264_DVR_NOERROR: return std::string("û�д���");
	case H264_DVR_SUCCESS: return std::string("���سɹ�");
	case H264_DVR_SDK_NOTVALID: return std::string("�Ƿ�����");
	case H264_DVR_NO_INIT: return std::string("SDKδ����ʼ��");
	case H264_DVR_ILLEGAL_PARAM: return std::string("�û��������Ϸ�");
	case H264_DVR_INVALID_HANDLE: return std::string("�����Ч");
	case H264_DVR_SDK_UNINIT_ERROR: return std::string("SDK�������");
	case H264_DVR_SDK_TIMEOUT: return std::string("�ȴ���ʱ");
	case H264_DVR_SDK_MEMORY_ERROR: return std::string("�ڴ���󣬴����ڴ�ʧ��");
	case H264_DVR_SDK_NET_ERROR: return std::string("�������");
	case H264_DVR_SDK_OPEN_FILE_ERROR: return std::string("���ļ�ʧ��");
	case H264_DVR_SDK_UNKNOWNERROR: return std::string("δ֪����");
	case H264_DVR_DEV_VER_NOMATCH: return std::string("�յ����ݲ���ȷ�����ܰ汾��ƥ��");
	case H264_DVR_SDK_NOTSUPPORT: return std::string("�汾��֧��");
	case H264_DVR_OPEN_CHANNEL_ERROR: return std::string("��ͨ��ʧ��");
	case H264_DVR_CLOSE_CHANNEL_ERROR: return std::string("�ر�ͨ��ʧ��");
	case H264_DVR_SUB_CONNECT_ERROR: return std::string("����ý��������ʧ��");
	case H264_DVR_SUB_CONNECT_SEND_ERROR: return std::string("ý��������ͨѶʧ��");
	case H264_DVR_NATCONNET_REACHED_MAX: return std::string("Nat��Ƶ���Ӵﵽ��󣬲������µ�Nat��Ƶ����");
	case H264_DVR_NOPOWER: return std::string("��Ȩ��");
	case H264_DVR_PASSWORD_NOT_VALID: return std::string("�˺����벻��");
	case H264_DVR_LOGIN_USER_NOEXIST: return std::string("�û�������");
	case H264_DVR_USER_LOCKED: return std::string("���û�������");
	case H264_DVR_USER_IN_BLACKLIST: return std::string("���û����������(�ں�������)");
	case H264_DVR_USER_HAS_USED: return std::string("���û��Ե�½");
	case H264_DVR_USER_NOT_LOGIN: return std::string("���û�û�е�½");
	case H264_DVR_CONNECT_DEVICE_ERROR: return std::string("�����豸������");
	case H264_DVR_ACCOUNT_INPUT_NOT_VALID: return std::string("�û��������벻�Ϸ�");
	case H264_DVR_ACCOUNT_OVERLAP: return std::string("�����ظ�");
	case H264_DVR_ACCOUNT_OBJECT_NONE: return std::string("�����ڶ���, ���ڲ�ѯʱ");
	case H264_DVR_ACCOUNT_OBJECT_NOT_VALID: return std::string("�����ڶ���");
	case H264_DVR_ACCOUNT_OBJECT_IN_USE: return std::string("��������ʹ��");
	case H264_DVR_ACCOUNT_SUBSET_OVERLAP: return std::string("�Ӽ�����Χ (�����Ȩ�޳���Ȩ�ޱ��û�Ȩ�޳������Ȩ�޷�Χ�ȵ�)");
	case H264_DVR_ACCOUNT_PWD_NOT_VALID: return std::string("���벻��ȷ");
	case H264_DVR_ACCOUNT_PWD_NOT_MATCH: return std::string("���벻ƥ��");
	case H264_DVR_ACCOUNT_RESERVED: return std::string("�����ʺ�");
	case H264_DVR_OPT_RESTART: return std::string("�������ú���Ҫ����Ӧ�ó���");
	case H264_DVR_OPT_REBOOT: return std::string("��Ҫ����ϵͳ");
	case H264_DVR_OPT_FILE_ERROR: return std::string("д�ļ�����");
	case H264_DVR_OPT_CAPS_ERROR: return std::string("�������Բ�֧��");
	case H264_DVR_OPT_VALIDATE_ERROR: return std::string("����У��ʧ��");
	case H264_DVR_OPT_CONFIG_NOT_EXIST: return std::string("����������õ����ò�����");
	case H264_DVR_CTRL_PAUSE_ERROR: return std::string("��ͣʧ��");
	case H264_DVR_SDK_NOTFOUND: return std::string("����ʧ�ܣ�û���ҵ���Ӧ�ļ�");
	case H264_DVR_CFG_NOT_ENABLE: return std::string("����δ����");
	case H264_DVR_DECORD_FAIL: return std::string("����ʧ��");
	case H264_DVR_SOCKET_ERROR: return std::string("�����׽���ʧ��");
	case H264_DVR_SOCKET_CONNECT: return std::string("�����׽���ʧ��");
	case H264_DVR_SOCKET_DOMAIN: return std::string("��������ʧ��");
	case H264_DVR_SOCKET_SEND: return std::string("��������ʧ��");
	case H264_DVR_ARSP_NO_DEVICE: return std::string("û�л�ȡ���豸��Ϣ���豸Ӧ�ò�����");
	case H264_DVR_ARSP_BUSING: return std::string("ARSP����æ");
	case H264_DVR_ARSP_BUSING_SELECT: return std::string("ARSP����æ,selectʧ��");
	case H264_DVR_ARSP_BUSING_RECVICE: return std::string("ARSP����æ,recviceʧ��");
	case H264_DVR_CONNECTSERVER_ERROR: return std::string("���ӷ�����ʧ��");
	case H264_DVR_CONNECT_FULL: return std::string("����������������");
	case H264_DVR_PIRATESOFTWARE: return std::string("�豸����");
	}

	return "";
}

std::string GZLL_GetLastErrorString()
{
	return GZLL_GetLastErrorString(H264_DVR_GetLastError());
}

void NetTimeToTM(const SDK_SYSTEM_TIME& nt, tm& t)
{
	t.tm_year = nt.year - 1900;
	t.tm_mon = nt.month - 1;
	t.tm_mday = nt.day;
	t.tm_hour = nt.hour;
	t.tm_min = nt.minute;
	t.tm_sec = nt.second;
}

void TMToNetTime(const tm& t, H264_DVR_TIME& nt)
{
	nt.dwYear = t.tm_year + 1900;
	nt.dwMonth = t.tm_mon + 1;
	nt.dwDay = t.tm_mday;
	nt.dwHour = t.tm_hour;
	nt.dwMinute = t.tm_min;
	nt.dwSecond = t.tm_sec;

}

CDZPVendor::CDZPVendor()
{
	m_eSDKType = DZP_SDK;
	m_bSearchDeviceAPI = true;
	m_sDefUserName = "admin";
	m_sDefPassword = "";
	m_iDefPort = 34567;
	m_iMaxChannel = 0;
	m_lSearchDeviceHandle = -1;
}

CDZPVendor::~CDZPVendor()
{
	H264_DVR_Cleanup();
}

void CDZPVendor::Init()
{
	int iRet = iRet = H264_DVR_Init(nullptr, 0);
	if (!iRet)
	{
		std::string m_sLastError = GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
	}

	H264_DVR_SetConnectTime(5000, 3);

	std::cout << "DZP ��ʼ��SDK �ɹ���" << std::endl;
}

long CDZPVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	H264_DVR_DEVICEINFO OutDev;
	memset(&OutDev, 0, sizeof(H264_DVR_DEVICEINFO));
	int nError = 0;

	H264_DVR_SetConnectTime(3000, 1);

	long loginHandle = -1;
	loginHandle = H264_DVR_Login((char *)ip.c_str(), port, (char*)user.c_str(), (char*)password.c_str(), &OutDev, &nError, TCPSOCKET);
	if (loginHandle <= 0)
	{
		std::string m_sLastError = GZLL_GetLastErrorString(nError);
		throw LoginException(m_sLastError.c_str());
		std::cout << "DZP ��½ ʧ�ܣ�" << std::endl;
		return -1;
	}

	m_iMaxChannel = OutDev.byChanNum + OutDev.iDigChannel;

	std::cout << "DZP ��½ �ɹ���" << std::endl;

	return loginHandle;
}

void CDZPVendor::Logout(const long loginHandle)
{
	if (loginHandle > 0)
	{
		if (H264_DVR_Logout(loginHandle))
		{
			std::cout << "DZP �˳���½ �ɹ���" << std::endl;
		}
		else
		{
			std::string m_sLastError = GZLL_GetLastErrorString();
			std::cout << "DZP �˳���½ ʧ�ܣ�" << std::endl;
			throw std::exception(m_sLastError.c_str());
		}
	}
}

void CDZPVendor::StartSearchDevice()
{
	std::cout << "DZP �����豸 ��ʼ��" << std::endl;

	m_listDeviceInfo.clear();

	SDK_CONFIG_NET_COMMON_V2 Device[256] = { 0 };
	int nRetLength = 0;

	BOOL bRet = H264_DVR_SearchDevice((char*)Device, sizeof(SDK_CONFIG_NET_COMMON_V2)* 256, &nRetLength, 3000);
	if (bRet)
	{
		int i;
		for (i = 0; i < nRetLength / sizeof(SDK_CONFIG_NET_COMMON_V2); i++)
		{
			NET_DEVICE_INFO* ndiInfo = new NET_DEVICE_INFO();
			int nLen = 0;
			ndiInfo->nSDKType = DZP_SDK;

			struct in_addr in1 = { 0 };
			in1.s_addr = Device[i].HostIP.l;
			nLen = (strlen(inet_ntoa(in1)) < MAX_IPADDR_LEN) ? strlen(inet_ntoa(in1)) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo->szIp, inet_ntoa(in1), nLen);


			ndiInfo->nPort = Device[i].TCPPort;

			struct in_addr in2 = { 0 };
			in2.s_addr = Device[i].Submask.l;
			nLen = (strlen(inet_ntoa(in2)) < MAX_IPADDR_LEN) ? strlen(inet_ntoa(in2)) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo->szSubmask, inet_ntoa(in2), nLen);

			nLen = (strlen(Device[i].sMac) < MAX_MACADDR_LEN) ? strlen(Device[i].sMac) : MAX_MACADDR_LEN;
			memcpy(&ndiInfo->szMac, &Device[i].sMac, nLen);

			ndiInfo->pVendor = this;

			m_listDeviceInfo.push_back(ndiInfo);
		}
	}
}

void CDZPVendor::StopSearchDevice()
{
	std::cout << "DZP �����豸 ������" << std::endl;
}

void CDZPVendor::SearchAll(const long loginHandle)
{

}

void CDZPVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	assert(range.end - range.start <= 24 * 3600);
	if (range.start > range.end)
	{
		throw std::exception("Time Range Error!");
		return;
	}

	std::cout << "DZP �����ļ� ��ʼ��" << std::endl;	
	m_files_Unit.clear();
	DZP_SDK_INTERFACE::DZP_SearchUnit(loginHandle, channel, range, m_files_Unit);
	std::cout << "DZP �����ļ� ������" << std::endl;

	// Save Search Video List Result to Config File
	//std::cout << "DZP дJson���ݵ��ļ� ��ʼ��" << std::endl;
	//CCommonUtrl::getInstance().SaveSearchFileListToFile(m_files_Unit, Vendor_DZP_Abbr);
	//std::cout << "DZP дJson���ݵ��ļ� ������" << std::endl;

	// Write File List to DB
	std::cout << "DZP д�ļ����ݵ����ݿ� ��ʼ��" << std::endl;
	CCommonUtrl::getInstance().WriteFileListToDB(m_files_Unit);
	std::cout << "DZP д�ļ����ݵ����ݿ� ������" << std::endl;

	for (size_t i = 0; i < m_files_Unit.size(); i++)
	{
		m_files.push_back(m_files_Unit[i]);
	}
	
	return;
}

void CDZPVendor::ClearLocalRecordFiles()
{
	m_files.clear();
}

void CDZPVendor::Download(const long loginHandle, const size_t channel, const RecordFile& file)
{
	H264_DVR_FINDINFO info;

	info.nChannelN0 = channel;
	info.nFileType = SDK_RECORD_ALL;

	struct tm Tm;
	_localtime64_s(&Tm, (const time_t*)&file.beginTime);
	memset(&info, 0, sizeof(info));
	TMToNetTime(Tm, info.startTime);
	_localtime64_s(&Tm, (const time_t*)&file.endTime);
	TMToNetTime(Tm, info.endTime);

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

	//std::string strFileName = DZP_SDK_INTERFACE::DZP_MakeFileName(channel, strTimeStart);
	std::string strFileName = CCommonUtrl::getInstance().MakeFileName(channel, strTimeStart, strTimeEnd, ".h264");

	// Init File Save Path 
	std::string strPath = CCommonUtrl::getInstance().MakeDownloadFileFolder(m_sRoot, strTimeStartZero, strTimeEndZero, Vendor_DZP, channel);

	//DZP_SDK_INTERFACE::m_lFileHandle = H264_DVR_GetFileByTime(loginHandle, &info, (char *)strPath.c_str(), true, DZP_SDK_INTERFACE::DZP_DownLoadPosCallBack, 0);
	//if (DZP_SDK_INTERFACE::m_lFileHandle <= 0)
	//{
	//	std::string m_sLastError = std::string("����ʧ�ܣ�����Ϊ:") + GZLL_GetLastErrorString();
	//	return;
	//}

	strPath.append(strFileName);
	H264_DVR_FILE_DATA pData = DZP_SDK_INTERFACE::DZP_MakeRecordFileToH264Data(file);
	DZP_SDK_INTERFACE::m_lFileHandle = H264_DVR_GetFileByName(loginHandle, &pData, (char*)strPath.c_str(), DZP_SDK_INTERFACE::DZP_DownLoadPosCallBack, 0);
	if (DZP_SDK_INTERFACE::m_lFileHandle <= 0)
	{
		std::string m_sLastError = std::string("����ʧ�ܣ�����Ϊ:") + GZLL_GetLastErrorString();
		return;
	}
}

void CDZPVendor::PlayVideo(const long loginHandle, const size_t channel, const RecordFile& file)
{
	H264_DVR_FILE_DATA fileData = DZP_SDK_INTERFACE::DZP_MakeRecordFileToH264Data(file);
	fileData.hWnd = m_hWnd;
	int playbackHandle = H264_DVR_PlayBackByName(loginHandle, &fileData, nullptr, NULL, 0);
	if (playbackHandle == 0)
	{
		H264_DVR_StopPlayBack(playbackHandle);
		std::string m_sLastError = GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
	}

	return;
}

void CDZPVendor::SetDownloadPath(const std::string& Root)
{
	m_sRoot = Root;
}

void CDZPVendor::throwException()
{

}
/****************************************************************************************/
//std::string DZP_SDK_INTERFACE::DZP_MakeFileName(int channel, const std::string& startTime)
//{
//	std::string strFileName;
//	std::string strStartTime = startTime.c_str();
//
//	//channel += 1;
//
//	//if (channel < 10)
//	//{
//	//	strFileName += "0";
//	//}
//	strFileName += std::to_string(channel);
//	strFileName += "-";
//	strFileName += strStartTime.substr(0, 8).data();
//	strFileName += "-";
//	strFileName += strStartTime.substr(8, strStartTime.length() - 8).data();
//	strFileName.append(".h264");
//
//	return strFileName;
//}

void DZP_SDK_INTERFACE::DZP_SearchUnit(const long loginHandle, const size_t channel, const time_range& range, RECORD_FILE_LIST& recordFiles)
{
	H264_DVR_FINDINFO info;
	memset(&info, 0, sizeof(info));
	info.nChannelN0 = channel;
	info.nFileType = SDK_RECORD_ALL;
	tm STime;
	tm ETime;
	_localtime64_s(&STime, (const time_t*)&range.start);
	_localtime64_s(&ETime, (const time_t*)&range.end);
	TMToNetTime(STime, info.startTime);
	TMToNetTime(ETime, info.endTime);

	H264_DVR_FILE_DATA nriFileinfo[2000];
	int fileCount;
	long hdl = H264_DVR_FindFile(loginHandle, &info, nriFileinfo, sizeof(nriFileinfo) / sizeof(H264_DVR_FILE_DATA), &fileCount);
	if (hdl <= 0)
	{
		std::string m_sLastError = std::string("��ѯʧ�ܣ�����Ϊ:") + GZLL_GetLastErrorString();
		throw SearchFileException(m_sLastError.c_str());
		return;
	}
	else
	{
		if (fileCount > 0)
		{
			for (int i = 0; i < fileCount; i++)
			{
				if (nriFileinfo[i].size>0)//��Ƶ�ļ��������0(�������з����ļ���СΪ0���,���Ա�����˵��Ĳ���)
				{
					RecordFile record;

					struct tm Tm;
					NetTimeToTM(nriFileinfo[i].stBeginTime, Tm);
					record.beginTime = _mktime64(&Tm);

					NetTimeToTM(nriFileinfo[i].stEndTime, Tm);
					record.endTime = _mktime64(&Tm);

					// File Start Time
					SDK_SYSTEM_TIME h264_time = nriFileinfo[i].stBeginTime;
					std::string strStartTime;
					sprintf((char*)strStartTime.data(), "%d%02d%02d%02d%02d%02d", h264_time.year, h264_time.month, h264_time.day, h264_time.hour, h264_time.minute, h264_time.second);

					// File End Time
					h264_time = nriFileinfo[i].stEndTime;
					std::string strEndTime;
					sprintf((char *)strEndTime.data(), "%d%02d%02d%02d%02d%02d", h264_time.year, h264_time.month, h264_time.day, h264_time.hour, h264_time.minute, h264_time.second);

					// File Channel and so on
					record.channel = channel;
					record.name = nriFileinfo[i].sFileName;
					//record.name = DZP_MakeFileName(channel, strStartTime);

					record.size = nriFileinfo[i].size;
					record.setPrivateData(&nriFileinfo[i], sizeof(H264_DVR_FILE_DATA));

					recordFiles.push_back(record);
					record.deletePrivateData();
				}
			}
		}
	}
}

void __stdcall DZP_SDK_INTERFACE::DZP_DownLoadPosCallBack(long lPlayHandle, long lTotalSize, long lDownLoadSize, long dwUser)
{
	static int prePos = 0;
	int curPos = 0;

	curPos = DZP_GetDownloadPos(m_lFileHandle);

	if (curPos != prePos)
	{
		std::cout << lDownLoadSize << "/" << lTotalSize << " - " << curPos << std::endl;
		prePos = curPos;
	}
	
	if (lDownLoadSize == -1)
	{
		prePos = 0;
	}
}

int __stdcall DZP_SDK_INTERFACE::DZP_RealDataCallBack(long lRealHandle, long dwDataType, unsigned char *pBuffer, long lbufsize, long dwUser)
{
	return 0;
}

int DZP_SDK_INTERFACE::DZP_GetDownloadPos(const int fileHandle)
{
	int pos = H264_DVR_GetDownloadPos(fileHandle);

	return pos;
}

H264_DVR_FILE_DATA DZP_SDK_INTERFACE::DZP_MakeRecordFileToH264Data(const RecordFile& file)
{
	H264_DVR_FILE_DATA h264_file;

	h264_file.ch = file.channel;
	h264_file.size = file.size;
	strcpy(h264_file.sFileName, file.name.c_str());
	//memcpy(h264_file.sFileName, file.name.c_str(), file.name.length());
	//file.name.copy(h264_file.sFileName, file.name.length(), 0);

	struct tm *ttime;
	H264_DVR_TIME h264_time;

	ttime = gmtime(&file.beginTime);
	TMToNetTime(*ttime, h264_time);
	h264_file.stBeginTime = DZP_MakeH264TimeToSDKTime(h264_time);

	ttime = gmtime(&file.endTime);
	TMToNetTime(*ttime, h264_time);
	h264_file.stEndTime = DZP_MakeH264TimeToSDKTime(h264_time);

	return h264_file;
	
}

SDK_SYSTEM_TIME DZP_SDK_INTERFACE::DZP_MakeH264TimeToSDKTime(H264_DVR_TIME h264_time)
{
	SDK_SYSTEM_TIME time;

	time.year = h264_time.dwYear;
	time.month = h264_time.dwMonth;
	time.day = h264_time.dwDay;
	time.hour = h264_time.dwHour;
	time.minute = h264_time.dwMinute;
	time.second = h264_time.dwSecond;

	return time;
}