#include <Windows.h>
#include "DZPVendor.h"

#include "netsdk.h"
#include "H264Play.h"

#pragma comment(lib, "DZPVendor\\sdk\\NetSdk")
#pragma comment(lib, "DZPVendor\\sdk\\H264Play")

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

CDZPVendor::CDZPVendor()
{
	m_lLoginHandle = 0;
}

CDZPVendor::~CDZPVendor()
{
}

void CDZPVendor::Init(const std::string& ip, size_t port)
{
	int iRet = iRet = H264_DVR_Init(nullptr, 0);
	if (!iRet)
	{
		m_sLastError = GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
	}

	H264_DVR_SetConnectTime(5000, 3);

	m_ip = ip;
	m_port = port;
}

void CDZPVendor::Login(const std::string& user, const std::string& password)
{
	H264_DVR_DEVICEINFO OutDev;
	memset(&OutDev, 0, sizeof(H264_DVR_DEVICEINFO));
	int nError = 0;

	H264_DVR_SetConnectTime(3000, 1);

	m_lLoginHandle = H264_DVR_Login((char *)m_ip.c_str(), m_port, (char*)user.c_str(), (char*)password.c_str(), &OutDev, &nError, TCPSOCKET);
	if (m_lLoginHandle <= 0)
	{
		m_sLastError = GZLL_GetLastErrorString(nError);
		throw std::exception(m_sLastError.c_str());
		return ;
	}
}
void CDZPVendor::Logout()
{

}
void CDZPVendor::SearchAll()
{

}
void CDZPVendor::Search(const size_t channel, const time_range& range)
{

}
void CDZPVendor::Download(const size_t channel, const time_range& range)
{

}
void CDZPVendor::PlayVideo(const size_t channel, const time_range& range)
{

}
void CDZPVendor::Download(const size_t channel, const std::string& filename)
{

}
void CDZPVendor::PlayVideo(const size_t channel, const std::string& filename)
{

}
void CDZPVendor::SetDownloadPath(const std::string& Root)
{

}
void CDZPVendor::throwException()
{

}


#include "catch.hpp"

//TEST_CASE_METHOD(CDZPVendor, "Init SDK", "[Init]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.15", 34567));
//	REQUIRE(handle != nullptr);
//}

//TEST_CASE_METHOD(CDZPVendor, "Login Device", "[Login]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.39", 34567));
//
//	REQUIRE_NOTHROW(Login("admin", ""));
//}
