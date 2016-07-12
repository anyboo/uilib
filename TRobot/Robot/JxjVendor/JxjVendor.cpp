
#include "JXJVendor.h"

#include "Notification.h"
#include "NotificationQueue.h"

// JXJ SDK
#include "mb_api.h"
#include "j_sdk.h"
#include "stdint.h"
#include "Jtype.h"
#include "JNetSDK.h"
#include "AVPlayer.h"

#pragma comment(lib, "JNetSDK")
#pragma comment(lib, "AVPlayer")

static eErrCode m_errCode = Err_No; // Error Code
JStoreLog m_storeLog;
JRecodeType m_recordType = ALL_RECODE;
int	m_iBeginNode = 0;
int	m_iEndNode = J_SDK_MAX_STORE_LOG_SIZE - 1;
int	m_iSsid = -1;

long m_lDownloadHandle = -1;
long m_lDownloadFileHandle = -1;
long m_lRecHandle = -1;
long m_lDownLoadStartTime = -1;
long m_lDownLoadTotalTime = -1;
//int m_iDownLoadPos = 0;
int m_iPlayVideoChannel = -1;

static bool JXJ_Init_Flag = false;


// Login Callback
int __stdcall JXJ_ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam);
// Search Device Callback
int __stdcall JXJ_JMBNotify(long lHandle, DWORD dwPortocol, int iErr, int iMsgID, LPCTSTR lpszDstID, void* pData, int iDataLen, void* pUserParam);
// Search Callback
void JXJ_MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, const int beginNode, const int endNode, const int ssid, const std::time_t& start, const std::time_t& end);
std::string JXJ_MakeFileName(int channel, const std::string& startTime, const std::string& endTime);
void JXJ_SearchUnit(const long loginHandle, const size_t channel, const time_range& range, RECORD_FILE_LIST& recordFiles);
void JXJ_ReFreshVideoList(const long loginHandle, int channel, const time_range& range, RECORD_FILE_LIST& recordFiles);
void JXJ_AddSearchFileList(int channel, RECORD_FILE_LIST& recordFiles);
time_t JXJ_MakeTimestampByJTime(JTime jTime);
bool JXJ_CheckFileExist(const RecordFile& file, const std::vector<RecordFile>& fileList);

// Download Callback
int  __stdcall JXJ_JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
static void JXJ_CloseDownload();

// PlayVideo Callback
int  __stdcall JXJ_JRecStream(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam);
DWORD JXJ_PlayThreadFun(LPVOID lpThreadParameter);

typedef enum
{
	IsPlay_Download = 0,
	IsPlay_Play
}eIsPlay;

typedef enum
{
	Encoder_Unknow = -1,
	Encoder_DM365 = 0,
	Encoder_DM368 = 1,
	Encoder_DM8127 = 2,
	Encoder_DM8168 = 3,
	Encoder_HI = 0x100 - 1,
	Encoder_3507 = 0x100,
	Encoder_3510,
	Encoder_3511,
	Encoder_3512,
	Encoder_3515,
	Encoder_3516,
	Encoder_3517,
	Encoder_3518,
	Encoder_3520,
	Encoder_3521,
	Encoder_3531,
	Encoder_3532,
	Encoder_3531_3532,
}eEncoderType;

const char* GetErrorString(int error)
{
	switch (error)
	{
	case JNETErrSuccess: return "�ɹ�";
	case JNETErrUnInit: return "δ��ʼ��";
	case JNETErrHandle: return "���������";
	case JNETErrParam: return "��������";
	case JNETErrBuffSize: return "������";
	case JNETErrNoMem: return "�ڴ治��";
	case JNETErrRecv: return "���մ���";
	case JNETErrSend: return "���ʹ���";
	case JNETErrOperate: return "��������";
	case JNETErrURL: return "URL����";
	case JNETErrLogining: return "�û����ڵ�¼";
	case JNETErrLogout: return "�Ѿ��ǳ�";
	case JNETErrNoFreePort: return "û�п���ͨ��";
	case JNETErrProtocol: return "Э�����";
	case JNETErrXMLFormat: return "�����XML����";
	case JNETErrNotSupport: return "��֧�ֵĲ���";
	case JNETErrGetParam: return "��ȡ��������";
	case JNETErrSetParam: return "���ò�������";
	case JNETErrOpenFile: return "���ļ�����";
	case JNETErrUpgOpen: return "��������";

	}

	return "δ����";
}

CJXJVendor::CJXJVendor()
{
	// Init Param
	m_eSDKType = JXJ_SDK;
	m_sDefUserName = "admin";
	m_sDefPassword = "admin";
	m_iMaxChannel = 0;

	m_lSearchDeviceHandle = -1;
}

CJXJVendor::~CJXJVendor()
{
	if (JXJ_Init_Flag)
	{
		assert(!JNetCleanup());
	}
}

void CJXJVendor::Init()
{
	// Init JNetSdk
	int iRet = JNetInit(NULL);
	if (0 != iRet)
	{
		std::string m_sLastError = GetErrorString(iRet);
		throw std::exception(m_sLastError.c_str());
	}

	// Init AVP_Init
	iRet = AVP_Init();
	if (iRet != AVPErrSuccess)
	{
		throw std::exception("AVP_Init Error!");
	}

	AVP_InitRecMng(128, 8);

	JXJ_Init_Flag = true;

	std::cout << "JXJ ��ʼ��SDK �ɹ���" << std::endl;
}

long CJXJVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	m_errCode = Err_No;

	long loginHandle = -1;
	int iRet = JNetLogin(ip.c_str(), port, user.c_str(), password.c_str(), 10, JXJ_ConnEventCB, NULL, JNET_PRO_T_JPF, loginHandle, NULL);
	if (iRet < 0)
	{
		std::string m_sLastError = GetErrorString(iRet);
		return -1;
	}

	while (m_errCode == Err_No)
	{
		::Sleep(100);
	}

	if (m_errCode == Err_LoginFail)
	{
		std::cout << "JXJ ��½ ʧ�ܣ�" << std::endl;
		return -1;
	}

	iRet = -1;
	JDeviceInfo deviceinfo;
	iRet = JNetGetParam(loginHandle, 0, PARAM_DEVICE_INFO, (char *)&deviceinfo, sizeof(deviceinfo), NULL, NULL);
	if (iRet != 0)
	{
		throw std::exception("��ȡ�豸��Ϣʧ��");
	}

	m_iMaxChannel = deviceinfo.channel_num;

	std::cout << "JXJ ��½ �ɹ���" << std::endl;

	return loginHandle;
}

void CJXJVendor::Logout(const long loginHandle)
{
	if (loginHandle > 0)
	{
		int iRet = JNetMBClose(loginHandle);
		if (JNETErrSuccess != iRet)
		{
			std::string m_sLastError = GetErrorString(iRet);
			std::cout << "JXJ �˳���½ ʧ�ܣ�" << std::endl;
			throw std::exception(m_sLastError.c_str());
		}
		else
		{
			std::cout << "JXJ �˳���½ �ɹ���" << std::endl;
		}
	}
}

void CJXJVendor::StartSearchDevice()
{
	m_listDeviceInfo.clear();

	char szIp[11] = "224.0.0.99";
	int nPort = 40086;
	long lResult = JNetMBOpen(szIp, nPort, JXJ_JMBNotify, this, JNET_PRO_T_JPF, m_lSearchDeviceHandle);
	lResult = JNetMBSearch(m_lSearchDeviceHandle, 5);

	std::cout << "JXJ �����豸 ��ʼ��" << std::endl;
}
void CJXJVendor::StopSearchDevice()
{
	JNetMBClose(m_lSearchDeviceHandle);

	std::cout << "JXJ �����豸 ������" << std::endl;
}

void CJXJVendor::SearchAll(const long loginHandle)
{

}

void CJXJVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	std::cout << "JXJ �����ļ� ��ʼ��" << std::endl;

	m_files.clear();

	if (range.start > range.end)
	{
		throw std::exception("Time Range Error!");
		return;
	}

	std::vector<time_range> timeRangeList = CCommonUtrl::getInstance().MakeTimeRangeList(range);
	for (size_t i = 0; i < timeRangeList.size(); i++)
	{
		JXJ_SearchUnit(loginHandle, channel, timeRangeList[i], m_files);
	}

	std::cout << "JXJ �����ļ� ������" << std::endl;

	// Save Search Video List Result to Config File
	std::cout << "JXJ дJson���ݵ��ļ� ��ʼ��" << std::endl;
	CCommonUtrl::getInstance().SaveSearchFileListToFile(m_files, Vendor_JXJ_Abbr);
	std::cout << "JXJ дJson���ݵ��ļ� ������" << std::endl;

	// Write File List to DB
	std::cout << "JXJ д�ļ����ݵ����ݿ� ��ʼ��" << std::endl;
	CCommonUtrl::getInstance().WriteFileListToDB(m_files);
	std::cout << "JXJ д�ļ����ݵ����ݿ� ������" << std::endl;

	return;
}

//void CJxjVendor::DownloadByTime(const std::time_t& start, const std::time_t& end)
void CJXJVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{
	// Init File Starttime and Endtime
	std::string strTimeStart;
	std::string strTimeEnd;
	std::string strTimeStartZero;
	std::string strTimeEndZero;

	struct tm ttime;

	localtime_s(&ttime, &range.start);
	strftime((char *)strTimeStart.data(), 24, "%Y%m%d%H%M%S", &ttime);
	strftime((char *)strTimeStartZero.data(), 24, "%Y%m%d0000", &ttime);
	strftime((char *)strTimeEndZero.data(), 24, "%Y%m%d2359", &ttime);
	localtime_s(&ttime, &range.end);
	strftime((char *)strTimeEnd.data(), 24, "%Y%m%d%H%M%S", &ttime);

	std::string strFileName = JXJ_MakeFileName(channel, strTimeStart, strTimeEnd);

	if (m_files.size() == 0)
	{
		throw std::exception("Search File List Empty!");
		return;
	}

	RecordFile file;
	size_t i = 0;
	for (; i < m_files.size(); i++)
	{
		file = m_files[i];
		if (strFileName.compare(file.name) == 0)
		{
			break;
		}
	}

	if (i >= m_files.size())
	{
		throw std::exception("Search File List Not Contain the Range!");
		return;
	}

	// Init File Save Path 
	std::string strPath = CCommonUtrl::getInstance().MakeDownloadFileFolder(m_sRoot, strTimeStartZero, strTimeEndZero, Vendor_JXJ, channel);
	strPath += file.name.data();

	// Set File Total Time
	m_lDownLoadTotalTime = file.duration;
	m_lDownloadHandle = JNetRecOpen4Time(loginHandle, "", channel, j_primary_stream, strTimeStart.c_str(), strTimeEnd.c_str(), 4096, IsPlay_Download, JXJ_JRecDownload, this, m_lRecHandle);
	if (m_lDownloadHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);

		m_lDownloadFileHandle = AVP_CreateRecFile(strPath.c_str(), AVP_PROTOCOL_JPF, Encoder_DM365);
		if (m_lDownloadFileHandle == -1)
		{
			throw std::exception("Create File Error!");
		}
	}
	else
	{
		throw std::exception("Download File Error!");
	}
}

//void CJxjVendor::DownloadByName(const std::string& filename)
void CJXJVendor::Download(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (m_files.size() == 0)
	{
		throw std::exception("Search File List Empty!");
		return;
	}

	RecordFile file;
	size_t i = 0;
	for (; i < m_files.size(); i++)
	{
		file = m_files[i];
		if (filename.compare(file.name) == 0)
		{
			break;
		}
	}

	if (i >= m_files.size())
	{
		throw std::exception("Search File List Not Contain the Filename!");
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

	std::string strFileName = JXJ_MakeFileName(channel, strTimeStart, strTimeEnd);

	// Init File Save Path 
	std::string strPath = CCommonUtrl::getInstance().MakeDownloadFileFolder(m_sRoot, strTimeStartZero, strTimeEndZero, Vendor_JXJ, channel);
	strPath += file.name.data();

	// Set File Total Time
	m_lDownLoadTotalTime = file.duration;
	m_lDownloadHandle = JNetRecOpen4Time(loginHandle, "", channel, j_primary_stream, strTimeStart.c_str(), strTimeEnd.c_str(), 4096, IsPlay_Download, JXJ_JRecDownload, this, m_lRecHandle);
	if (m_lDownloadHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);

		m_lDownloadFileHandle = AVP_CreateRecFile(strPath.c_str(), AVP_PROTOCOL_JPF, Encoder_DM365);
		if (m_lDownloadFileHandle == -1)
		{
			throw std::exception("Create File Error!");
		}
	}
	else
	{
		throw std::exception("Download File Error!");
	}
}

//void CJxjVendor::PlayVideo(const std::string& filename)
void CJXJVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{
	// Init File Starttime and Endtime
	std::string strTimeStart;
	std::string strTimeEnd;

	struct tm ttime;

	localtime_s(&ttime, &range.start);
	strftime((char *)strTimeStart.c_str(), 24, "%Y%m%d%H%M%S", &ttime);
	localtime_s(&ttime, &range.end);
	strftime((char *)strTimeEnd.c_str(), 24, "%Y%m%d%H%M%S", &ttime);

	std::string strFileName = JXJ_MakeFileName(channel, strTimeStart, strTimeEnd);

	if (m_files.size() == 0)
	{
		throw std::exception("Search File List Empty!");
		return;
	}

	RecordFile file;
	size_t i = 0;
	for (; i < m_files.size(); i++)
	{
		file = m_files[i];
		if (strFileName.compare(file.name) == 0)
		{
			break;
		}
	}

	if (i >= m_files.size())
	{
		throw std::exception("Search File List Not Contain the Range!");
		return;
	}

	m_iPlayVideoChannel = AVP_GetFreePort();

	m_lRecHandle = JNetRecOpen4Time(loginHandle, "", channel, 0, strTimeStart.c_str(), strTimeEnd.c_str(), 4096, IsPlay_Play, JXJ_JRecStream, this, m_lRecHandle);
	if (m_lRecHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);
	}

	int iRet = -1;
	AVP_SetPlayPriority(m_iPlayVideoChannel, AVPPlaySmooth);
	AVP_SetDataProtocol(m_iPlayVideoChannel, AVP_PROTOCOL_JPF);

	//AVP_EnableYUVDraw(gChannel,TRUE);
	// ����ͨ��ʹ�õĽ�����
	iRet = AVP_SetCoder(m_iPlayVideoChannel, AVP_CODER_JXJ);
	if (iRet != AVPErrSuccess)
	{
		throw std::exception("AVP_SetCoder Error!");
		return;
	}

	AVP_AddPlayWnd(m_iPlayVideoChannel, NULL, NULL, NULL, NULL);

	// ��������
	AVP_Play(m_iPlayVideoChannel);
}
void CJXJVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (m_files.size() == 0)
	{
		throw std::exception("Search File List Empty!");
		return;
	}

	RecordFile file;
	size_t i = 0;
	for (; i < m_files.size(); i++)
	{
		file = m_files[i];
		if (filename.compare(file.name) == 0)
		{
			break;
		}
	}

	if (i >= m_files.size())
	{
		throw std::exception("Search File List Not Contain the Filename!");
		return;
	}

	// Init File Starttime and Endtime
	std::string strTimeStart;
	std::string strTimeEnd;

	struct tm ttime;

	localtime_s(&ttime, &file.beginTime);
	strftime((char *)strTimeStart.c_str(), 24, "%Y%m%d%H%M%S", &ttime);
	localtime_s(&ttime, &file.endTime);
	strftime((char *)strTimeEnd.c_str(), 24, "%Y%m%d%H%M%S", &ttime);

	m_iPlayVideoChannel = AVP_GetFreePort();

	m_lRecHandle = JNetRecOpen4Time(loginHandle, "", channel, 0, strTimeStart.c_str(), strTimeEnd.c_str(), 4096, IsPlay_Play, JXJ_JRecStream, this, m_lRecHandle);
	if (m_lRecHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);
	}

	int iRet = -1;
	AVP_SetPlayPriority(m_iPlayVideoChannel, AVPPlaySmooth);
	AVP_SetDataProtocol(m_iPlayVideoChannel, AVP_PROTOCOL_JPF);

	//AVP_EnableYUVDraw(gChannel,TRUE);
	// ����ͨ��ʹ�õĽ�����
	iRet = AVP_SetCoder(m_iPlayVideoChannel, AVP_CODER_JXJ);
	if (iRet != AVPErrSuccess)
	{
		throw std::exception("AVP_SetCoder Error!");
		return;
	}

	AVP_AddPlayWnd(m_iPlayVideoChannel, NULL, NULL, NULL, NULL);

	// ��������
	AVP_Play(m_iPlayVideoChannel);

	return;
}
void CJXJVendor::SetDownloadPath(const std::string& Root)
{
	m_sRoot = Root;
}

void CJXJVendor::throwException()
{
}

/***********************************************************************************************/
int __stdcall JXJ_ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam)
{
	switch (eType)
	{
	case JNET_EET_LOGINING:		 //���ڵ�½
	{
	}
		break;
	case JNET_EET_LOGIN_OK:		 //��¼�ɹ�
	{
									 m_errCode = Err_LoginSuccess;
	}
		break;
	case JNET_EET_LOGIN_ERROR:   //��¼ʧ��
	{
									 m_errCode = Err_LoginFail;
	}
		break;
	}

	return 0;
}

int __stdcall JXJ_JMBNotify(long lHandle, DWORD dwPortocol, int iErr, int iMsgID, LPCTSTR lpszDstID, void* pData, int iDataLen, void* pUserParam)
{
	CJXJVendor* jxjVendor = (CJXJVendor *)pUserParam;

	NET_DEVICE_INFO* ndiInfo = new NET_DEVICE_INFO();

	assert(pData);
	assert(iDataLen);

	if (NULL == pData || iDataLen == 0)
	{
		return 0;
	}

	ndiInfo->nSDKType = JXJ_SDK;

	j_Device_T jdtTmp = { 0 };

	j_Device_T *pDev = (j_Device_T *)(pData);
	int copy_size = 0;

	if (iDataLen != sizeof(j_Device_T))
	{
		copy_size = sizeof(J_SysCfg_T);
		if (pDev->SysSize < sizeof(J_SysCfg_T))
		{
			copy_size = pDev->SysSize;

		}
		jdtTmp.SysSize = sizeof(J_SysCfg_T);
		memcpy(&jdtTmp.SysCfg, &pDev->SysCfg, copy_size);

		copy_size = sizeof(JNetworkInfo);
		if (pDev->NetSize < sizeof(JNetworkInfo))
		{
			copy_size = pDev->NetSize;

		}
		jdtTmp.NetSize = sizeof(JNetworkInfo);
		memcpy(&jdtTmp.NetworkInfo, &pDev->NetworkInfo, copy_size);
	}
	else
	{
		memcpy(&jdtTmp, pData, sizeof(j_Device_T));
	}

	int nLen = (strlen((char *)jdtTmp.NetworkInfo.network[0].ip) < MAX_IPADDR_LEN) ? strlen((char *)jdtTmp.NetworkInfo.network[0].ip) : MAX_IPADDR_LEN;
	memcpy(&ndiInfo->szIp, &jdtTmp.NetworkInfo.network[0].ip, nLen);

	nLen = (strlen((char *)jdtTmp.NetworkInfo.network[0].netmask) < MAX_IPADDR_LEN) ? strlen((char *)jdtTmp.NetworkInfo.network[0].netmask) : MAX_IPADDR_LEN;
	memcpy(&ndiInfo->szSubmask, &jdtTmp.NetworkInfo.network[0].netmask, nLen);

	nLen = (strlen((char *)jdtTmp.NetworkInfo.network[0].mac) < MAX_MACADDR_LEN) ? strlen((char *)jdtTmp.NetworkInfo.network[0].mac) : MAX_MACADDR_LEN;
	memcpy(&ndiInfo->szMac, &jdtTmp.NetworkInfo.network[0].mac, nLen);
	ndiInfo->nPort = jdtTmp.NetworkInfo.cmd_port;

	ndiInfo->pVendor = jxjVendor;

	jxjVendor->GetDeviceInfoList().push_back(ndiInfo);
}

void JXJ_MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, const int beginNode, const int endNode, const int ssid, const std::time_t& start, const std::time_t& end)
{
	//��ѯ¼�����
	ZeroMemory(&storeLog, sizeof(JStoreLog));
	storeLog.rec_type = recordType;
	storeLog.beg_node = beginNode;
	storeLog.end_node = endNode;
	storeLog.sess_id = ssid;

	struct tm ttimeStart;
	struct tm ttimeEnd;
	localtime_s(&ttimeStart, &start);
	localtime_s(&ttimeEnd, &end);

	storeLog.beg_time.year = (uint8_t)ttimeStart.tm_year;
	storeLog.beg_time.month = (uint8_t)(ttimeStart.tm_mon + 1);
	storeLog.beg_time.date = (uint8_t)ttimeStart.tm_mday;
	storeLog.beg_time.weekday = (uint8_t)(ttimeStart.tm_wday + 1);
	storeLog.beg_time.hour = (uint8_t)ttimeStart.tm_hour;
	storeLog.beg_time.minute = (uint8_t)ttimeStart.tm_min;
	storeLog.beg_time.second = (uint8_t)ttimeStart.tm_sec;

	storeLog.end_time.year = (uint8_t)ttimeEnd.tm_year;
	storeLog.end_time.month = (uint8_t)(ttimeEnd.tm_mon + 1);
	storeLog.end_time.date = (uint8_t)ttimeEnd.tm_mday;
	storeLog.end_time.weekday = (uint8_t)(ttimeEnd.tm_wday + 1);
	storeLog.end_time.hour = (uint8_t)ttimeEnd.tm_hour;
	storeLog.end_time.minute = (uint8_t)ttimeEnd.tm_min;
	storeLog.end_time.second = (uint8_t)ttimeEnd.tm_sec;
}

std::string JXJ_MakeFileName(int channel, const std::string& startTime, const std::string& endTime)
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
	strFileName.append(".jav");

	return strFileName;
}

void JXJ_SearchUnit(const long loginHandle, const size_t channel, const time_range& range, RECORD_FILE_LIST& recordFiles)
{
	
	JXJ_MakeStoreLog(m_storeLog, ALL_RECODE, m_iBeginNode, m_iEndNode, m_iSsid, range.start, range.end);

	// Get Search File Info
	int iRet = -1;
	iRet = JNetGetParam(loginHandle, channel, PARAM_STORE_LOG, (char *)&m_storeLog, sizeof(m_storeLog), NULL, NULL);
	if (iRet != 0)
	{
		throw std::exception("Search Error!");
	}

	if (m_storeLog.total_count > 0)
	{
		JXJ_ReFreshVideoList(loginHandle, channel, range, recordFiles);
	}
}

void JXJ_ReFreshVideoList(const long loginHandle, int channel, const time_range& range, RECORD_FILE_LIST& recordFiles)
{
	JXJ_AddSearchFileList(channel, recordFiles);

	if (m_iEndNode < (int)m_storeLog.total_count) //ÿ�����ֻ��ȡ24�� ������ȡ
	{
		m_iBeginNode += 24;
		m_iEndNode += 24;
		m_iSsid = m_storeLog.sess_id;
		JXJ_SearchUnit(loginHandle, channel, range, recordFiles);
	}
	else
	{
		m_iBeginNode = 0;
		m_iEndNode = J_SDK_MAX_STORE_LOG_SIZE - 1;
		m_iSsid = -1;
	}
}

void JXJ_AddSearchFileList(int channel, RECORD_FILE_LIST& recordFiles)
{
	int j = m_iBeginNode;

	for (int i = 0; i < (int)m_storeLog.node_count; i++)
	{

		RecordFile recordFile;

		// File Start Time
		JTime jTime = m_storeLog.store[i].beg_time;
		recordFile.beginTime = JXJ_MakeTimestampByJTime(jTime);
		std::string strStartTime;
		sprintf((char*)strStartTime.c_str(), "%d%02d%02d%02d%02d%02d", jTime.year + 1900, jTime.month, jTime.date, jTime.hour, jTime.minute, jTime.second);

		// File End Time
		jTime = m_storeLog.store[i].end_time;
		recordFile.endTime = JXJ_MakeTimestampByJTime(jTime);
		std::string strEndTime;
		sprintf((char *)strEndTime.data(), "%d%02d%02d%02d%02d%02d", jTime.year + 1900, jTime.month, jTime.date, jTime.hour, jTime.minute, jTime.second);

		// File Belong Time Secton 
		recordFile.strTimeSection = strStartTime.data();
		recordFile.strTimeSection.append("-");
		recordFile.strTimeSection += strEndTime.data();

		// File Duration
		recordFile.duration = recordFile.endTime - recordFile.beginTime;

		// File Channel and File Size
		recordFile.channel = channel;
		//recordFile.size = m_storeLog.store[i].file_size / (1024 * 1024); // MB
		recordFile.size = m_storeLog.store[i].file_size; // Byte

		// File Name
		std::string fileName = JXJ_MakeFileName(recordFile.channel, strStartTime, strEndTime);
		recordFile.name = fileName;

		if (!JXJ_CheckFileExist(recordFile, recordFiles))
		{
			recordFiles.push_back(recordFile);
		}

		j++;
	}
}
time_t JXJ_MakeTimestampByJTime(JTime jTime)
{
	struct tm ttime;
	ttime.tm_year = jTime.year;
	ttime.tm_mon = jTime.month - 1;
	ttime.tm_mday = jTime.date;
	ttime.tm_hour = jTime.hour;
	ttime.tm_min = jTime.minute;
	ttime.tm_sec = jTime.second;
	time_t time = mktime(&ttime);

	return time;
}

bool JXJ_CheckFileExist(const RecordFile& file, const std::vector<RecordFile>& fileList)
{
	for (size_t i = 0; i < fileList.size(); i++)
	{
		RecordFile fileItem = fileList[i];
		if (file.name.compare(fileItem.name) == 0
			&& file.size == fileItem.size
			&& file.beginTime == fileItem.beginTime
			&& file.endTime == fileItem.endTime)
		{
			return true;
		}
	}

	return false;
}

int __stdcall JXJ_JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam)
{
	static int prePos = 0;
	int curPos = 0;

	if (pBuff)
	{
		j_frame_t *pFrame = (j_frame_t *)pBuff;
		if (m_lDownLoadStartTime == -1 && pFrame->frame_type != j_audio_frame)
		{
			m_lDownLoadStartTime = pFrame->timestamp_sec;
		}

		AVP_WriteRecFile(m_lDownloadFileHandle, pBuff, dwRevLen, NULL, 0);

		if (m_lDownLoadTotalTime == 0)
		{
			return 0;
		}

		if (pFrame->timestamp_sec > 0 && m_lDownLoadTotalTime > 0)
		{
			curPos = (pFrame->timestamp_sec - m_lDownLoadStartTime) * 100 / m_lDownLoadTotalTime;
			if (curPos != prePos)
			{
				prePos = curPos;

				char cDownInfo[100];
				sprintf_s(cDownInfo, 100, "m_idownloadpos = %d , starttime = %ld, curtime = %ld, totaltime = %ld\r\n", curPos, m_lDownLoadStartTime, pFrame->timestamp_sec, m_lDownLoadTotalTime);
				std::cout << cDownInfo << std::endl;

				NotificationQueue& queue = CNotificationQueue::getInstance().GetQueue();
				queue.enqueueNotification(new CNotification(Notification_Type_Download_Start));
			}
		}

		if ((m_lDownLoadStartTime + m_lDownLoadTotalTime) == pFrame->timestamp_sec)
		{
			m_errCode = Err_DownloadSuccess;

			m_lDownLoadStartTime = -1;
			m_lDownLoadTotalTime = -1;

			curPos = 100;
			prePos = 0;

			char cDownInfo[100];
			sprintf_s(cDownInfo, 100, "m_idownloadpos = %d , starttime = %ld, curtime = %ld, totaltime = %ld\r\n", curPos, m_lDownLoadStartTime, pFrame->timestamp_sec, m_lDownLoadTotalTime);
			std::cout << cDownInfo << std::endl;

			NotificationQueue& queue = CNotificationQueue::getInstance().GetQueue();
			queue.enqueueNotification(new CNotification(Notification_Type_Download_End));

			JXJ_CloseDownload();
		}
	}
	return 0;
}

void JXJ_CloseDownload()
{
	if (m_lDownloadFileHandle)
	{
		AVP_CloseRecFile(m_lDownloadFileHandle);
		m_lDownloadFileHandle = -1;
	}

	if (m_lDownloadHandle)
	{
		JNetRecClose(m_lDownloadHandle);
		m_lDownloadHandle = -1;
	}
}

int __stdcall JXJ_JRecStream(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam)
{
	j_frame_t *pFrame = (j_frame_t *)pBuff;

	char cPlayInfo[100];
	sprintf_s(cPlayInfo, 100, "--------%d    %d\r\n", pFrame->timestamp_sec, pFrame->timestamp_usec);
	std::cout << cPlayInfo << std::endl;
	AVP_PutFrame(m_iPlayVideoChannel, pBuff);

	return 0;
}

DWORD JXJ_PlayThreadFun(LPVOID lpThreadParameter)
{
	return 0;
}
