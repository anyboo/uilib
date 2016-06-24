#include "stdafx.h"
#include "JxjVendor.h"

#define oneDay		(24 * 60 * 60)
#define oneHour		(60 * 60)
#define oneMinute	(60)

eErrCode CJxjVendor::m_errCode = Err_No;
long CJxjVendor::m_lDownloadHandle = -1;
long CJxjVendor::m_lDownLoadStartTime = 0;
long CJxjVendor::m_lDownLoadTotalTime = 0;
int CJxjVendor::g_iDownLoadPos = 0;

time_t gTimeStart = 1466265600; // 2016-6-19 00:00:00 1466352000
time_t gTimeEnd = 1466697599; // 2016-6-23 23:59:59

CJxjVendor::CJxjVendor()
{
	// Init Param
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
	
	// Init JNetSdk
	int iRet = JNetInit(NULL);
	if (0 != iRet)
	{
		throw std::exception("JNetInit Error!");
	}
}

CJxjVendor::~CJxjVendor()
{
	if(m_hBhandle)
	{
		int iRet = -1;
		iRet = JNetMBClose(m_hBhandle);
		if( 0 != iRet )
		{
			throw std::exception("JNetMBClose Error!");
		}
	}
}

void CJxjVendor::Init(const std::string& ip, size_t port)
{
	m_ip = ip;
	m_port = port;
}

void CJxjVendor::Login(const std::string& user, const std::string& password)
{
	m_errCode = Err_No;

	JNetLogin(m_ip.c_str(), m_port, user.c_str(), password.c_str(), 10, ConnEventCB, NULL, JNET_PRO_T_JPF, m_lLoginHandle, NULL);

	while (m_errCode == Err_No)
	{
		::Sleep(100);
	}

	if (m_errCode == Err_LoginFail)
	{
		throw std::exception("Login Failed!");
	}

	//// Get Device Info
	//JDeviceInfo deviceinfo;
	//int iRet = JNetGetParam(m_lLoginHandle, m_NVRChn, PARAM_DEVICE_INFO, (char *)&deviceinfo, sizeof(deviceinfo), NULL, NULL);
	//if (iRet != JNETErrSuccess)
	//{
	//	throw std::exception("Get Device Info Failed!");
	//}

	return;
}

void CJxjVendor::Logout()
{
	if (m_hBhandle)
	{
		int iRet = -1;
		iRet = JNetMBClose(m_hBhandle);
		if (0 != iRet)
		{
			throw std::exception("JNetMBClose Error!");
		}
	}
}

void CJxjVendor::SearchAll()
{
	
}

//void CJxjVendor::SearchByTime(const std::time_t& start, const std::time_t& end)
void CJxjVendor::Search(const size_t channel, const time_range& range)
{
	vector<time_range> timeRangeList = MakeTimeRangeList(range);
	for (size_t i = 0; i < timeRangeList.size(); i++)
	{
		SearchUnit(channel, timeRangeList[i]);
	}

	return;
}

//void CJxjVendor::DownloadByTime(const std::time_t& start, const std::time_t& end)
void CJxjVendor::Download(const size_t channel, const time_range& range)
{
	m_errCode = Err_No;

	char cTimeStart[24];
	char cTimeEnd[24];
	memset(cTimeStart, 24, sizeof(cTimeStart));
	memset(cTimeEnd, 24, sizeof(cTimeEnd));

	struct tm ttime;
	localtime_s(&ttime, &range.start);
	strftime(cTimeStart, 24, "%Y%m%d%H%M%S", &ttime);
	localtime_s(&ttime, &range.end);
	strftime(cTimeEnd, 24, "%Y%m%d%H%M%S", &ttime);

	string strTimeStart(cTimeStart);
	string strTimeEnd(cTimeEnd);

	m_lDownloadHandle = JNetRecOpen4Time(m_lLoginHandle, "", channel, j_primary_stream, strTimeStart.c_str(), strTimeEnd.c_str(), m_recordType, IsPlay_Download, JRecDownload, this, m_lRecHandle);
	if (m_lDownloadHandle > 0)
	{
		Sleep(1000);
		JNetRecCtrl(m_lRecHandle, JNET_PB_CTRL_START, NULL);

		while (m_errCode == Err_No)
		{
			::Sleep(100);
		}
	}
	else
	{
		throw std::exception("Download File Error!");
	}
}

//void CJxjVendor::DownloadByName(const std::string& filename)
void CJxjVendor::Download(const size_t channel, const std::string& filename)
{
}

//void CJxjVendor::PlayVideo(const std::string& filename)
void CJxjVendor::PlayVideo(const size_t channel, const time_range& range)
{
}
void CJxjVendor::PlayVideo(const size_t channel, const std::string& filename)
{
}
void CJxjVendor::SetDownloadPath(const std::string& Root)
{
}
void CJxjVendor::throwException()
{
}

/***************************************************/
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
		throw std::exception("Search Error!");
	}

	if (m_storeLog.total_count > 0)
	{
		// if re-search file list, you should initation the file vector first.
		if (m_storeLog.beg_node == 0)
		{
			m_files.empty();
		}

		ReFreshVideoList(channel, range);
	}
	else
	{
		//throw std::exception("Search No Files Error!");
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

	_localtime64_s(&Tm, (const time_t*)&timeStart);
	jStartTime.year = Tm.tm_year;
	jStartTime.month = Tm.tm_mon + 1;
	jStartTime.date = Tm.tm_mday;
	jStartTime.hour = Tm.tm_hour;
	jStartTime.minute = Tm.tm_min;
	jStartTime.second = Tm.tm_sec;
	jStartTime.weekday = Tm.tm_wday;

	_localtime64_s(&Tm, (const time_t*)&timeEnd);
	jStopTime.year = Tm.tm_year;
	jStopTime.month = Tm.tm_mon + 1;
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
		sprintf_s(strStartTime, 20, "%d%02d%02d%02d%02d%02d", jTime.year, jTime.month, jTime.date, jTime.hour, jTime.minute, jTime.second);

		// File End Time
		jTime = m_storeLog.store[i].end_time;
		recordFile.endTime = MakeTimestampByJTime(jTime);
		char strEndTime[20];
		sprintf_s(strEndTime, 20, "%d%02d%02d%02d%02d%02d", jTime.year, jTime.month, jTime.date, jTime.hour, jTime.minute, jTime.second);
		
		// File Channel and File Size
		recordFile.channel = channel;
		//recordFile.size = m_storeLog.store[i].file_size / (1024 * 1024); // MB
		recordFile.size = m_storeLog.store[i].file_size; // Byte

		// File Name
		char fileName[50];
		sprintf_s(fileName, 50, "channel%d-%s-%s", recordFile.channel, strStartTime, strEndTime);
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
	ttime.tm_mon = jTime.month;
	ttime.tm_mday = jTime.date;
	ttime.tm_hour = jTime.hour;
	ttime.tm_min = jTime.minute;
	ttime.tm_sec = jTime.second;
	time_t time = mktime(&ttime);

	return time;
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

		if (m_lDownLoadTotalTime == 0)
			return 0;
		if (pFrame->timestamp_sec > 0 && m_lDownLoadTotalTime > 0)
		{
			g_iDownLoadPos = (pFrame->timestamp_sec - m_lDownLoadStartTime) / (m_lDownLoadTotalTime / 100);
			//printf("g_idownloadpos = %d , starttime = %ld, curtime = %ld, totaltime = %ld\r\n", g_iDownLoadPos, m_lDownLoadStartTime, pFrame->timestamp_sec, m_lDownLoadTotalTime);
		}

		if ((m_lDownLoadStartTime + m_lDownLoadTotalTime) == pFrame->timestamp_sec)
		{
			g_iDownLoadPos = 1000;
			m_errCode = Err_DownloadSuccess;
			CloseDownload();
		}
	}
	return 0;
}

void CJxjVendor::CloseDownload()
{
	if (m_lDownloadHandle)
	{
		JNetRecClose(m_lDownloadHandle);
		m_lDownloadHandle = NULL;
	}
}

#include "catch.hpp"

//CJxjVendor jxjVendor;

//TEST_CASE_METHOD(CJxjVendor, "Init SDK", "[Init]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//	REQUIRE(handle != nullptr);
//}
//
//TEST_CASE_METHOD(CJxjVendor, "Login Device", "[Login]")
//{
//	REQUIRE_NOTHROW(Login("", ""));
//}

//TEST_CASE_METHOD(CJxjVendor, "Logout Device", "[Logout]")
//{
//	//REQUIRE_NOTHROW(Logout());
//}

TEST_CASE_METHOD(CJxjVendor, "Search videos by time_range from device", "[Search]")
{
	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));

	REQUIRE_NOTHROW(Login("admin", "admin"));

	time_range timeRange;
	timeRange.start = gTimeStart;
	timeRange.end = gTimeEnd;

	REQUIRE_NOTHROW(Search(0, timeRange));
}

//TEST_CASE_METHOD(CJxjVendor, "Logout Device", "[SearchAll]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//
//	REQUIRE_NOTHROW(Login("admin", "admin"));
//
//	REQUIRE_NOTHROW(SearchAll());
//}

//TEST_CASE_METHOD(CJxjVendor, "Download by time range", "[DownloadByTime]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//
//	REQUIRE_NOTHROW(Login("admin", "admin"));
//
//	REQUIRE_NOTHROW(DownloadByTime(gTimeStart, gTimeEnd));
//}
