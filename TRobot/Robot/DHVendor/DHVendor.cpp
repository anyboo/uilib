#include "DHVendor.h"

//DH SDK
#include "dhnetsdk.h"

#pragma comment(lib, "dhnetsdk")

NET_DEVICEINFO m_deviceInfo;

void CALLBACK DH_BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser);
void CALLBACK DH_BTDownLoadFile(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
void CALLBACK DH_PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
int CALLBACK DH_PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser);

std::string DH_MakeStrByInteger(int data);
std::string DH_MakeStrTimeByTimestamp(time_t time);

std::string DH_GetLastErrorString();
void DH_timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd);
void DH_timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH);
void DH_trTOnt(NET_TIME &ntStartTime, NET_TIME &ntEndTime, const time_range range);
void DH_CreatePath(const size_t channel);



DHVendor::DHVendor()
{
	m_eSDKType = DH_SDK;
	m_sDefUserName = "admin";
	m_sDefPassword = "";
	m_iMaxChannel = 0;

	m_lSearchDeviceHandle = -1;
}

DHVendor::~DHVendor()
{
}

void DHVendor::Init()
{
	BOOL bInit = CLIENT_Init(0, 0);
	
	if (!bInit)
	{
		std::cout << "初始化错误：" << CLIENT_GetLastError() << std::endl;
		throw std::exception("Init failed");
	}
	else
	{
		std::cout << "DH 初始化SDK 成功！" << std::endl;
	}
}

long DHVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	int nError = 0;
	long lLoginHandle = -1;
	lLoginHandle = CLIENT_Login((char *)ip.c_str(), port, (char *)(user.c_str()), (char *)(password.c_str()), &m_deviceInfo, &nError);

	if (0 != nError)
	{
		//std::cout << "登录错误(nError)：" << DH_GetLastErrorString().c_str() << std::endl;
		std::cout << "DH 登陆 失败！" << std::endl;
		return -1;
	}

	if (0 == lLoginHandle)
	{
		//std::cout << "登录错误(lLogin)：" << DH_GetLastErrorString().c_str() << std::endl;
		std::cout << "DH 登陆 失败！" << std::endl;
		return -1;
	}

	if (lLoginHandle > 0)
	{
		int nRetLen = 0;
		NET_DEV_CHN_COUNT_INFO stuChn = { sizeof(NET_DEV_CHN_COUNT_INFO) };
		stuChn.stuVideoIn.dwSize = sizeof(stuChn.stuVideoIn);
		stuChn.stuVideoOut.dwSize = sizeof(stuChn.stuVideoOut);

		BOOL bDevState = CLIENT_QueryDevState(lLoginHandle, DH_DEVSTATE_DEV_CHN_COUNT, (char*)&stuChn, stuChn.dwSize, &nRetLen, 1000);

		if (bDevState)
		{
			m_iMaxChannel = stuChn.stuVideoIn.nMaxTotal;
		}
		else
		{
			std::cout << "Get channel failed" << DH_GetLastErrorString() << std::endl;
			throw std::exception("Get channel failed");
			return -1;
		}
	}

	std::cout << "DH 登陆 成功！" << std::endl;

	return lLoginHandle;
}

void DHVendor::Logout(const long loginHandle)
{
	if (loginHandle > 0)
	{
		if (!CLIENT_Logout(loginHandle))
		{
			//std::cout << "退出失败：" << DH_GetLastErrorString().c_str() << std::endl;
			std::cout << "DH 退出登陆 失败！" << std::endl;
			throw std::exception("Logout failed");
		}
		else
		{
			std::cout << "DH 退出登陆 成功！" << std::endl;
		}
	}
}

void DHVendor::StartSearchDevice()
{
	std::cout << "DH 搜索设备 开始！" << std::endl;

 	DEVICE_NET_INFO Device[256] = { 0 };
 	int nLength = 0;
 
 	BOOL bRet = CLIENT_SearchDevices((char *)Device, sizeof(DEVICE_NET_INFO) * 256, &nLength, 3000);
 
	if (bRet)
	{
		int i;
		for (i = 0; i < nLength / sizeof(DEVICE_NET_INFO); i++)
		{
			NET_DEVICE_INFO* ndiInfo = new NET_DEVICE_INFO();
			int nLen = 0;
			ndiInfo->nSDKType = DH_SDK;

			nLen = ((strlen(Device[i].szIP)) < MAX_IPADDR_LEN) ? strlen(Device[i].szIP) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo->szIp, Device[i].szIP, nLen);

			ndiInfo->nPort = Device[i].nPort;

			nLen = ((strlen(Device[i].szSubmask)) < MAX_IPADDR_LEN) ? strlen(Device[i].szSubmask) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo->szSubmask, Device[i].szSubmask, nLen);

			nLen = (strlen(Device[i].szMac) < MAX_MACADDR_LEN) ? strlen(Device[i].szMac) : MAX_MACADDR_LEN;
			memcpy(&ndiInfo->szMac, &Device[i].szMac, nLen);

			ndiInfo->pVendor = this;

			m_listDeviceInfo.push_back(ndiInfo);
		}
	}
}

void DHVendor::StopSearchDevice()
{
	std::cout << "DH 搜索设备 结束！" << std::endl;
}

void DHVendor::SearchAll(const long loginHandle)
{

}

void timeDHToStd(NET_TIME *pTimeDH, tm *pTimeStd)
{
	pTimeStd->tm_year = pTimeDH->dwYear - 1900;
	pTimeStd->tm_mon = pTimeDH->dwMonth - 1;
	pTimeStd->tm_mday = pTimeDH->dwDay;
	pTimeStd->tm_hour = pTimeDH->dwHour;
	pTimeStd->tm_min = pTimeDH->dwMinute;
	pTimeStd->tm_sec = pTimeDH->dwSecond;
}

void timeStdToDH(tm *pTimeStd, NET_TIME *pTimeDH)
{
	pTimeDH->dwYear = pTimeStd->tm_year + 1900;
	pTimeDH->dwMonth = pTimeStd->tm_mon + 1;
	pTimeDH->dwDay = pTimeStd->tm_mday;
	pTimeDH->dwHour = pTimeStd->tm_hour;
	pTimeDH->dwMinute = pTimeStd->tm_min;
	pTimeDH->dwSecond = pTimeStd->tm_sec;
}

void DHVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	if (0 >= loginHandle)
	{
		std::cout << "请先登录!" << std::endl;
		return;
	}

	if (range.start >= range.end)
	{
		std::cout << "时间范围不对!" << std::endl;
		return;
	}

	//m_files.clear();

	std::vector<time_range> trInfor = CCommonUtrl::getInstance().MakeTimeRangeList(range);

	std::vector<time_range>::iterator it;

	for (it = trInfor.begin(); it != trInfor.end(); ++it)
	{
		NET_TIME ntStime;
		NET_TIME ntEtime;

		DH_trTOnt(ntStime, ntEtime, *it);

		NET_RECORDFILE_INFO ifileinfo[MAX_SEARCH_COUNT];
		ZeroMemory(ifileinfo, sizeof(ifileinfo));
		int iMaxNum = 0;

		BOOL bRet = CLIENT_QueryRecordFile(loginHandle, channel, 0, &ntStime, &ntEtime, 0, ifileinfo, sizeof(ifileinfo), &iMaxNum, 5000, true);

		if (!bRet)
		{
			std::cout << "GetRecordFileList 查询录像失败，错误原因：" << DH_GetLastErrorString() << std::endl;
			throw std::exception("Search file by time failed");
		}
		if (iMaxNum <= 0)
		{
			std::cout << "GetRecordFileList 查询录像成功，录像个数为0" << std::endl;
		}

		NET_RECORDFILE_INFO item;
		RecordFile info;
		tm sTm;
		tm eTm;

		char szTime[512];

		for (int i = 0; i < iMaxNum; i++)
		{
			item = ifileinfo[i];

			timeDHToStd(&item.starttime, &sTm);
			timeDHToStd(&item.endtime, &eTm);

			ZeroMemory(szTime, 512);
			sprintf_s(szTime, "channel%d-%d%02d%02d%02d%02d%02d-%d%02d%02d%02d%02d%02d-%d", channel,
				ntStime.dwYear, ntStime.dwMonth, ntStime.dwDay,
				ntStime.dwHour, ntStime.dwMinute, ntStime.dwSecond,
				ntEtime.dwYear, ntEtime.dwMonth, ntEtime.dwDay, ntEtime.dwHour, ntEtime.dwMinute, ntEtime.dwSecond, i);

			info.channel = item.ch;
			info.size = item.size * 1024;
			info.name = szTime;

			info.beginTime = mktime(&sTm);
			info.endTime = mktime(&eTm);
			info.setPrivateData(&ifileinfo[i], sizeof(NET_RECORDFILE_INFO));
			m_files.push_back(info);

			//std::cout << "GetRecordFileList 文件名:" << info.name << std::endl<< "  " << "文件大小:" << info.size << "  " << "通道:" << info.channel << std::endl;
		}
	}

	// Save Search Video List Result to Config File
	CCommonUtrl::getInstance().SaveSearchFileListToFile(m_files, Vendor_DH_Abbr);

	// Write File List to DB
	CCommonUtrl::getInstance().WriteFileListToDB(m_files);
}

void DHVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{
	if (0 >= loginHandle)
	{
		std::cout << "请先登录!" << std::endl;
		return;
	}

	NET_TIME ntStime;
	NET_TIME ntEtime;

	DH_trTOnt(ntStime, ntEtime, range);

	DH_CreatePath(channel);

	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\大华\\通道%d\\channel%d-%d%02d%02d%02d%02d%02d-%d%02d%02d%02d%02d%02d.dav", channel, channel, ntStime.dwYear, ntStime.dwMonth, ntStime.dwDay,
		ntStime.dwHour, ntStime.dwMinute, ntStime.dwSecond, ntEtime.dwYear, ntEtime.dwMonth, ntEtime.dwDay, ntEtime.dwHour, ntEtime.dwMinute, ntEtime.dwSecond);

	BOOL bRet = CLIENT_DownloadByTime(loginHandle, channel, 0, &ntStime, &ntEtime, szTime, DH_BTDownLoadPos, (DWORD)this);
	std::cout << "strName:" << szTime << std::endl;

	int total, cur;
	total = 0;
	cur = 0;
	BOOL bret = CLIENT_GetDownloadPos(bRet, &total, &cur);
	while ((cur / total) != 1)
	{
		std::cout << "进度：" << (double)(cur / total) << std::endl;
	}

	if (0 == bRet)
	{
		std::cout << "downLoadByRecordFile 下载录像失败，错误原因：" << DH_GetLastErrorString() << std::endl;
		throw std::exception("Download by Record file failed");
		return;
	}
	else
	{
		std::cout << "downLoadByRecordFile 下载录像成功！" << std::endl;
	}
}

void DHVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{
	if (0 >= loginHandle)
	{
		std::cout << "PlayVideo 在线播放失败原因：" << DH_GetLastErrorString() << std::endl;
		throw std::exception("Login handle by Record file failed");
		return;
	}

	NET_TIME ntStime;
	NET_TIME ntEtime;

	DH_trTOnt(ntStime, ntEtime, range);

	BOOL lPlayID = CLIENT_PlayBackByTimeEx(loginHandle, channel, &ntStime, &ntEtime, m_hWnd, DH_PlayCallBack, (DWORD)this, DH_PBDataCallBack, (DWORD)this);

	if (!lPlayID)
	{
		std::cout << "播放失败原因：" << DH_GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}
}

void DHVendor::Download(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (0 >= loginHandle)
	{
		std::cout << "请先登录!" << std::endl;
		return;
	}

	DH_CreatePath(channel);

	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\大华\\通道%d\\", channel);

	char szBuf[] = ".dav";
	strcat_s(szTime, (char *)filename.c_str());
	strcat_s(szTime, szBuf);

	std::cout << szTime << std::endl;

	std::vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{
			NET_RECORDFILE_INFO* pData = (NET_RECORDFILE_INFO*)it->getPrivateData();
			BOOL bRet = CLIENT_DownloadByRecordFile(loginHandle, pData, szTime, DH_BTDownLoadFile, (DWORD)this);

			int total, cur;
			total = 0;
			cur = 0;
			BOOL bret = CLIENT_GetDownloadPos(bRet, &total, &cur);
			while ((cur / total) != 1)
			{
				std::cout << "进度：" << (double)(cur / total) << std::endl;
			}

			if (0 == bRet)
			{
				std::cout << "downLoadByRecordFile 下载录像失败，错误原因：" << DH_GetLastErrorString() << std::endl;
				throw std::exception("Download by Record file failed");
				return;
			}
			else
			{
				std::cout << "downLoadByRecordFile 下载录像成功！" << std::endl;
			}
		}
		if (m_files.size() - 1 == nSize)
		{
			std::cout << "下载文件名不存在！" << std::endl;
		}
		nSize++;
	}
}

void DHVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (0 >= loginHandle)
	{
		std::cout << "PlayVideo 在线播放失败原因：" << DH_GetLastErrorString() << std::endl;
		return;
	}

	std::vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{
			NET_RECORDFILE_INFO* pData = (NET_RECORDFILE_INFO*)it->getPrivateData();
			BOOL lPlayID = CLIENT_PlayBackByRecordFile(loginHandle, pData, m_hWnd, DH_PlayCallBack, (DWORD)this);

			system("PAUSE");

			if (!lPlayID)
			{
				std::cout << "播放失败原因：" << DH_GetLastErrorString() << std::endl;
				throw std::exception("Play back by time failed");
			}
		}

		if (m_files.size() - 1 == nSize)
		{
			std::cout << "播放文件名不存在！" << std::endl;
		}
		nSize++;
	}


	
}

void DHVendor::SetDownloadPath(const std::string& Root)
{
	m_sRoot = Root;
}

void DHVendor::throwException()
{

}

void CALLBACK DH_BTDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser)
{
	
}

void CALLBACK DH_BTDownLoadFile(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{

}

void CALLBACK DH_PlayCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	if (dwUser == 0)
	{
		return;
	}
}

int CALLBACK DH_PBDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser)
{
	if (dwUser == 0)
	{
		return -1;
	}

	return 1;
}


void DH_CreatePath(const size_t channel)
{
	std::string strPath = "D:\\DownLoadVideo\\";

	BOOL bOne = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bOne)
	{
		std::cout << "Error or exit!" << std::endl;
		return;
	}

	strPath.append("大华\\");
	BOOL bTwo = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bTwo)
	{
		std::cout << "Error or exit!!" << std::endl;
		return;
	}
	char szChannel[10];
	ZeroMemory(szChannel, 10);
	sprintf_s(szChannel, "通道%d", channel);
	strPath.append(szChannel);
	strPath.append("\\");

	BOOL bThree = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bThree)
	{
		std::cout << "Error!" << std::endl;
		return;
	}
}

void DH_trTOnt(NET_TIME &ntStartTime, NET_TIME &ntEndTime, const time_range range)
{
	tm tmStartTime;
	tm tmEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);

	timeStdToDH(&tmStartTime, &ntStartTime);
	timeStdToDH(&tmEndTime, &ntEndTime);
}

std::string DH_MakeStrTimeByTimestamp(time_t time)
{
	char cTime[50];
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime(cTime, 50, "%Y%m%d%H%M%S", &ttime);

	std::string strTime(cTime);

	return strTime;
}
std::string DH_MakeStrByInteger(int data)
{
	char cData[50];

	sprintf_s(cData, 50, "%d", data);

	std::string strTime(cData);

	return strTime;
}

std::string DH_GetLastErrorString()
{
	DWORD dwError;
	dwError = CLIENT_GetLastError();

	switch (dwError)
	{
	case NET_NOERROR: return "没有错误";
	case NET_ERROR: return "未知错误";
	case NET_SYSTEM_ERROR: return "Windows系统出错";
	case NET_NETWORK_ERROR: return "网络错误，可能是因为网络超时";
	case NET_DEV_VER_NOMATCH: return "设备协议不匹配";
	case NET_INVALID_HANDLE: return "句柄无效";
	case NET_OPEN_CHANNEL_ERROR: return "打开通道失败";
	case NET_CLOSE_CHANNEL_ERROR: return "关闭通道失败";
	case NET_ILLEGAL_PARAM: return "用户参数不合法";
	case NET_SDK_INIT_ERROR: return "SDK初始化出错";
	case NET_SDK_UNINIT_ERROR: return "SDK清理出错";
	case NET_RENDER_OPEN_ERROR: return "申请render资源出错";
	case NET_DEC_OPEN_ERROR: return "打开解码库出错";
	case NET_DEC_CLOSE_ERROR: return "关闭解码库出错";
	case NET_MULTIPLAY_NOCHANNEL: return "多画面预览中检测到通道数为0";
	case NET_TALK_INIT_ERROR: return "录音库初始化失败";
	case NET_TALK_NOT_INIT: return "录音库未经初始化";
	case NET_TALK_SENDDATA_ERROR: return "发送音频数据出错";
	case NET_REAL_ALREADY_SAVING: return "实时数据已经处于保存状态";
	case NET_NOT_SAVING: return "未保存实时数据";
	case NET_OPEN_FILE_ERROR: return "打开文件出错";
	case NET_PTZ_SET_TIMER_ERROR: return "启动云台控制定时器失败";
	case NET_RETURN_DATA_ERROR: return "对返回数据的校验出错";
	case NET_INSUFFICIENT_BUFFER: return "没有足够的缓存";
	case NET_NOT_SUPPORTED: return "当前SDK未支持该功能";
	case NET_NO_RECORD_FOUND: return "查询不到录象";
	case NET_NOT_AUTHORIZED: return "无操作权限";
	case NET_NOT_NOW: return "暂时无法执行";
	case NET_NO_TALK_CHANNEL: return "未发现对讲通道";
	case NET_NO_AUDIO: return "未发现音频";
	case NET_NO_INIT: return "网络SDK未经初始化";
	case NET_DOWNLOAD_END: return "下载已结束";
	case NET_EMPTY_LIST: return "查询结果为空";
	case NET_ERROR_GETCFG_SYSATTR: return "获取系统属性配置失败";
	case NET_ERROR_GETCFG_SERIAL: return "获取序列号失败";
	case NET_ERROR_GETCFG_GENERAL: return "获取常规属性失败";
	case NET_ERROR_GETCFG_DSPCAP: return "获取DSP能力描述失败";
	case NET_ERROR_GETCFG_NETCFG: return "获取网络配置失败";
	case NET_ERROR_GETCFG_CHANNAME: return "获取通道名称失败";
	case NET_ERROR_GETCFG_VIDEO: return "获取视频属性失败";
	case NET_ERROR_GETCFG_RECORD: return "获取录象配置失败";
	case NET_ERROR_GETCFG_PRONAME: return "获取解码器协议名称失败";
	case NET_ERROR_GETCFG_FUNCNAME: return "获取232串口功能名称失败";
	case NET_ERROR_GETCFG_485DECODER: return "获取解码器属性失败";
	case NET_ERROR_GETCFG_232COM: return "获取232串口配置失败";
	case NET_ERROR_GETCFG_ALARMIN: return "获取外部报警输入配置失败";
	case NET_ERROR_GETCFG_ALARMDET: return "获取动态检测报警失败";
	case NET_ERROR_GETCFG_SYSTIME: return "获取设备时间失败";
	case NET_ERROR_GETCFG_PREVIEW: return "获取预览参数失败";
	case NET_ERROR_GETCFG_AUTOMT: return "获取自动维护配置失败";
	case NET_ERROR_GETCFG_VIDEOMTRX: return "获取视频矩阵配置失败";
	case NET_ERROR_GETCFG_COVER: return "获取区域遮挡配置失败";
	case NET_ERROR_GETCFG_WATERMAKE: return "获取图象水印配置失败";
	case NET_ERROR_SETCFG_GENERAL: return "修改常规属性失败";
	case NET_ERROR_SETCFG_NETCFG: return "修改网络配置失败";
	case NET_ERROR_SETCFG_CHANNAME: return "修改通道名称失败";
	case NET_ERROR_SETCFG_VIDEO: return "修改视频属性失败";
	case NET_ERROR_SETCFG_RECORD: return "修改录象配置失败";
	case NET_ERROR_SETCFG_485DECODER: return "修改解码器属性失败";
	case NET_ERROR_SETCFG_232COM: return "修改232串口配置失败";
	case NET_ERROR_SETCFG_ALARMIN: return "修改外部输入报警配置失败";
	case NET_ERROR_SETCFG_ALARMDET: return "修改动态检测报警配置失败";
	case NET_ERROR_SETCFG_SYSTIME: return "修改设备时间失败";
	case NET_ERROR_SETCFG_PREVIEW: return "修改预览参数失败";
	case NET_ERROR_SETCFG_AUTOMT: return "修改自动维护配置失败";
	case NET_ERROR_SETCFG_VIDEOMTRX: return "修改视频矩阵配置失败";
	case NET_ERROR_SETCFG_COVER: return "修改区域遮挡配置失败";
	case NET_ERROR_SETCFG_WATERMAKE: return "修改图象水印配置失败";
	case NET_ERROR_SETCFG_WLAN: return "修改无线网络信息失败";
	case NET_ERROR_SETCFG_WLANDEV: return "选择无线网络设备失败";
	case NET_ERROR_SETCFG_REGISTER: return "修改主动注册参数配置失败";
	case NET_ERROR_SETCFG_CAMERA: return "修改摄像头属性配置失败";
	case NET_ERROR_SETCFG_INFRARED: return "修改红外报警配置失败";
	case NET_ERROR_SETCFG_SOUNDALARM: return "修改音频报警配置失败";
	case NET_ERROR_SETCFG_STORAGE: return "修改存储位置配置失败";
	case NET_AUDIOENCODE_NOTINIT: return "音频编码接口没有成功初始化";
	case NET_DATA_TOOLONGH: return "数据过长";
	case NET_UNSUPPORTED: return "设备不支持该操作";
	case NET_DEVICE_BUSY: return "设备资源不足";
	case NET_SERVER_STARTED: return "服务器已经启动";
	case NET_SERVER_STOPPED: return "服务器尚未成功启动";
	case NET_LISTER_INCORRECT_SERIAL: return "输入序列号有误";
	case NET_QUERY_DISKINFO_FAILED: return "获取硬盘信息失败";
	case NET_ERROR_GETCFG_SESSION: return "获取连接Session信息";
	case NET_USER_FLASEPWD_TRYTIME: return "输入密码错误超过限制次数";
	case NET_LOGIN_ERROR_PASSWORD: return "密码不正确";
	case NET_LOGIN_ERROR_USER: return "帐户不存在";
	case NET_LOGIN_ERROR_TIMEOUT: return "等待登录返回超时";
	case NET_LOGIN_ERROR_RELOGGIN: return "帐号已登录";
	case NET_LOGIN_ERROR_LOCKED: return "帐号已被锁定";
	case NET_LOGIN_ERROR_BLACKLIST: return "帐号已被列为黑名单";
	case NET_LOGIN_ERROR_BUSY: return "资源不足，系统忙";
	case NET_LOGIN_ERROR_CONNECT: return "登录设备超时，请检查网络并重试";
	case NET_LOGIN_ERROR_NETWORK: return "网络连接失败";
	case NET_LOGIN_ERROR_SUBCONNECT: return "登录设备成功，但无法创建视频通道，请检查网络状况";
	case NET_LOGIN_ERROR_MAXCONNECT: return "超过最大连接数";
	case NET_LOGIN_ERROR_PROTOCOL3_ONLY: return "只支持3代协议";
	case NET_LOGIN_ERROR_UKEY_LOST: return "未插入U盾或U盾信息错误";
	case NET_LOGIN_ERROR_NO_AUTHORIZED: return "客户端IP地址没有登录权限";
	case NET_RENDER_SOUND_ON_ERROR: return "Render库打开音频出错";
	case NET_RENDER_SOUND_OFF_ERROR: return "Render库关闭音频出错";
	case NET_RENDER_SET_VOLUME_ERROR: return "Render库控制音量出错";
	case NET_RENDER_ADJUST_ERROR: return "Render库设置画面参数出错";
	case NET_RENDER_PAUSE_ERROR: return "Render库暂停播放出错";
	case NET_RENDER_SNAP_ERROR: return "Render库抓图出错";
	case NET_RENDER_STEP_ERROR: return "Render库步进出错";
	case NET_RENDER_FRAMERATE_ERROR: return "Render库设置帧率出错";
	case NET_RENDER_DISPLAYREGION_ERROR: return "Render库设置显示区域出错";
	case NET_GROUP_EXIST: return "组名已存在";
	case NET_GROUP_NOEXIST: return "组名不存在";
	case NET_GROUP_RIGHTOVER: return "组的权限超出权限列表范围";
	case NET_GROUP_HAVEUSER: return "组下有用户，不能删除";
	case NET_GROUP_RIGHTUSE: return "组的某个权限被用户使用，不能出除";
	case NET_GROUP_SAMENAME: return "新组名同已有组名重复";
	case NET_USER_EXIST: return "用户已存在";
	case NET_USER_NOEXIST: return "用户不存在";
	case NET_USER_RIGHTOVER: return "用户权限超出组权限";
	case NET_USER_PWD: return "保留帐号，不容许修改密码";
	case NET_USER_FLASEPWD: return "密码不正确";
	case NET_USER_NOMATCHING: return "密码不匹配";
	case NET_USER_INUSE: return "账号正在使用中";
	case NET_ERROR_GETCFG_ETHERNET: return "获取网卡配置失败";
	case NET_ERROR_GETCFG_WLAN: return "获取无线网络信息失败";
	case NET_ERROR_GETCFG_WLANDEV: return "获取无线网络设备失败";
	case NET_ERROR_GETCFG_REGISTER: return "获取主动注册参数失败";
	case NET_ERROR_GETCFG_CAMERA: return "获取摄像头属性失败";
	case NET_ERROR_GETCFG_INFRARED: return "获取红外报警配置失败";
	case NET_ERROR_GETCFG_SOUNDALARM: return "获取音频报警配置失败";
	case NET_ERROR_GETCFG_STORAGE: return "获取存储位置配置失败";
	case NET_ERROR_GETCFG_MAIL: return "获取邮件配置失败";
	case NET_CONFIG_DEVBUSY: return "暂时无法设置";
	case NET_CONFIG_DATAILLEGAL: return "配置数据不合法";
	case NET_ERROR_GETCFG_DST: return "获取夏令时配置失败";
	case NET_ERROR_SETCFG_DST: return "设置夏令时配置失败";
	case NET_ERROR_GETCFG_VIDEO_OSD: return "获取视频OSD叠加配置失败";
	case NET_ERROR_SETCFG_VIDEO_OSD: return "设置视频OSD叠加配置失败";
	case NET_ERROR_GETCFG_GPRSCDMA: return "获取CDMA或GPRS网络配置失败";
	case NET_ERROR_SETCFG_GPRSCDMA: return "设置CDMA或GPRS网络配置失败";
	case NET_ERROR_GETCFG_IPFILTER: return "获取IP过滤配置失败";
	case NET_ERROR_SETCFG_IPFILTER: return "设置IP过滤配置失败";
	case NET_ERROR_GETCFG_TALKENCODE: return "获取语音对讲编码配置失败";
	case NET_ERROR_SETCFG_TALKENCODE: return "设置语音对讲编码配置失败";
	case NET_ERROR_GETCFG_RECORDLEN: return "获取录像打包长度配置失败";
	case NET_ERROR_SETCFG_RECORDLEN: return "设置录像打包长度配置失败";
	case NET_DONT_SUPPORT_SUBAREA: return "不支持网络硬盘分区";
	case NET_ERROR_GET_AUTOREGSERVER: return "获取设备上主动注册服务器信息失败";
	case NET_ERROR_CONTROL_AUTOREGISTER: return "主动注册重定向注册错误";
	case NET_ERROR_DISCONNECT_AUTOREGISTER: return "断开主动注册服务器错误";
	case NET_ERROR_GETCFG_MMS: return "获取mms配置失败";
	case NET_ERROR_SETCFG_MMS: return "设置mms配置失败";
	case NET_ERROR_GETCFG_SMSACTIVATION: return "获取短信激活无线连接配置失败";
	case NET_ERROR_SETCFG_SMSACTIVATION: return "设置短信激活无线连接配置失败";
	case NET_ERROR_GETCFG_DIALINACTIVATION: return "获取拨号激活无线连接配置失败";
	case NET_ERROR_SETCFG_DIALINACTIVATION: return "设置拨号激活无线连接配置失败";
	case NET_ERROR_GETCFG_VIDEOOUT: return "查询视频输出参数配置失败";
	case NET_ERROR_SETCFG_VIDEOOUT: return "设置视频输出参数配置失败";
	case NET_ERROR_GETCFG_OSDENABLE: return "获取osd叠加使能配置失败";
	case NET_ERROR_SETCFG_OSDENABLE: return "设置osd叠加使能配置失败";
	case NET_ERROR_SETCFG_ENCODERINFO: return "设置数字通道前端编码接入配置失败";
	case NET_ERROR_GETCFG_TVADJUST: return "获取TV调节配置失败";
	case NET_ERROR_SETCFG_TVADJUST: return "设置TV调节配置失败";
	case NET_ERROR_CONNECT_FAILED: return "请求建立连接失败";
	case NET_ERROR_SETCFG_BURNFILE: return "请求刻录文件上传失败";
	case NET_ERROR_SNIFFER_GETCFG: return "获取抓包配置信息失败";
	case NET_ERROR_SNIFFER_SETCFG: return "设置抓包配置信息失败";
	case NET_ERROR_DOWNLOADRATE_GETCFG: return "查询下载限制信息失败";
	case NET_ERROR_DOWNLOADRATE_SETCFG: return "设置下载限制信息失败";
	case NET_ERROR_SEARCH_TRANSCOM: return "查询串口参数失败";
	case NET_ERROR_GETCFG_POINT: return "获取预制点信息错误";
	case NET_ERROR_SETCFG_POINT: return "设置预制点信息错误";
	case NET_SDK_LOGOUT_ERROR: return "SDK没有正常登出设备";
	case NET_ERROR_GET_VEHICLE_CFG: return "获取车载配置失败";
	case NET_ERROR_SET_VEHICLE_CFG: return "设置车载配置失败";
	case NET_ERROR_GET_ATM_OVERLAY_CFG: return "获取atm叠加配置失败";
	case NET_ERROR_SET_ATM_OVERLAY_CFG: return "设置atm叠加配置失败";
	case NET_ERROR_GET_ATM_OVERLAY_ABILITY: return "获取atm叠加能力失败";
	case NET_ERROR_GET_DECODER_TOUR_CFG: return "获取解码器解码轮巡配置失败";
	case NET_ERROR_SET_DECODER_TOUR_CFG: return "设置解码器解码轮巡配置失败";
	case NET_ERROR_CTRL_DECODER_TOUR: return "控制解码器解码轮巡失败";
	case NET_GROUP_OVERSUPPORTNUM: return "超出设备支持最大用户组数目";
	case NET_USER_OVERSUPPORTNUM: return "超出设备支持最大用户数目";
	case NET_ERROR_GET_SIP_CFG: return "获取SIP配置失败";
	case NET_ERROR_SET_SIP_CFG: return "设置SIP配置失败";
	case NET_ERROR_GET_SIP_ABILITY: return "获取SIP能力失败";
	case NET_ERROR_GET_WIFI_AP_CFG: return "获取WIFI ap配置失败";
	case NET_ERROR_SET_WIFI_AP_CFG: return "设置WIFI ap配置失败";
	case NET_ERROR_GET_DECODE_POLICY: return "获取解码策略配置失败";
	case NET_ERROR_SET_DECODE_POLICY: return "设置解码策略配置失败";
	case NET_ERROR_TALK_REJECT: return "拒绝对讲";
	case NET_ERROR_TALK_OPENED: return "对讲被其他客户端打开";
	case NET_ERROR_TALK_RESOURCE_CONFLICIT: return "资源冲突";
	case NET_ERROR_TALK_UNSUPPORTED_ENCODE: return "不支持的语音编码格式";
	case NET_ERROR_TALK_RIGHTLESS: return "无权限";
	case NET_ERROR_TALK_FAILED: return "请求对讲失败";
	case NET_ERROR_GET_MACHINE_CFG: return "获取机器相关配置失败";
	case NET_ERROR_SET_MACHINE_CFG: return "设置机器相关配置失败";
	case NET_ERROR_GET_DATA_FAILED: return "设备无法获取当前请求数据";
	case NET_ERROR_MAC_VALIDATE_FAILED: return "MAC地址验证失败";
	case NET_ERROR_GET_INSTANCE: return "获取服务器实例失败";
	case NET_ERROR_JSON_REQUEST: return "生成的jason字符串错误";
	case NET_ERROR_JSON_RESPONSE: return "响应的jason字符串错误";
	case NET_ERROR_VERSION_HIGHER: return "协议版本低于当前使用的版本";
	case NET_SPARE_NO_CAPACITY: return "热备操作失败, 容量不足";
	case NET_ERROR_SOURCE_IN_USE: return "显示源被其他输出占用";
	case NET_ERROR_REAVE: return "高级用户抢占低级用户资源";
	case NET_ERROR_NETFORBID: return "禁止入网";
	case NET_ERROR_GETCFG_MACFILTER: return "获取MAC过滤配置失败";
	case NET_ERROR_SETCFG_MACFILTER: return "设置MAC过滤配置失败";
	case NET_ERROR_GETCFG_IPMACFILTER: return "获取IP/MAC过滤配置失败";
	case NET_ERROR_SETCFG_IPMACFILTER: return "设置IP/MAC过滤配置失败";
	case NET_ERROR_OPERATION_OVERTIME: return "当前操作超时";
	case NET_ERROR_SENIOR_VALIDATE_FAILED: return "高级校验失败";
	case NET_ERROR_DEVICE_ID_NOT_EXIST: return "设备ID不存在";
	case NET_ERROR_UNSUPPORTED: return "不支持当前操作";
	case NET_ERROR_PROXY_DLLLOAD: return "代理库加载失败";
	case NET_ERROR_PROXY_ILLEGAL_PARAM: return "代理用户参数不合法";
	case NET_ERROR_PROXY_INVALID_HANDLE: return "代理句柄无效";
	case NET_ERROR_PROXY_LOGIN_DEVICE_ERROR: return "代理登入前端设备失败";
	case NET_ERROR_PROXY_START_SERVER_ERROR: return "启动代理服务失败";
	case NET_ERROR_SPEAK_FAILED: return "请求喊话失败";
	case NET_ERROR_NOT_SUPPORT_F6: return "设备不支持此F6接口调用";
	default: return "未知错误";
	}
}

// #include "catch.hpp"
// 
// TEST_CASE_METHOD(DHVendor, "Init DH SDK", "[DHVendor]")
//  {
// 
// 	time_range range;
// 	range.start = 1467302400;
// 	//range.end = 1466524800;
// 	range.end = 1467648000;
// 	//range.end = 1478833871;
// 	REQUIRE_NOTHROW(Init());
// 	REQUIRE_NOTHROW(StartSearchDevice());
// 	
// 	REQUIRE_NOTHROW(Login("192.168.0.96", 37777, "admin", ""));
// 
// 	REQUIRE_NOTHROW(Search(0, 0, range));
// 	//REQUIRE_NOTHROW(Search(1, range));
// 
// 	//REQUIRE_NOTHROW(Download(0, 0, range));
// 	REQUIRE_NOTHROW(Download(0, 0, "channel0-20160701000000-20160701235959-0"));
// 	//REQUIRE_NOTHROW(Download(0, 0, "channel0-20160701000000-20160701235959-1"));
// 
// 	//REQUIRE_NOTHROW(PlayVideo(0, range));
// 	//REQUIRE_NOTHROW(PlayVideo(0, "channel0-20160621000000-20160621235959-0"));
// 	//REQUIRE_NOTHROW(Logout());
// 
// }
