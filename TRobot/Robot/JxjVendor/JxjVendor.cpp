#include "stdafx.h"
#include "JxjVendor.h"

int gNVRChn = 0;
bool CJxjVendor::m_bIsLoginOk = false;


CJxjVendor::CJxjVendor()
{
	// Init Param
	m_iBeginNode = 0;
	m_iEndNode = 23;
	m_iSsid = -1;

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

	Login("admin", "admin");
}

void CJxjVendor::Login(const std::string& user, const std::string& password)
{
	long lLogin = 0;
	JNetLogin(m_ip.c_str(), m_port, user.c_str(), password.c_str(), 10, ConnEventCB, NULL, JNET_PRO_T_JPF, lLogin, NULL);

	while (!m_bIsLoginOk)
	{
		::Sleep(100);
	}

	//SearchAll();

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
	//��ѯ¼�����
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

	int iRet = -1;
	long lLogin = 0;
	iRet = JNetGetParam(lLogin, gNVRChn, PARAM_STORE_LOG, (char *)&m_storeLog, sizeof(m_storeLog), NULL, NULL);
	if (iRet != 0)
	{
		printf("Search Failed!");
		return;
	}
}

void CJxjVendor::SearchByTime(const std::time_t& time)
{
}

void CJxjVendor::DownloadByTime(const std::time_t& time)
{
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
	case JNET_EET_LOGINING:		 //���ڵ�½
	{
									 cout << "Logining" << endl;
	}
		break;
	case JNET_EET_LOGIN_OK:		 //��¼�ɹ�
	{
									 cout << "OK" << endl;
									 m_bIsLoginOk = true;
	}
		break;
	case JNET_EET_LOGIN_ERROR:   //��¼ʧ��
	{
									 cout << "Error" << endl;
	}
		break;
	}

	return 0;
}

#include "catch.hpp"
TEST_CASE_METHOD(CJxjVendor, "Init SDK", "[Init]")
{
	REQUIRE_NOTHROW(Init("192.168.0.67", 3321));
	REQUIRE(handle != nullptr);
}

TEST_CASE_METHOD(CJxjVendor, "Login Device", "[Login]")
{
	//REQUIRE_NOTHROW(Login("admin", "admin"));
}

TEST_CASE_METHOD(CJxjVendor, "Logout Device", "[Logout]")
{
	//REQUIRE_NOTHROW(Logout());
}

TEST_CASE_METHOD(CJxjVendor, "Search all videos from device", "[SearchAll]")
{
	//REQUIRE_NOTHROW(SearchAll());
}
