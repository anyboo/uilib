#include "stdafx.h"
#include "JxjVendor.h"

int gNVRChn = 0;
bool CJxjVendor::m_bIsLoginOk = false;
eErrCode CJxjVendor::m_errCode = Err_No;

CJxjVendor::CJxjVendor()
{
	// Init Param
	m_iBeginNode = 0;
	m_iEndNode = 23;
	m_iSsid = -1;
	m_lLogin = -1;

	// Init JNetSdk
	int iRet = JNetInit(NULL);
	if (0 != iRet)
	{
		printf("JNetInit error!");
		return;
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
			printf("JNetMBClose error!");
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
	JNetLogin(m_ip.c_str(), m_port, user.c_str(), password.c_str(), 10, ConnEventCB, NULL, JNET_PRO_T_JPF, m_lLogin, NULL);

	while (m_errCode == Err_No)
	{
		::Sleep(100);
	}

	if (m_errCode == Err_LoginFail)
	{
		throw std::exception("Login Failed!");
	}

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
			printf("JNetMBClose error!");
		}
	}
}

void CJxjVendor::SearchAll()
{
	//查询录像参数
	ZeroMemory(&m_storeLog, sizeof(JStoreLog));
	m_storeLog.rec_type = TIMER_RECODE;
	m_storeLog.beg_node = m_iBeginNode;
	m_storeLog.end_node = m_iEndNode;
	m_storeLog.sess_id = m_iSsid;

	m_storeLog.beg_time.year = 2016 - 1900;
	m_storeLog.beg_time.month = 6;
	m_storeLog.beg_time.date = 21;
	m_storeLog.beg_time.weekday = 3;
	m_storeLog.beg_time.hour = 0;
	m_storeLog.beg_time.minute = 0;
	m_storeLog.beg_time.second = 0;

	m_storeLog.end_time.year = 2016 - 1900;
	m_storeLog.end_time.month = 6;
	m_storeLog.end_time.date = 21;
	m_storeLog.end_time.weekday = 3;
	m_storeLog.end_time.hour = 23;
	m_storeLog.end_time.minute = 59;
	m_storeLog.end_time.second = 59;

	//获取设备能力集
	int iRet = -1;
	iRet = JNetGetParam(m_lLogin, gNVRChn, PARAM_STORE_LOG, (char *)&m_storeLog, sizeof(m_storeLog), NULL, NULL);
	if (iRet != 0)
	{
		printf("Search Failed!");
		return;
	}
}
void CJxjVendor::SearchByTime(const std::time_t& start, const std::time_t& end)
{
}

void CJxjVendor::DownloadByTime(const std::time_t& start, const std::time_t& end)
{
	m_downloadHandle = JNetRecOpen4Time(m_lLogin, "", 0, 0, "20120101000000", "20120102235959", MANUAL_RECODE, 0, JRecDownload, this, m_lRecHandle);
	if (m_downloadHandle > 0)
	{

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
	case JNET_EET_LOGINING:		 //正在登陆
	{
									 //cout << "Logining" << endl;
	}
		break;
	case JNET_EET_LOGIN_OK:		 //登录成功
	{
									 //cout << "LoginOK" << endl;
									 m_errCode = Err_LoginSuccess;
	}
		break;
	case JNET_EET_LOGIN_ERROR:   //登录失败
	{
									 cout << "LoginError" << endl;	
									 m_errCode = Err_LoginFail;
	}
		break;
	}

	return 0;
}

int  CJxjVendor::JRecDownload(long lHandle, LPBYTE pBuff, DWORD dwRevLen, void* pUserParam)
{
	if (pBuff)
	{
		j_frame_t *pFrame = (j_frame_t *)pBuff;
		/*if (pDlg->m_lDownLoadStartTime == -1 && pFrame->frame_type != j_audio_frame)
		{
			pDlg->m_lDownLoadStartTime = pFrame->timestamp_sec;
		}


		AVP_WriteRecFile(pDlg->m_lDownLoad, pBuff, dwRevLen, NULL, 0);

		if (pDlg->m_lDownLoadTotalTime == 0)
			return 0;
		if (pFrame->timestamp_sec > 0 && pDlg->m_lDownLoadTotalTime > 0)
		{
			g_PubData.g_iDownLoadPos = (pFrame->timestamp_sec - pDlg->m_lDownLoadStartTime) / (pDlg->m_lDownLoadTotalTime / 100);
			TRACE("g_iDownLoadPos = %d , starttime = %ld, curtime = %ld, totaltime = %ld\r\n", g_PubData.g_iDownLoadPos, pDlg->m_lDownLoadStartTime, pFrame->timestamp_sec, pDlg->m_lDownLoadTotalTime);
		}

		if ((pDlg->m_lDownLoadStartTime + pDlg->m_lDownLoadTotalTime) == pFrame->timestamp_sec)
		{

			TRACE("DownLoad Success\r\n");
			g_PubData.g_iDownLoadPos = 1000;
			pDlg->CloseDownload();
		}*/
	}
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
//	REQUIRE(handle != nullptr);
//
//	REQUIRE_NOTHROW(Login("", ""));
//}

//TEST_CASE_METHOD(CJxjVendor, "Logout Device", "[Logout]")
//{
//	//REQUIRE_NOTHROW(Logout());
//}
//
TEST_CASE_METHOD(CJxjVendor, "Search all videos from device", "[SearchAll]")
{
	REQUIRE_NOTHROW(Init("192.168.0.89", 3321));
	REQUIRE(handle != nullptr);

	REQUIRE_NOTHROW(Login("", ""));

	REQUIRE_NOTHROW(SearchAll());
}
