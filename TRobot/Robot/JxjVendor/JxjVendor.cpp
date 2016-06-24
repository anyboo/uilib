#include "stdafx.h"
#include "JxjVendor.h"

eErrCode CJxjVendor::m_errCode = Err_No;
long CJxjVendor::m_lDownloadHandle = -1;
long CJxjVendor::m_lDownLoadStartTime = 0;
long CJxjVendor::m_lDownLoadTotalTime = 0;
int CJxjVendor::g_iDownLoadPos = 0;

time_t gTimeStart = 1466352000; // 2016-6-20 00:00:00
//time_t gTimeStart = 1434729600; // 2015-6-20 00:00:00
time_t gTimeEnd = 1466697599; // 2016-6-23 23:59:59

CJxjVendor::CJxjVendor()
{
	// Init Param
	/* Login */
	m_lLoginHandle = -1;
	m_ip = "";
	m_port = 0;
	m_NVRChn = 0;

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
void CJxjVendor::SearchByTime(const std::time_t& start, const std::time_t& end)
{
	MakeStoreLog(m_storeLog, m_recordType,m_iBeginNode, m_iEndNode, m_iSsid, start, end);

	// Get Search File Info
	int iRet = -1;
	iRet = JNetGetParam(m_lLoginHandle, m_NVRChn, PARAM_STORE_LOG, (char *)&m_storeLog, sizeof(m_storeLog), NULL, NULL);
	if (iRet != 0)
	{
		throw std::exception("Search Error!");
	}

	if (m_storeLog.total_count > 0)
	{
		//m_store = m_storeLog.store[0];
	}
	else
	{
		throw std::exception("Search No Files Error!");
	}
}

void CJxjVendor::DownloadByTime(const std::time_t& start, const std::time_t& end)
{
	m_errCode = Err_No;

	char cTimeStart[24];
	char cTimeEnd[24];
	memset(cTimeStart, 24, sizeof(cTimeStart));
	memset(cTimeEnd, 24, sizeof(cTimeEnd));

	struct tm ttime;
	localtime_s(&ttime, &start);
	strftime(cTimeStart, 24, "%Y%m%d%H%M%S", &ttime);
	localtime_s(&ttime, &end);
	strftime(cTimeEnd, 24, "%Y%m%d%H%M%S", &ttime);

	string strTimeStart(cTimeStart);
	string strTimeEnd(cTimeEnd);

	m_lDownloadHandle = JNetRecOpen4Time(m_lLoginHandle, "", m_NVRChn, j_primary_stream, strTimeStart.c_str(), strTimeEnd.c_str(), m_recordType, IsPlay_Download, JRecDownload, this, m_lRecHandle);
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

void CJxjVendor::DownloadByName(const std::string& filename)
{
}

void CJxjVendor::PlayVideo(const std::string& filename)
{
}

int CJxjVendor::ConnEventCB(long lHandle, eJNetEvent eType, int iDataType, void* pEventData, int iDataLen, void* pUserParam)
{
	switch (eType)
	{
	case JNET_EET_LOGINING:		 //ÕýÔÚµÇÂ½
	{
	}
		break;
	case JNET_EET_LOGIN_OK:		 //µÇÂ¼³É¹¦
	{
									 m_errCode = Err_LoginSuccess;
	}
		break;
	case JNET_EET_LOGIN_ERROR:   //µÇÂ¼Ê§°Ü
	{
									 m_errCode = Err_LoginFail;
	}
		break;
	}

	return 0;
}

void CJxjVendor::MakeStoreLog(JStoreLog& storeLog, const JRecodeType recordType, 
	const int beginNode, const int endNode, const int ssid, 
	const std::time_t& start, const std::time_t& end)
{
	//²éÑ¯Â¼Ïñ²ÎÊý
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
//TEST_CASE_METHOD(CJxjVendor, "Init SDK", "[Init]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//	REQUIRE(handle != nullptr);
//}

//TEST_CASE_METHOD(CJxjVendor, "Login Device", "[Login]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//	REQUIRE(handle != nullptr);
//
//	REQUIRE_NOTHROW(Login("", ""));
//}

//TEST_CASE_METHOD(CJxjVendor, "Logout Device", "[Logout]")
//{
//	//REQUIRE_NOTHROW(Logout());
//}

TEST_CASE_METHOD(CJxjVendor, "Search videos by time_range from device", "[SearchByTime]")
{
	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));

	REQUIRE_NOTHROW(Login("admin", "admin"));

	REQUIRE_NOTHROW(SearchByTime(gTimeStart, gTimeEnd));
}

//TEST_CASE_METHOD(CJxjVendor, "Download by time range", "[DownloadByTime]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
//
//	REQUIRE_NOTHROW(Login("admin", "admin"));
//
//	REQUIRE_NOTHROW(DownloadByTime(gTimeStart, gTimeEnd));
//}
