#include <Windows.h>
#include "DZPVendor.h"

#include "netsdk.h"
#include "H264Play.h"

#pragma comment(lib, "DZPVendor\\sdk\\NetSdk")
#pragma comment(lib, "DZPVendor\\sdk\\H264Play")

//#define Test_Bug
#define Test_Filename

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

}

CDZPVendor::~CDZPVendor()
{
	//if (m_lLoginHandle != 0 && !H264_DVR_Logout(m_lLoginHandle))
	//{
	//	m_sLastError = GZLL_GetLastErrorString();
	//	throw std::exception(m_sLastError.c_str());
	//	return;
	//}
}

void CDZPVendor::Init()
{
	int iRet = iRet = H264_DVR_Init(nullptr, 0);
	if (!iRet)
	{
		m_sLastError = GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
	}

	H264_DVR_SetConnectTime(5000, 3);
}

long CDZPVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	H264_DVR_DEVICEINFO OutDev;
	memset(&OutDev, 0, sizeof(H264_DVR_DEVICEINFO));
	int nError = 0;

	H264_DVR_SetConnectTime(3000, 1);

	int loginHandle = H264_DVR_Login((char *)ip.c_str(), port, (char*)user.c_str(), (char*)password.c_str(), &OutDev, &nError, TCPSOCKET);
	if (loginHandle <= 0)
	{
		m_sLastError = GZLL_GetLastErrorString(nError);
		throw std::exception(m_sLastError.c_str());
		return loginHandle;
	}

	return loginHandle;
}
void CDZPVendor::Logout(const long loginHandle)
{
	if (loginHandle != 0 && !H264_DVR_Logout(loginHandle))
	{
		m_sLastError = GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
		return;
	}
}

void CDZPVendor::SearchAll(const long loginHandle)
{

}
void CDZPVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	m_files.clear();

	if (range.start > range.end)
	{
		throw std::exception("Time Range Error!");
		return;
	}

	std::vector<time_range> timeRangeList = CCommonUtrl::getInstance().MakeTimeRangeList(range);
	for (size_t i = 0; i < timeRangeList.size(); i++)
	{
		SearchUnit(loginHandle, channel, timeRangeList[i]);
	}

	return;
}
void CDZPVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{
	//if (0 == m_lLoginHandle)
	//{
	//	m_sLastError = std::string("���ȵ�¼!");
	//	return false;
	//}


	////qDebug()<<codec->toUnicode("��ǰ���ص��ļ���Ϣ:")<<H264_DVR_FILE_DATA(nFileinfo[fileInfoIndex]).ch <<H264_DVR_FILE_DATA(nFileinfo[fileInfoIndex]).size<<H264_DVR_FILE_DATA(nFileinfo[fileInfoIndex]).sFileName;
	//H264_DVR_FILE_DATA* pData = (H264_DVR_FILE_DATA*)file.getPrivateData();
	//hdl = H264_DVR_GetFileByName(m_lLoginHandle, pData, (char *)saveFileName);
	//if (0 == hdl)
	//{
	//	m_sLastError = GZLL_GetLastErrorString();
	//	return false;
	//}
}
void CDZPVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{

}
void CDZPVendor::Download(const long loginHandle, const size_t channel, const std::string& filename)
{

}
void CDZPVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{

}
void CDZPVendor::SetDownloadPath(const std::string& Root)
{

}
void CDZPVendor::throwException()
{

}
/****************************************************************************************/
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

void CDZPVendor::SearchUnit(const long loginHandle, const size_t channel, const time_range& range)
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
		m_sLastError = std::string("��ѯʧ�ܣ�����Ϊ:") + GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
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
					Record record;

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

					// File Belong Time Secton 
					record.strTimeSection = strStartTime.data();
					record.strTimeSection.append("-");
					record.strTimeSection += strEndTime.data();

					// File Duration
					record.duration = record.endTime - record.beginTime;

					// File Channel and so on
					record.channel = channel;
					record.name = nriFileinfo[i].sFileName;
					record.alias = CCommonUtrl::getInstance().MakeFileName(channel, strStartTime, strEndTime);

					record.size = nriFileinfo[i].size * 1024;
					record.setPrivateData(&nriFileinfo[i], sizeof(H264_DVR_FILE_DATA));
			
					m_files.push_back(record);
					record.deletePrivateData();
				}
			}
		}
	}
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

//TEST_CASE_METHOD(CDZPVendor, "Search videos by time_range from device", "[Search]")
//{
//	REQUIRE_NOTHROW(Init("192.168.0.39", 34567));
//
//	REQUIRE_NOTHROW(Login("admin", ""));
//
//	time_range timeRange;
//
//	time_t gTimeStart = 1466265600; // 2016-6-19 00:00:00 1466352000
//	time_t gTimeEnd = 1466697599; // 2016-6-23 23:59:59
//	timeRange.start = gTimeStart;
//	timeRange.end = gTimeEnd;
//	REQUIRE_NOTHROW(Search(0, timeRange));
//
//#ifdef Test_Filename
//	std::string filename = "channel00-20160619235245-20160620001144";
//	REQUIRE_NOTHROW(Download(0, filename));
//#else
//	//time_t start = 1466351565;
//	//time_t end = 1466352704;
//	//timeRange.start = start;
//	//timeRange.end = end;
//	//REQUIRE_NOTHROW(Download(0, timeRange));
//#endif
//}