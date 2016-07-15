#include "DZPVendor.h"

#include "Poco/NotificationQueue.h"

// JXJ DZP
#include "netsdk.h"
#include "H264Play.h"

using Poco::NotificationQueue;

#pragma comment(lib, "NetSdk")
#pragma comment(lib, "H264Play")

std::string DZP_MakeFileName(int channel, const std::string& startTime);
void DZP_SearchUnit(const long loginHandle, const size_t channel, const time_range& range, RECORD_FILE_LIST& recordFiles);
void __stdcall DZP_DownLoadPosCallBack(long lPlayHandle, long lTotalSize, long lDownLoadSize, long dwUser);
int __stdcall DZP_RealDataCallBack(long lRealHandle, long dwDataType, unsigned char *pBuffer, long lbufsize, long dwUser);
int DZP_GetDownloadPos(const int fileHandle);

int m_lFileHandle = 0;
static bool DZP_Init_Flag = false;

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
	if (DZP_Init_Flag)
	{
		H264_DVR_Cleanup();
	}
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

	DZP_Init_Flag = true;

	std::cout << "DZP 初始化SDK 成功！" << std::endl;
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
		//throw std::exception(m_sLastError.c_str());
		std::cout << "DZP 登陆 失败！" << std::endl;
		return -1;
	}

	m_iMaxChannel = OutDev.byChanNum + OutDev.iDigChannel;

	std::cout << "DZP 登陆 成功！" << std::endl;

	return loginHandle;
}
void CDZPVendor::Logout(const long loginHandle)
{
	if (loginHandle > 0)
	{
		if (H264_DVR_Logout(loginHandle))
		{
			std::cout << "DZP 退出登陆 成功！" << std::endl;
		}
		else
		{
			std::string m_sLastError = GZLL_GetLastErrorString();
			std::cout << "DZP 退出登陆 失败！" << std::endl;
			throw std::exception(m_sLastError.c_str());
		}
	}
}

void CDZPVendor::StartSearchDevice()
{
	std::cout << "DZP 搜索设备 开始！" << std::endl;

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
	std::cout << "DZP 搜索设备 结束！" << std::endl;
}

void CDZPVendor::SearchAll(const long loginHandle)
{

}
void CDZPVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	std::cout << "DZP 搜索文件 开始！" << std::endl;

	m_files.clear();

	if (range.start > range.end)
	{
		throw std::exception("Time Range Error!");
		return;
	}

	std::vector<time_range> timeRangeList = CCommonUtrl::getInstance().MakeTimeRangeList(range);
	for (size_t i = 0; i < timeRangeList.size(); i++)
	{
		DZP_SearchUnit(loginHandle, channel, timeRangeList[i], m_files);
	}

	std::cout << "DZP 搜索文件 结束！" << std::endl;

	// Save Search Video List Result to Config File
	std::cout << "DZP 写Json数据到文件 开始！" << std::endl;
	CCommonUtrl::getInstance().SaveSearchFileListToFile(m_files, Vendor_DZP_Abbr);
	std::cout << "DZP 写Json数据到文件 结束！" << std::endl;

	// Write File List to DB
	std::cout << "DZP 写文件数据到数据库 开始！" << std::endl;
	CCommonUtrl::getInstance().WriteFileListToDB(m_files);
	std::cout << "DZP 写文件数据到数据库 结束！" << std::endl;

	return;
}
void CDZPVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{
	H264_DVR_FINDINFO info;

	info.nChannelN0 = channel;
	info.nFileType = SDK_RECORD_ALL;

	struct tm Tm;
	_localtime64_s(&Tm, (const time_t*)&range.start);
	memset(&info, 0, sizeof(info));
	TMToNetTime(Tm, info.startTime);
	_localtime64_s(&Tm, (const time_t*)&range.end);
	TMToNetTime(Tm, info.endTime);

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

	std::string strFileName = DZP_MakeFileName(channel, strTimeStart);

	// Init File Save Path 
	std::string strPath = CCommonUtrl::getInstance().MakeDownloadFileFolder(m_sRoot, strTimeStartZero, strTimeEndZero, Vendor_DZP, channel);
	m_lFileHandle = H264_DVR_GetFileByTime(loginHandle, &info, (char *)strPath.c_str(), true, DZP_DownLoadPosCallBack, 0);
	if (m_lFileHandle <= 0)
	{
		std::string m_sLastError = std::string("下载失败！错误为:") + GZLL_GetLastErrorString();
		return;
	}
}
void CDZPVendor::Download(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (0 == loginHandle)
	{
		std::string m_sLastError = std::string("请先登录!");
		throw std::exception(m_sLastError.c_str());
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
		throw std::exception("Search File List Not Contain the Range!");
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

	std::string strFileName = DZP_MakeFileName(channel, strTimeStart);

	// Init File Save Path 
	std::string strPath = CCommonUtrl::getInstance().MakeDownloadFileFolder(m_sRoot, strTimeStartZero, strTimeEndZero, Vendor_DZP, channel);
	strPath.append(file.name);
	H264_DVR_FILE_DATA* pData = (H264_DVR_FILE_DATA*)file.getPrivateData();
	m_lFileHandle = H264_DVR_GetFileByName(loginHandle, pData, (char *)strPath.c_str(), DZP_DownLoadPosCallBack, 0);
	if (m_lFileHandle <= 0)
	{
		std::string m_sLastError = GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
		return;
	}
}
void CDZPVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{
	H264_DVR_FINDINFO info;

	info.nFileType = SDK_RECORD_ALL;
	info.nChannelN0 = channel;

	struct tm Tm;
	_localtime64_s(&Tm, (const time_t*)&range.start);
	memset(&info, 0, sizeof(info));
	TMToNetTime(Tm, info.startTime);
	_localtime64_s(&Tm, (const time_t*)&range.end);
	TMToNetTime(Tm, info.endTime);

	info.hWnd = m_hWnd;

	int playbackHandle = H264_DVR_PlayBackByTime(loginHandle, &info, DZP_DownLoadPosCallBack, DZP_RealDataCallBack, 0);
	if (0 == playbackHandle)
	{
		H264_DVR_StopPlayBack(playbackHandle);
		std::string m_sLastError = GZLL_GetLastErrorString();
		throw std::exception(m_sLastError.c_str());
		return;
	}
}
void CDZPVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (0 == loginHandle)
	{
		std::string m_sLastError = std::string("请先登录!");
		throw std::exception(m_sLastError.c_str());
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
		throw std::exception("Search File List Not Contain the Range!");
		return;
	}

	H264_DVR_FILE_DATA* fileData = (H264_DVR_FILE_DATA*)file.getPrivateData();
	fileData->hWnd = m_hWnd;
	int playbackHandle = H264_DVR_PlayBackByName(loginHandle, fileData, nullptr, NULL, 0);
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
std::string DZP_MakeFileName(int channel, const std::string& startTime)
{
	std::string strFileName;
	std::string strStartTime = startTime.c_str();

	channel += 1;

	if (channel < 10)
	{
		strFileName += "0";
	}
	strFileName += std::to_string(channel);
	strFileName += "-";
	strFileName += strStartTime.substr(0, 8).data();
	strFileName += "-";
	strFileName += strStartTime.substr(8, strStartTime.length() - 8).data();
	strFileName.append(".h264");

	return strFileName;
}
void DZP_SearchUnit(const long loginHandle, const size_t channel, const time_range& range, RECORD_FILE_LIST& recordFiles)
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
		std::string m_sLastError = std::string("查询失败！错误为:") + GZLL_GetLastErrorString();
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

					// File Belong Time Secton 
					record.strTimeSection = strStartTime.data();
					record.strTimeSection.append("-");
					record.strTimeSection += strEndTime.data();

					// File Duration
					record.duration = record.endTime - record.beginTime;

					// File Channel and so on
					record.channel = channel;
					//record.name = nriFileinfo[i].sFileName;
					record.name = DZP_MakeFileName(channel, strStartTime);

					record.size = nriFileinfo[i].size * 1024;
					record.setPrivateData(&nriFileinfo[i], sizeof(H264_DVR_FILE_DATA));

					recordFiles.push_back(record);
					record.deletePrivateData();
				}
			}
		}
	}
}

void __stdcall DZP_DownLoadPosCallBack(long lPlayHandle, long lTotalSize, long lDownLoadSize, long dwUser)
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
int __stdcall DZP_RealDataCallBack(long lRealHandle, long dwDataType, unsigned char *pBuffer, long lbufsize, long dwUser)
{
	return 0;
}
int DZP_GetDownloadPos(const int fileHandle)
{
	int pos = H264_DVR_GetDownloadPos(fileHandle);

	return pos;
}