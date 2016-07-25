#include "HKVendor.h"

// HK SDK
#include "HCNetSDK.h"

#pragma comment(lib, "HCNetSDK")

class HK_SDK_INTERFACE
{
public:
	static void HK_timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd);
	static void HK_timeStdToDH(tm *pTimeStd, NET_DVR_TIME *pTimeDH);
	static void HK_trTOndt(NET_DVR_TIME &ndtStartTime, NET_DVR_TIME &ndtEndTime, const time_range range);

	static std::string HK_GetLastErrorString();

	static std::string HK_MakeStrByInteger(int data);
	static std::string HK_MakeStrTimeByTimestamp(time_t time);

	static bool HK_isGetDVRConfig(const long loginHandle);
	static size_t HK_getChannel(const long loginHandle, size_t channel);
};


HKVendor::HKVendor()
{
	m_eSDKType = HK_SDK;
	m_sDefUserName = "admin";
	m_sDefPassword = "12345";
	m_iMaxChannel = 0;
	m_bSearchDeviceAPI = true;

	m_lSearchDeviceHandle = -1;

	m_sRoot = "D:\\DownLoadVideo\\";
}

HKVendor::~HKVendor()
{
	m_iMaxChannel = 0;
	m_lSearchDeviceHandle = -1;

	assert(NET_DVR_Cleanup());
}

void HKVendor::Init()
{
	BOOL bInit = NET_DVR_Init();

	if (!bInit)
	{
		std::cout << "The HK SDK initialization error：" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Init failed");
	}
	else
	{
		std::cout << "The HK SDK initialization success!" << std::endl;
	}

	BOOL bConnect = NET_DVR_SetConnectTime(1000, 30);
	BOOL bReConnect = NET_DVR_SetReconnect(3000);
	assert(bConnect);
	assert(bReConnect);
}

long HKVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID = -1;
	lLoginID  = NET_DVR_Login_V30((char *)ip.c_str(), port, (char *)(user.c_str()), (char *)(password.c_str()), &DeviceInfoTmp);
	
	if (-1 == lLoginID)
	{
		std::cout << "The HK SDK login failed:" << HK_SDK_INTERFACE::HK_GetLastErrorString().c_str() << std::endl;
		//throw std::exception("Login failed");
		return -1;
	}

	m_iMaxChannel = DeviceInfoTmp.byIPChanNum;

	return lLoginID;
}

void HKVendor::Logout(const long loginHandle)
{
	if (loginHandle < 0 && !NET_DVR_Logout(loginHandle))
	{
		std::cout << "The HK SDK logout failed:" << HK_SDK_INTERFACE::HK_GetLastErrorString().c_str() << std::endl;
		throw std::exception("Logout failed");
		return;
	}
}

void HKVendor::SearchAll(const long loginHandle)
{

}

void HKVendor::ClearLocalRecordFiles()
{
	m_files.clear();
}
	

void HKVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	if (-1 == loginHandle)
	{
		std::cout << "Please login first!" << std::endl;
		return;
	}

	if (range.start >= range.end)
	{
		std::cout << "Time range is wrong!" << std::endl;
		return;
	}

	if (!m_FilesChange.empty())
	{
		m_FilesChange.clear();
	}

	size_t sChannel = HK_SDK_INTERFACE::HK_getChannel(loginHandle, channel);

	NET_DVR_TIME ndtStartTime;
	NET_DVR_TIME ndtEndTime;

	HK_SDK_INTERFACE::HK_trTOndt(ndtStartTime, ndtEndTime, range);

	NET_DVR_FILECOND FileCond;
	FileCond.lChannel = sChannel;
	FileCond.dwFileType = 0xff;
	FileCond.dwIsLocked = 0xff;
	FileCond.dwUseCardNo = 0;
	FileCond.struStartTime = ndtStartTime;
	FileCond.struStopTime = ndtEndTime;

	LONG hFindHandle = NET_DVR_FindFile_V30(loginHandle, &FileCond);

	if (-1 == hFindHandle)
	{
		std::cout << "search error:" << HK_SDK_INTERFACE::HK_GetLastErrorString() << endl;
		return;
	}
	else
	{
		NET_DVR_FINDDATA_V30 FindData;  //The find file information
		RecordFile rf;

		int ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
		tm tmStart;
		tm tmEnd;

		while (ret > 0)
		{

			if (NET_DVR_FILE_EXCEPTION == ret)
			{
				std::cout << "Find file Abnormally:" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
				throw SearchFileException(HK_SDK_INTERFACE::HK_GetLastErrorString().c_str());
				break;
			}
			else if (NET_DVR_FILE_NOFIND == ret)
			{
				std::cout << "No video file!" << std::endl;
				break;

			}
			else if (NET_DVR_NOMOREFILE == ret)   //End of the query
			{
				std::cout << "End of the query!" << std::endl;
				break;
			}
			else if (NET_DVR_ISFINDING == ret)  //Finding
			{
				ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
			}
			else if (NET_DVR_FILE_SUCCESS == ret)  //File information successfully
			{
				HK_SDK_INTERFACE::HK_timeDHToStd(&FindData.struStartTime, &tmStart);
				HK_SDK_INTERFACE::HK_timeDHToStd(&FindData.struStopTime, &tmEnd);


				//Save file information
				rf.name = FindData.sFileName;
				rf.channel = channel;
				rf.size = FindData.dwFileSize / 1024 / 1024;

				rf.beginTime = mktime(&tmStart);
				rf.endTime = mktime(&tmEnd);

				m_FilesChange.push_back(rf);
				m_files.push_back(rf);

				ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);

				std::cout << "FileName:" << rf.name << endl << "  " << "FileSize:" << rf.size << "  " << "Channel:" << rf.channel << endl;
			}
		}

		//Close the lookup, release the handle
		NET_DVR_FindClose_V30(hFindHandle);
	}

	// Save Search Video List Result to Config File
	CCommonUtrl::getInstance().SaveSearchFileListToFile(m_FilesChange, Vendor_HK_Abbr);

	// Write File List to DB
	CCommonUtrl::getInstance().WriteFileListToDB(m_FilesChange);
}

void HKVendor::Download(const long loginHandle, const RecordFile& file)
{
	if (-1 == loginHandle)
	{
		std::cout << "Please login frist!" << std::endl;
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

	std::string strFileName = CCommonUtrl::getInstance().MakeFileName(file.channel, strTimeStart, strTimeEnd, ".mp4");

	std::string strPath = CCommonUtrl::getInstance().MakeDownloadFileFolder(m_sRoot, strTimeStartZero, strTimeEndZero, Vendor_HK, file.channel);

	strPath += strFileName;

	LONG lRet = NET_DVR_GetFileByName(loginHandle, (char *)file.name.c_str(), (char *)strPath.c_str());

	if (0 < lRet)
	{
		std::cout << "Download videos failed：" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Download by Record file failed");
		return;
	}
	else
	{
		NET_DVR_PlayBackControl(lRet, NET_DVR_PLAYSTART, 0, NULL);

		// 				DWORD pos;
		// 				pos = NET_DVR_GetDownloadPos(lRet);
		// 
		// 				while (pos < 100)
		// 				{
		// 					pos = NET_DVR_GetDownloadPos(lRet);
		// 					std::cout << "Pos:" << pos << std::endl;
		// 				}


		std::cout << "Download videos success!" << std::endl;
	}
}

void HKVendor::PlayVideo(const long loginHandle, const RecordFile& file)
{
	if (-1 == loginHandle)
	{
		std::cout << "Streaming video to fail：" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Login handle by Record file failed");
		return;
	}

	NET_DVR_TIME ndtStime;
	NET_DVR_TIME ndtEtime;
	time_range range;
	range.start = file.beginTime;
	range.end = file.endTime;

	HK_SDK_INTERFACE::HK_trTOndt(ndtStime, ndtEtime, range);

	TestWindows Test;
	Test.Init();

	size_t sChannel = HK_SDK_INTERFACE::HK_getChannel(loginHandle, file.channel);
	LONG hPlayHandle = NET_DVR_PlayBackByTime(loginHandle, sChannel, &ndtStime, &ndtEtime, Test.m_hWnd);

	if (hPlayHandle < 0)
	{
		std::cout << "Play the failure：" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}

	if (!NET_DVR_PlayBackControl(hPlayHandle, NET_DVR_PLAYSTART, NULL, 0))
	{
		std::cout << "Play the failure：" << HK_SDK_INTERFACE::HK_GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}

	system("PAUSE");
}

bool HKVendor::StopDownload()
{
	return true;
}

void HKVendor::SetDownloadPath(const std::string& Root)
{
	m_sRoot = Root;
}

void HKVendor::throwException()
{

}

void HK_SDK_INTERFACE::HK_timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd)
{
	pTimeStd->tm_year = pTimeDH->dwYear - 1900;
	pTimeStd->tm_mon = pTimeDH->dwMonth - 1;
	pTimeStd->tm_mday = pTimeDH->dwDay;
	pTimeStd->tm_hour = pTimeDH->dwHour;
	pTimeStd->tm_min = pTimeDH->dwMinute;
	pTimeStd->tm_sec = pTimeDH->dwSecond;
}

void HK_SDK_INTERFACE::HK_trTOndt(NET_DVR_TIME &ndtStartTime, NET_DVR_TIME &ndtEndTime, const time_range range)
{
	tm tmStartTime;
	tm tmEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);

	HK_SDK_INTERFACE::HK_timeStdToDH(&tmStartTime, &ndtStartTime);
	HK_SDK_INTERFACE::HK_timeStdToDH(&tmEndTime, &ndtEndTime);
}

void HK_SDK_INTERFACE::HK_timeStdToDH(tm *pTimeStd, NET_DVR_TIME *pTimeDH)
{
	pTimeDH->dwYear = pTimeStd->tm_year + 1900;
	pTimeDH->dwMonth = pTimeStd->tm_mon + 1;
	pTimeDH->dwDay = pTimeStd->tm_mday;
	pTimeDH->dwHour = pTimeStd->tm_hour;
	pTimeDH->dwMinute = pTimeStd->tm_min;
	pTimeDH->dwSecond = pTimeStd->tm_sec;
}

std::string HK_SDK_INTERFACE::HK_MakeStrTimeByTimestamp(time_t time)
{
	char cTime[50];
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime(cTime, 50, "%Y%m%d%H%M%S", &ttime);

	string strTime(cTime);

	return strTime;
}
std::string HK_SDK_INTERFACE::HK_MakeStrByInteger(int data)
{
	char cData[50];

	sprintf_s(cData, 50, "%d", data);

	string strTime(cData);

	return strTime;
}

bool HK_SDK_INTERFACE::HK_isGetDVRConfig(const long loginHandle)
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg;
	memset(&IpAccessCfg, 0, sizeof(IpAccessCfg));
	DWORD  dwReturned;

	return  NET_DVR_GetDVRConfig(loginHandle, NET_DVR_GET_IPPARACFG_V40, 0, &IpAccessCfg, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned);
}

size_t HK_SDK_INTERFACE::HK_getChannel(const long loginHandle, size_t channel)
{
	//获取设备的配置信息成功就启用模拟通道
	if (HK_isGetDVRConfig(loginHandle))
	{
		return channel += 33;
	}
	else
	{
		return channel + 1;
	}
}

void HKVendor::StartSearchDevice()
{

}

void HKVendor::StopSearchDevice()
{

}

bool HKVendor::IsSearchDeviceAPIExist()
{
	return m_bSearchDeviceAPI;
}


std::string HK_SDK_INTERFACE::HK_GetLastErrorString()
{
	DWORD dwError;
	dwError = NET_DVR_GetLastError();

	switch (dwError)
	{
	case NET_DVR_NOERROR: return "NOERROR";
	case NET_DVR_PASSWORD_ERROR: return "用户名密码错误";
	case NET_DVR_NOENOUGHPRI: return "权限不足";
	case NET_DVR_NOINIT: return "没有初始化";
	case NET_DVR_CHANNEL_ERROR: return "通道号错误";
	case NET_DVR_OVER_MAXLINK: return "连接到DVR的客户端个数超过最大";
	case NET_DVR_VERSIONNOMATCH: return "版本不匹配";
	case NET_DVR_NETWORK_FAIL_CONNECT: return "连接服务器失败";
	case NET_DVR_NETWORK_SEND_ERROR: return "向服务器发送失败";
	case NET_DVR_NETWORK_RECV_ERROR: return "从服务器接收数据失败";
	case NET_DVR_NETWORK_RECV_TIMEOUT: return "从服务器接收数据超时";
	case NET_DVR_NETWORK_ERRORDATA: return "传送的数据有误";
	case NET_DVR_ORDER_ERROR: return "调用次序错误";
	case NET_DVR_OPERNOPERMIT: return "无此权限";
	case NET_DVR_COMMANDTIMEOUT: return "DVR命令执行超时";
	case NET_DVR_ERRORSERIALPORT: return "串口号错误";
	case NET_DVR_ERRORALARMPORT: return "报警端口错误";
	case NET_DVR_PARAMETER_ERROR: return "参数错误";
	case NET_DVR_CHAN_EXCEPTION: return "服务器通道处于错误状态";
	case NET_DVR_NODISK: return "没有硬盘";
	case NET_DVR_ERRORDISKNUM: return "硬盘号错误";
	case NET_DVR_DISK_FULL: return "服务器硬盘满";
	case NET_DVR_DISK_ERROR: return "服务器硬盘出错";
	case NET_DVR_NOSUPPORT: return "服务器不支持";
	case NET_DVR_BUSY: return "服务器忙";
	case NET_DVR_MODIFY_FAIL: return "服务器修改不成功";
	case NET_DVR_PASSWORD_FORMAT_ERROR: return "密码输入格式不正确";
	case NET_DVR_DISK_FORMATING: return "硬盘正在格式化，不能启动操作";
	case NET_DVR_DVRNORESOURCE: return "DVR资源不足";
	case NET_DVR_DVROPRATEFAILED: return "DVR操作失败";
	case NET_DVR_OPENHOSTSOUND_FAIL: return "打开PC声音失败";
	case NET_DVR_DVRVOICEOPENED: return "服务器语音对讲被占用";
	case NET_DVR_TIMEINPUTERROR: return "时间输入不正确";
	case NET_DVR_NOSPECFILE: return "回放时服务器没有指定的文件";
	case NET_DVR_CREATEFILE_ERROR: return "创建文件出错";
	case NET_DVR_FILEOPENFAIL: return "打开文件出错";
	case NET_DVR_OPERNOTFINISH: return "上次的操作还没有完成";
	case NET_DVR_GETPLAYTIMEFAIL: return "获取当前播放的时间出错";
	case NET_DVR_PLAYFAIL: return "播放出错";
	case NET_DVR_FILEFORMAT_ERROR: return "文件格式不正确";
	case NET_DVR_DIR_ERROR: return "路径错误";
	case NET_DVR_ALLOC_RESOURCE_ERROR: return "资源分配错误";
	case NET_DVR_AUDIO_MODE_ERROR: return "声卡模式错误";
	case NET_DVR_NOENOUGH_BUF: return "缓冲区太小";
	case NET_DVR_CREATESOCKET_ERROR: return "创建SOCKET出错";
	case NET_DVR_SETSOCKET_ERROR: return "设置SOCKET出错";
	case NET_DVR_MAX_NUM: return "个数达到最大";
	case NET_DVR_USERNOTEXIST: return "用户不存在";
	case NET_DVR_WRITEFLASHERROR: return "写FLASH出错";
	case NET_DVR_UPGRADEFAIL: return "DVR升级失败";
	case NET_DVR_CARDHAVEINIT: return "解码卡已经初始化过";
	case NET_DVR_PLAYERFAILED: return "调用播放库中某个函数失败";
	case NET_DVR_MAX_USERNUM: return "设备端用户数达到最大";
	case NET_DVR_GETLOCALIPANDMACFAIL: return "获得客户端的IP地址或物理地址失败";
	case NET_DVR_NOENCODEING: return "该通道没有编码";
	case NET_DVR_IPMISMATCH: return "IP地址不匹配";
	case NET_DVR_MACMISMATCH: return "MAC地址不匹配";
	case NET_DVR_UPGRADELANGMISMATCH: return "升级文件语言不匹配";
	case NET_DVR_MAX_PLAYERPORT: return "播放器路数达到最大";
	case NET_DVR_NOSPACEBACKUP: return "备份设备中没有足够空间进行备份";
	case NET_DVR_NODEVICEBACKUP: return "没有找到指定的备份设备";
	case NET_DVR_PICTURE_BITS_ERROR: return "图像素位数不符，限24色";
	case NET_DVR_PICTURE_DIMENSION_ERROR: return "图片高*宽超限， 限128*256";
	case NET_DVR_PICTURE_SIZ_ERROR: return "图片大小超限，限100K";
	case NET_DVR_LOADPLAYERSDKFAILED: return "载入当前目录下Player Sdk出错";
	case NET_DVR_LOADPLAYERSDKPROC_ERROR: return "找不到Player Sdk中某个函数入口";
	case NET_DVR_LOADDSSDKFAILED: return "载入当前目录下DSsdk出错";
	case NET_DVR_LOADDSSDKPROC_ERROR: return "找不到DsSdk中某个函数入口";
	case NET_DVR_DSSDK_ERROR: return "调用硬解码库DsSdk中某个函数失败";
	case NET_DVR_VOICEMONOPOLIZE: return "声卡被独占";
	case NET_DVR_JOINMULTICASTFAILED: return "加入多播组失败";
	case NET_DVR_CREATEDIR_ERROR: return "建立日志文件目录失败";
	case NET_DVR_BINDSOCKET_ERROR: return "绑定套接字失败";
	case NET_DVR_SOCKETCLOSE_ERROR: return "socket连接中断，此错误通常是由于连接中断或目的地不可达";
	case NET_DVR_USERID_ISUSING: return "注销时用户ID正在进行某操作";
	case NET_DVR_SOCKETLISTEN_ERROR: return "监听失败";
	case NET_DVR_PROGRAM_EXCEPTION: return "程序异常";
	case NET_DVR_WRITEFILE_FAILED: return "写文件失败";
	case NET_DVR_FORMAT_READONLY: return "禁止格式化只读硬盘";
	case NET_DVR_WITHSAMEUSERNAME: return "用户配置结构中存在相同的用户名";
	case NET_DVR_DEVICETYPE_ERROR: return "导入参数时设备型号不匹配";
	case NET_DVR_LANGUAGE_ERROR: return "导入参数时语言不匹配";
	case NET_DVR_PARAVERSION_ERROR: return "导入参数时软件版本不匹配";
	case NET_DVR_IPCHAN_NOTALIVE: return "预览时外接IP通道不在线";
	case NET_DVR_RTSP_SDK_ERROR: return "加载高清IPC通讯库StreamTransClient.dll失败";
	case NET_DVR_CONVERT_SDK_ERROR: return "加载转码库失败";
	case NET_DVR_IPC_COUNT_OVERFLOW: return "超出最大的ip接入通道数";
	case NET_DVR_MAX_ADD_NUM: return "添加标签(一个文件片段64)等个数达到最大";
	case NET_DVR_PARAMMODE_ERROR: return "图像增强仪，参数模式错误（用于硬件设置时，客户端进行软件设置时错误值）";
	case NET_DVR_CODESPITTER_OFFLINE: return "视频综合平台，码分器不在线";
	case NET_DVR_BACKUP_COPYING: return "设备正在备份";
	case NET_DVR_CHAN_NOTSUPPORT: return "通道不支持该操作";
	case NET_DVR_CALLINEINVALID: return "高度线位置太集中或长度线不够倾斜";
	case NET_DVR_CALCANCELCONFLICT: return "取消标定冲突，如果设置了规则及全局的实际大小尺寸过滤";
	case NET_DVR_CALPOINTOUTRANGE: return "标定点超出范围";
	case NET_DVR_FILTERRECTINVALID: return "尺寸过滤器不符合要求";
	case NET_DVR_DDNS_DEVOFFLINE: return "设备没有注册到ddns上";
	case NET_DVR_DDNS_INTER_ERROR: return "DDNS 服务器内部错误";
	case NET_DVR_FUNCTION_NOT_SUPPORT_OS: return "此功能不支持该操作系统";
	case NET_DVR_DEC_CHAN_REBIND: return "解码通道绑定显示输出次数受限";
	case NET_DVR_INTERCOM_SDK_ERROR: return "加载当前目录下的语音对讲库失败";
	case NET_DVR_NO_CURRENT_UPDATEFILE: return "没有正确的升级包";
	case NET_DVR_USER_NOT_SUCC_LOGIN: return "用户还没登陆成功";
	case NET_DVR_USE_LOG_SWITCH_FILE: return "正在使用日志开关文件";
	case NET_DVR_POOL_PORT_EXHAUST: return "端口池中用于绑定的端口已耗尽";
	case NET_DVR_PACKET_TYPE_NOT_SUPPORT: return "码流封装格式错误";
	case NET_DVR_ALIAS_DUPLICATE: return "别名重复";
	default:
		return "未知错误";
	}
}