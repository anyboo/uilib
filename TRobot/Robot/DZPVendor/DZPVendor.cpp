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
	case H264_DVR_NOERROR: return std::string("没有错误");
	case H264_DVR_SUCCESS: return std::string("返回成功");
	case H264_DVR_SDK_NOTVALID: return std::string("非法请求");
	case H264_DVR_NO_INIT: return std::string("SDK未经初始化");
	case H264_DVR_ILLEGAL_PARAM: return std::string("用户参数不合法");
	case H264_DVR_INVALID_HANDLE: return std::string("句柄无效");
	case H264_DVR_SDK_UNINIT_ERROR: return std::string("SDK清理出错");
	case H264_DVR_SDK_TIMEOUT: return std::string("等待超时");
	case H264_DVR_SDK_MEMORY_ERROR: return std::string("内存错误，创建内存失败");
	case H264_DVR_SDK_NET_ERROR: return std::string("网络错误");
	case H264_DVR_SDK_OPEN_FILE_ERROR: return std::string("打开文件失败");
	case H264_DVR_SDK_UNKNOWNERROR: return std::string("未知错误");
	case H264_DVR_DEV_VER_NOMATCH: return std::string("收到数据不正确，可能版本不匹配");
	case H264_DVR_SDK_NOTSUPPORT: return std::string("版本不支持");
	case H264_DVR_OPEN_CHANNEL_ERROR: return std::string("打开通道失败");
	case H264_DVR_CLOSE_CHANNEL_ERROR: return std::string("关闭通道失败");
	case H264_DVR_SUB_CONNECT_ERROR: return std::string("建立媒体子连接失败");
	case H264_DVR_SUB_CONNECT_SEND_ERROR: return std::string("媒体子连接通讯失败");
	case H264_DVR_NATCONNET_REACHED_MAX: return std::string("Nat视频链接达到最大，不允许新的Nat视频链接");
	case H264_DVR_NOPOWER: return std::string("无权限");
	case H264_DVR_PASSWORD_NOT_VALID: return std::string("账号密码不对");
	case H264_DVR_LOGIN_USER_NOEXIST: return std::string("用户不存在");
	case H264_DVR_USER_LOCKED: return std::string("该用户被锁定");
	case H264_DVR_USER_IN_BLACKLIST: return std::string("该用户不允许访问(在黑名单中)");
	case H264_DVR_USER_HAS_USED: return std::string("该用户以登陆");
	case H264_DVR_USER_NOT_LOGIN: return std::string("该用户没有登陆");
	case H264_DVR_CONNECT_DEVICE_ERROR: return std::string("可能设备不存在");
	case H264_DVR_ACCOUNT_INPUT_NOT_VALID: return std::string("用户管理输入不合法");
	case H264_DVR_ACCOUNT_OVERLAP: return std::string("索引重复");
	case H264_DVR_ACCOUNT_OBJECT_NONE: return std::string("不存在对象, 用于查询时");
	case H264_DVR_ACCOUNT_OBJECT_NOT_VALID: return std::string("不存在对象");
	case H264_DVR_ACCOUNT_OBJECT_IN_USE: return std::string("对象正在使用");
	case H264_DVR_ACCOUNT_SUBSET_OVERLAP: return std::string("子集超范围 (如组的权限超过权限表，用户权限超出组的权限范围等等)");
	case H264_DVR_ACCOUNT_PWD_NOT_VALID: return std::string("密码不正确");
	case H264_DVR_ACCOUNT_PWD_NOT_MATCH: return std::string("密码不匹配");
	case H264_DVR_ACCOUNT_RESERVED: return std::string("保留帐号");
	case H264_DVR_OPT_RESTART: return std::string("保存配置后需要重启应用程序");
	case H264_DVR_OPT_REBOOT: return std::string("需要重启系统");
	case H264_DVR_OPT_FILE_ERROR: return std::string("写文件出错");
	case H264_DVR_OPT_CAPS_ERROR: return std::string("配置特性不支持");
	case H264_DVR_OPT_VALIDATE_ERROR: return std::string("配置校验失败");
	case H264_DVR_OPT_CONFIG_NOT_EXIST: return std::string("请求或者设置的配置不存在");
	case H264_DVR_CTRL_PAUSE_ERROR: return std::string("暂停失败");
	case H264_DVR_SDK_NOTFOUND: return std::string("查找失败，没有找到对应文件");
	case H264_DVR_CFG_NOT_ENABLE: return std::string("配置未启用");
	case H264_DVR_DECORD_FAIL: return std::string("解码失败");
	case H264_DVR_SOCKET_ERROR: return std::string("创建套节字失败");
	case H264_DVR_SOCKET_CONNECT: return std::string("连接套节字失败");
	case H264_DVR_SOCKET_DOMAIN: return std::string("域名解析失败");
	case H264_DVR_SOCKET_SEND: return std::string("发送数据失败");
	case H264_DVR_ARSP_NO_DEVICE: return std::string("没有获取到设备信息，设备应该不在线");
	case H264_DVR_ARSP_BUSING: return std::string("ARSP服务繁忙");
	case H264_DVR_ARSP_BUSING_SELECT: return std::string("ARSP服务繁忙,select失败");
	case H264_DVR_ARSP_BUSING_RECVICE: return std::string("ARSP服务繁忙,recvice失败");
	case H264_DVR_CONNECTSERVER_ERROR: return std::string("连接服务器失败");
	case H264_DVR_CONNECT_FULL: return std::string("服务器连接数已满");
	case H264_DVR_PIRATESOFTWARE: return std::string("设备盗版");
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
	//	m_sLastError = std::string("请先登录!");
	//	return false;
	//}


	////qDebug()<<codec->toUnicode("当前下载的文件信息:")<<H264_DVR_FILE_DATA(nFileinfo[fileInfoIndex]).ch <<H264_DVR_FILE_DATA(nFileinfo[fileInfoIndex]).size<<H264_DVR_FILE_DATA(nFileinfo[fileInfoIndex]).sFileName;
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
		m_sLastError = std::string("查询失败！错误为:") + GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
		return;
	}
	else
	{
		if (fileCount > 0)
		{
			for (int i = 0; i < fileCount; i++)
			{
				if (nriFileinfo[i].size>0)//视频文件必须大于0(测试中有发现文件大小为0情况,所以必须过滤掉改部分)
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