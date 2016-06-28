#include "stdafx.h"
#include "JxjVendor.h"
#include "TestWindows.h"

#define Test_Bug
#define Test_Filename

#define oneDay		(24 * 60 * 60)
#define oneHour		(60 * 60)
#define oneMinute	(60)

eErrCode CJxjVendor::m_errCode = Err_No;

long CJxjVendor::m_lDownloadHandle = -1;
long CJxjVendor::m_lDownloadFileHandle = -1;
long CJxjVendor::m_lDownLoadStartTime = -1;
long CJxjVendor::m_lDownLoadTotalTime = -1;
int CJxjVendor::g_iDownLoadPos = 0;

int CJxjVendor::m_iPlayVideoChannel = -1;

time_t gTimeStart = 1466265600; // 2016-6-19 00:00:00 1466352000
time_t gTimeEnd = 1466697599; // 2016-6-23 23:59:59


const char* GetErrorString(int error)
{
	switch (error)
	{
	case JNETErrSuccess: return "成功";
	case JNETErrUnInit: return "未初始化";
	case JNETErrHandle: return "句柄不存在";
	case JNETErrParam: return "参数错误";
	case JNETErrBuffSize: return "缓存满";
	case JNETErrNoMem: return "内存不足";
	case JNETErrRecv: return "接收错误";
	case JNETErrSend: return "发送错误";
	case JNETErrOperate: return "操作错误";
	case JNETErrURL: return "URL有误";
	case JNETErrLogining: return "用户正在登录";
	case JNETErrLogout: return "已经登出";
	case JNETErrNoFreePort: return "没有空闲通道";
	case JNETErrProtocol: return "协议错误";
	case JNETErrXMLFormat: return "错误的XML数据";
	case JNETErrNotSupport: return "不支持的操作";
	case JNETErrGetParam: return "获取参数错误";
	case JNETErrSetParam: return "设置参数错误";
	case JNETErrOpenFile: return "打开文件出错";
	case JNETErrUpgOpen: return "升级出错";

	}

	return "未定义";
}

CJxjVendor::CJxjVendor()
{
	// Init Param
	m_strRoot = "";
	/* Login */
	m_lLoginHandle = -1;
	m_ip = "";
	m_port = 0;
	/* Search */
	m_recordType = TIMER_RECODE;
	m_iBeginNode = 0;
	m_iEndNode = J_SDK_MAX_STORE_LOG_SIZE - 1;
	m_iSsid = -1;
	/* Download */
	m_lRecHandle = -1;
	/* PlayVideo*/
}

CJxjVendor::~CJxjVendor()
{
	if (m_lLoginHandle)
	{
		int iRet = -1;
		iRet = JNetMBClose(m_lLoginHandle);
		if( 0 != iRet )
		{
			OutputDebugString("JNetMBClose Error!");
			throw std::exception("JNetMBClose Error!");
		}
	}
}

void CJxjVendor::Init(const std::string& ip, size_t port)
{
	// Init JNetSdk
	int iRet = JNetInit(NULL);
	if (0 != iRet)
	{
		m_sLastError = GetErrorString(iRet);
		throw std::exception(m_sLastError.c_str());
	}

	// Init AVP_Init
	iRet = AVP_Init();
	if (iRet != AVPErrSuccess)
	{
		OutputDebugString("AVP_Init Error!");
		throw std::exception("AVP_Init Error!");
	}

	AVP_InitRecMng(128, 8);

	m_ip = ip;
	m_port = port;
}

void CJxjVendor::Login(const std::string& user, const std::string& password)
{
	m_errCode = Err_No;

	int iRet = JNetLogin(m_ip.c_str(), m_port, user.c_str(), password.c_str(), 10, ConnEventCB, NULL, JNET_PRO_T_JPF, m_lLoginHandle, NULL);
	if (iRet < 0)
	{
		m_sLastError = GetErrorString(iRet);
		throw std::exception(m_sLastError.c_str());
	}

	while (m_errCode == Err_No)
	{
		::Sleep(100);
	}

	if (m_errCode == Err_LoginFail)
	{
		throw std::exception("登陆失败");
	}

	return;
}

void CJxjVendor::Logout()
{
	if (m_lLoginHandle)
	{
		int iRet = JNetMBClose(m_lLoginHandle);
		if (JNETErrSuccess != iRet)
		{
			m_sLastError = GetErrorString(iRet);
			throw std::exception(m_sLastError.c_str());
		}
	}
}

void CJxjVendor::SearchAll()
{
	
}

//void CJxjVendor::SearchByTime(const std::time_t& start, const std::time_t& end)
void CJxjVendor::Search(const size_t channel, const time_range& range)
{
	if (range.start > range.end)
	{
		OutputDebugString("Time Range Error!");
		throw std::exception("Time Range Error!");
		return;
	}

	vector<time_range> timeRangeList = MakeTimeRangeList(range);
	for (size_t i = 0; i < timeRangeList.size(); i++)
	{
		SearchUnit(channel, timeRangeList[i]);
	}

	// Save Search Video List Result to Config File
	SaveSearchFileListToFile();

	// Load Search Video List Result From Config File (For Test)
	//LoadSearchFileListFromFile();

	return;
}

//void CJxjVendor::DownloadByTime(const std::time_t& start, const std::time_t& end)
void CJxjVendor::Download(const size_t channel, const time_range& range)
{
	m_errCode = Err_No;

	// Init File Starttime and Endtime
	char cTimeStart[24];
	char cTimeEnd[24];
	char cTimeStartZero[24];
	char cTimeEndZero[24];

	struct tm ttime;

	localtime_s(&ttime, &range.start);
	strftime(cTimeStart, 24, "%Y%m%d%H%M%S", &ttime);
	strftime(cTimeStartZero, 24, "%Y%m%d0000", &ttime);
	strftime(cTimeEndZero, 24, "%Y%m%d2359", &ttime);
	localtime_s(&ttime, &range.end);
	strftime(cTimeEnd, 24, "%Y%m%d%H%M%S", &ttime);

	char cFileName[50];
	sprintf_s(cFileName, 50, "channel%02d-%s-%s", channel, cTimeStart, cTimeEnd);

	if (m_files.size() == 0)
	{
		OutputDebugString("Search File List Empty!");
		throw std::exception("Search File List Empty!");
		return;
	}

	RecordFile file;
	size_t i = 0;
	string strFileName(cFileName);
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
		OutputDebugString("Search File List Not Contain the Range!");
		throw std::exception("Search File List Not Contain the Range!");
		return;
	}

	// Init File Save Path 
#ifdef Test_Bug
	string strPath = "D:\\DOWNLOAD_SRC";
#else
	string strPath = m_strRoot;
#endif
	strPath.append("\\");
	strPath.append(cTimeStartZero);
	strPath.append("-");
	strPath.append(cTimeEndZero);
	strPath.append("\\");
	CreateDirectory(strPath.c_str(), NULL);
	strPath.append("佳信捷\\");
	CreateDirectory(strPath.c_str(), NULL);
	char cChannel[10];
	sprintf_s(cChannel, 10, "通道%02d", channel);
	strPath.append(cChannel);
	strPath.append("\\");
	CreateDirectory(strPath.c_str(), NULL);
	strPath.append(file.name);
	strPath.append(".jav");

	// Set File Total Time
	m_lDownLoadTotalTime = file.duration;
	m_lDownloadHandle = JNetRecOpen4Time(m_lLoginHandle, "", channel, j_primary_stream, cTimeStart, cTimeEnd, 4096, IsPlay_Download, JRecDownload, this, m_lRecHandle);
	if (m_lDownloadHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);

		m_lDownloadFileHandle = AVP_CreateRecFile(strPath.c_str(), AVP_PROTOCOL_JPF, Encoder_DM365);
		if (m_lDownloadFileHandle == -1)
		{
			OutputDebugString("Create File Error!");
			throw std::exception("Create File Error!");
		}

		while (m_errCode == Err_No)
		{
			::Sleep(100);
		}
	}
	else
	{
		OutputDebugString("Download File Error!");
		throw std::exception("Download File Error!");
	}
}

//void CJxjVendor::DownloadByName(const std::string& filename)
void CJxjVendor::Download(const size_t channel, const std::string& filename)
{
	if (m_files.size() == 0)
	{
		OutputDebugString("Search File List Empty!");
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
		OutputDebugString("Search File List Not Contain the Filename!");
		throw std::exception("Search File List Not Contain the Filename!");
		return;
	}

	m_errCode = Err_No;

	// Init File Starttime and Endtime
	char cTimeStart[24];
	char cTimeEnd[24];
	char cTimeStartZero[24];
	char cTimeEndZero[24];

	struct tm ttime;

	localtime_s(&ttime, &file.beginTime);
	strftime(cTimeStart, 24, "%Y%m%d%H%M%S", &ttime);
	strftime(cTimeStartZero, 24, "%Y%m%d0000", &ttime);
	strftime(cTimeEndZero, 24, "%Y%m%d2359", &ttime);
	localtime_s(&ttime, &file.endTime);
	strftime(cTimeEnd, 24, "%Y%m%d%H%M%S", &ttime);

	// Init File Save Path 
#ifdef Test_Bug
	string strPath = "D:\\DOWNLOAD_SRC";
#else
	string strPath = m_strRoot;
#endif
	strPath.append("\\");
	strPath.append(cTimeStartZero);
	strPath.append("-");
	strPath.append(cTimeEndZero);
	strPath.append("\\");
	CreateDirectory(strPath.c_str(), NULL);
	strPath.append("佳信捷\\");
	CreateDirectory(strPath.c_str(), NULL);
	char cChannel[10];
	sprintf_s(cChannel, 10, "通道%02d", channel);
	strPath.append(cChannel);
	strPath.append("\\");
	CreateDirectory(strPath.c_str(), NULL);
	strPath.append(file.name);
	strPath.append(".jav");

	// Set File Total Time
	m_lDownLoadTotalTime = file.duration;
	m_lDownloadHandle = JNetRecOpen4Time(m_lLoginHandle, "", channel, j_primary_stream, cTimeStart, cTimeEnd, 4096, IsPlay_Download, JRecDownload, this, m_lRecHandle);
	if (m_lDownloadHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);

		m_lDownloadFileHandle = AVP_CreateRecFile(strPath.c_str(), AVP_PROTOCOL_JPF, Encoder_DM365);
		if (m_lDownloadFileHandle == -1)
		{
			OutputDebugString("Create File Error!");
			throw std::exception("Create File Error!");
		}

		while (m_errCode == Err_No)
		{
			::Sleep(100);
		}
	}
	else
	{
		OutputDebugString("Download File Error!");
		throw std::exception("Download File Error!");
	}
}

//void CJxjVendor::PlayVideo(const std::string& filename)
void CJxjVendor::PlayVideo(const size_t channel, const time_range& range)
{
	m_errCode = Err_No;

	// Init File Starttime and Endtime
	char cTimeStart[24];
	char cTimeEnd[24];

	struct tm ttime;

	localtime_s(&ttime, &range.start);
	strftime(cTimeStart, 24, "%Y%m%d%H%M%S", &ttime);
	localtime_s(&ttime, &range.end);
	strftime(cTimeEnd, 24, "%Y%m%d%H%M%S", &ttime);

	char cFileName[50];
	sprintf_s(cFileName, 50, "channel%02d-%s-%s", channel, cTimeStart, cTimeEnd);
	if (m_files.size() == 0)
	{
		OutputDebugString("Search File List Empty!");
		throw std::exception("Search File List Empty!");
		return;
	}

	RecordFile file;
	size_t i = 0;
	string strFileName(cFileName);
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
		OutputDebugString("Search File List Not Contain the Range!");
		throw std::exception("Search File List Not Contain the Range!");
		return;
	}

	m_iPlayVideoChannel = AVP_GetFreePort();

	m_lRecHandle = JNetRecOpen4Time(m_lLoginHandle, "", channel, 0, cTimeStart, cTimeEnd, 4096, IsPlay_Play, JRecStream, this, m_lRecHandle);
	if (m_lRecHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);
	}

	int iRet = -1;
	AVP_SetPlayPriority(m_iPlayVideoChannel, AVPPlaySmooth);
	AVP_SetDataProtocol(m_iPlayVideoChannel, AVP_PROTOCOL_JPF);

	//AVP_EnableYUVDraw(gChannel,TRUE);
	// 设置通道使用的解码器
	iRet = AVP_SetCoder(m_iPlayVideoChannel, AVP_CODER_JXJ);
	if (iRet != AVPErrSuccess)
	{
		OutputDebugString("AVP_SetCoder Error!");
		throw std::exception("AVP_SetCoder Error!");
		return;
	}

	// 设置显示窗口
#ifdef Test_Bug
	TestWindows testWindows;
	testWindows.Init();
#endif
	AVP_AddPlayWnd(m_iPlayVideoChannel, NULL, g_hWnd, NULL, NULL);

	// 开启解码
	AVP_Play(m_iPlayVideoChannel);

	while (m_errCode == Err_No)
	{
		::Sleep(100);
	}
}
void CJxjVendor::PlayVideo(const size_t channel, const std::string& filename)
{
	if (m_files.size() == 0)
	{
		OutputDebugString("Search File List Empty!");
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
		OutputDebugString("Search File List Not Contain the Filename!");
		throw std::exception("Search File List Not Contain the Filename!");
		return;
	}

	m_errCode = Err_No;

	// Init File Starttime and Endtime
	char cTimeStart[24];
	char cTimeEnd[24];

	struct tm ttime;

	localtime_s(&ttime, &file.beginTime);
	strftime(cTimeStart, 24, "%Y%m%d%H%M%S", &ttime);
	localtime_s(&ttime, &file.endTime);
	strftime(cTimeEnd, 24, "%Y%m%d%H%M%S", &ttime);

	m_iPlayVideoChannel = AVP_GetFreePort();

	m_lRecHandle = JNetRecOpen4Time(m_lLoginHandle, "", channel, 0, cTimeStart, cTimeEnd, 4096, IsPlay_Play, JRecStream, this, m_lRecHandle);
	if (m_lRecHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);
	}

	int iRet = -1;
	AVP_SetPlayPriority(m_iPlayVideoChannel, AVPPlaySmooth);
	AVP_SetDataProtocol(m_iPlayVideoChannel, AVP_PROTOCOL_JPF);

	//AVP_EnableYUVDraw(gChannel,TRUE);
	// 设置通道使用的解码器
	iRet = AVP_SetCoder(m_iPlayVideoChannel, AVP_CODER_JXJ);
	if (iRet != AVPErrSuccess)
	{
		OutputDebugString("AVP_SetCoder Error!");
		throw std::exception("AVP_SetCoder Error!");
		return;
	}

	// 设置显示窗口
#ifdef Test_Bug
	TestWindows testWindows;
	testWindows.Init();
#endif
	AVP_AddPlayWnd(m_iPlayVideoChannel, NULL, g_hWnd, NULL, NULL);

	// 开启解码
	AVP_Play(m_iPlayVideoChannel);

	while (m_errCode == Err_No)
	{
		::Sleep(100);
	}

}
void CJxjVendor::SetDownloadPath(const std::string& Root)
{
	m_strRoot = Root;

	CreateDirectory(Root.c_str(), NULL);
}
void CJxjVendor::throwException()
{
}

/***********************************************************************************************/
int CJxjVendor::ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam)
{
	switch (eType)
	{
	case JNET_EET_LOGINING:		 //正在登陆
	{
	}
		break;
	case JNET_EET_LOGIN_OK:		 //登录成功
	{
									 m_errCode = Err_LoginSuccess;
	}
		break;
	case JNET_EET_LOGIN_ERROR:   //登录失败
	{
									 m_errCode = Err_LoginFail;
	}
		break;
	}

	return 0;
}

void CJxjVendor::MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, const int beginNode, const int endNode, const int ssid, const std::time_t& start, const std::time_t& end)
{
	//查询录像参数
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

void CJxjVendor::SearchUnit(const size_t channel, const time_range& range)
{
	MakeStoreLog(m_storeLog, m_recordType, m_iBeginNode, m_iEndNode, m_iSsid, range.start, range.end);

	// Get Search File Info
	int iRet = -1;
	iRet = JNetGetParam(m_lLoginHandle, channel, PARAM_STORE_LOG, (char *)&m_storeLog, sizeof(m_storeLog), NULL, NULL);
	if (iRet != 0)
	{
		OutputDebugString("Search Error!");
		throw std::exception("Search Error!");
	}

	if (m_storeLog.total_count > 0)
	{
		if (m_storeLog.beg_node == 0)
		{
			//m_files.clear();
		}

		ReFreshVideoList(channel, range);
	}
}

vector<time_range> CJxjVendor::MakeTimeRangeList(const time_range& range)
{
	time_t timeStart = range.start;
	time_t timeEnd = range.end;
	vector<time_range> timeRangeList;

	JTime jStartTime, jStopTime;
	InitSearchTime(jStartTime, jStopTime, timeStart, timeEnd);

	if (timeEnd - timeStart <= oneDay)
	{
		if (jStartTime.date == jStopTime.date)
		{
			timeRangeList.push_back(range);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - jStartTime.hour) * oneHour + (59 - jStartTime.minute) * oneMinute + (59 - jStartTime.second);
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
		int day = (diff / oneDay) + (diff % oneDay > 0 ? 1 : 0);

		if (jStartTime.hour == 0 && jStartTime.minute == 0 && jStartTime.second == 0)
		{
			for (size_t i = 0; i < day - 1; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + oneDay - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + oneDay;
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
			time_t diff = (23 - jStartTime.hour) * oneHour + (59 - jStartTime.minute) * oneMinute + (59 - jStartTime.second);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			timeStart = timeStart + diff + 1;
			for (size_t i = 0; i < day - 2; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + oneDay - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + oneDay;
			}

			if (timeEnd > timeStart + oneDay - 1)
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + oneDay - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + oneDay;
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

void CJxjVendor::InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd)
{
	struct tm Tm;

	localtime_s(&Tm, (const time_t*)&timeStart);
	jStartTime.year = Tm.tm_year;
	jStartTime.month = Tm.tm_mon;
	jStartTime.date = Tm.tm_mday;
	jStartTime.hour = Tm.tm_hour;
	jStartTime.minute = Tm.tm_min;
	jStartTime.second = Tm.tm_sec;
	jStartTime.weekday = Tm.tm_wday;

	localtime_s(&Tm, (const time_t*)&timeEnd);
	jStopTime.year = Tm.tm_year;
	jStopTime.month = Tm.tm_mon;
	jStopTime.date = Tm.tm_mday;
	jStopTime.hour = Tm.tm_hour;
	jStopTime.minute = Tm.tm_min;
	jStopTime.second = Tm.tm_sec;
	jStopTime.weekday = Tm.tm_wday;
}

void CJxjVendor::ReFreshVideoList(int channel, const time_range& range)
{
	AddSearchFileList(channel);

	if (m_iEndNode < (int)m_storeLog.total_count) //每次最多只能取24条 ，分批取
	{
		m_iBeginNode += 24;
		m_iEndNode += 24;
		m_iSsid = m_storeLog.sess_id;
		SearchUnit(channel, range);
	}
	else
	{
		m_iBeginNode = 0;
		m_iEndNode = 23;
		m_iSsid = -1;
	}
}

void CJxjVendor::AddSearchFileList(int channel)
{
	int j = m_iBeginNode;

	for (int i = 0; i< (int)m_storeLog.node_count; i++)
	{
		RecordFile recordFile;
		
		// File Start Time
		JTime jTime = m_storeLog.store[i].beg_time;
		recordFile.beginTime = MakeTimestampByJTime(jTime);
		char strStartTime[20];
		sprintf_s(strStartTime, 20, "%d%02d%02d%02d%02d%02d", jTime.year+1900, jTime.month, jTime.date, jTime.hour, jTime.minute, jTime.second);

		// File End Time
		jTime = m_storeLog.store[i].end_time;
		recordFile.endTime = MakeTimestampByJTime(jTime);
		char strEndTime[20];
		sprintf_s(strEndTime, 20, "%d%02d%02d%02d%02d%02d", jTime.year+1900, jTime.month, jTime.date, jTime.hour, jTime.minute, jTime.second);
	
		// File Belong Time Secton 
		recordFile.strTimeSection = strStartTime;
		recordFile.strTimeSection.append("-");
		recordFile.strTimeSection.append(strEndTime);
		
		// File Duration
		recordFile.duration = recordFile.endTime - recordFile.beginTime;

		// File Channel and File Size
		recordFile.channel = channel;
		//recordFile.size = m_storeLog.store[i].file_size / (1024 * 1024); // MB
		recordFile.size = m_storeLog.store[i].file_size; // Byte

		// File Name
		char fileName[50];
		sprintf_s(fileName, 50, "channel%02d-%s-%s", recordFile.channel, strStartTime, strEndTime);
		recordFile.name = fileName;

		if (!CheckFileExist(recordFile, m_files))
		{
			m_files.push_back(recordFile);
		}
		
		j++;
	}
}

bool CJxjVendor::CheckFileExist(const RecordFile& file, const vector<RecordFile>& fileList)
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

time_t CJxjVendor::MakeTimestampByJTime(JTime jTime)
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

string CJxjVendor::MakeStrTimeByTimestamp(time_t time)
{
	char cTime[50];
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime(cTime, 50, "%Y%m%d%H%M%S", &ttime);

	string strTime(cTime);

	return strTime;
}
string CJxjVendor::MakeStrByInteger(int data)
{
	char cData[50];

	sprintf_s(cData, 50, "%d", data);

	string strTime(cData);

	return strTime;
}

void CJxjVendor::SaveSearchFileListToFile()
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

void CJxjVendor::LoadSearchFileListFromFile()
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
	for (Iter it = d.MemberBegin(); it != d.MemberEnd(); it++)
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
int  CJxjVendor::JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam)
{
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
			g_iDownLoadPos = (pFrame->timestamp_sec - m_lDownLoadStartTime) / (m_lDownLoadTotalTime / 100);
			char cDownInfo[100];
			sprintf_s(cDownInfo, 100, "g_idownloadpos = %d , starttime = %ld, curtime = %ld, totaltime = %ld\r\n", g_iDownLoadPos, m_lDownLoadStartTime, pFrame->timestamp_sec, m_lDownLoadTotalTime);
			OutputDebugString(cDownInfo);
		}

		if ((m_lDownLoadStartTime + m_lDownLoadTotalTime) == pFrame->timestamp_sec)
		{
			m_errCode = Err_DownloadSuccess;

			g_iDownLoadPos = 100;
			m_lDownLoadStartTime = -1;
			m_lDownLoadTotalTime = -1;

			CloseDownload();
		}
	}
	return 0;
}

void CJxjVendor::CloseDownload()
{
	if (m_lDownloadFileHandle)
	{
		AVP_CloseRecFile(m_lDownloadFileHandle);
		m_lDownloadFileHandle = NULL;
	}

	if (m_lDownloadHandle)
	{
		JNetRecClose(m_lDownloadHandle);
		m_lDownloadHandle = NULL;
	}
}

int CJxjVendor::JRecStream(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam)
{
	j_frame_t *pFrame = (j_frame_t *)pBuff;

	char cPlayInfo[100];
	sprintf_s(cPlayInfo, 100, "--------%d    %d\r\n", pFrame->timestamp_sec, pFrame->timestamp_usec);
	OutputDebugString(cPlayInfo);
	AVP_PutFrame(m_iPlayVideoChannel, pBuff);

	return 0;
}

DWORD CJxjVendor::PlayThreadFun(LPVOID lpThreadParameter)
{
	return 0;
}


#include "catch.hpp"

//TEST_CASE_METHOD(CJxjVendor, "Init SDK", "[Init]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//	REQUIRE(handle != nullptr);
//}

//TEST_CASE_METHOD(CJxjVendor, "Login Device", "[Login]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//
//	REQUIRE_NOTHROW(Login("admin", "admin"));
//}

//TEST_CASE_METHOD(CJxjVendor, "Logout Device", "[Logout]")
//{
//	REQUIRE_NOTHROW(Logout());
//}

TEST_CASE_METHOD(CJxjVendor, "Search videos by time_range from device", "[Download]")
{
	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));

	REQUIRE_NOTHROW(Login("admin", "admin"));

	time_range timeRange;
	timeRange.start = gTimeStart;
	timeRange.end = gTimeEnd;
	REQUIRE_NOTHROW(Search(0, timeRange));

#ifdef Test_Filename
	string filename = "channel00-20160619235245-20160620001144";
	REQUIRE_NOTHROW(Download(0, filename));
#else
	time_t start = 1466351565;
	time_t end = 1466352704;
	timeRange.start = start;
	timeRange.end = end;
	REQUIRE_NOTHROW(Download(0, timeRange));
#endif
}

//TEST_CASE_METHOD(CJxjVendor, "Download by time range", "[PlayVideo]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//
//	REQUIRE_NOTHROW(Login("admin", "admin"));
//
//	time_range timeRange;
//	timeRange.start = gTimeStart;
//	timeRange.end = gTimeEnd;
//	REQUIRE_NOTHROW(Search(0, timeRange));
//
//#ifdef Test_Filename
//	string filename = "channel00-20160619235245-20160620001144";
//	REQUIRE_NOTHROW(PlayVideo(0, filename));
//#else
//	time_t start = 1466351565;
//	time_t end = 1466352704;
//	timeRange.start = start;
//	timeRange.end = end;
//	REQUIRE_NOTHROW(PlayVideo(0, timeRange));
//#endif
//}
