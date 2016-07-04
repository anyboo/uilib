#include <fstream>
#include "HKVendor.h"
#include "TestWindows.h"

HKVendor::HKVendor():
m_strName(""),
m_dwPort(0),
m_strUser(""),
m_strPasswords(""),
m_strIP(""),
m_lLoginHandle(0),
m_nChannels(0)
{
}


HKVendor::~HKVendor()
{
	assert(NET_DVR_Cleanup());
}

void HKVendor::Init()
{
	BOOL bInit = NET_DVR_Init();

	if (!bInit)
	{
		std::cout << "初始化错误：" << GetLastErrorString() << std::endl;
		throw std::exception("Init failed");
	}
	else
	{
		std::cout << "初始化SDK成功！" << std::endl;
	}

	BOOL bConnect = NET_DVR_SetConnectTime(1000, 30);
	BOOL bReConnect = NET_DVR_SetReconnect(3000);
	assert(bConnect);
	assert(bReConnect);
}

long HKVendor::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	m_strIP = ip;
	m_dwPort = port;
	m_strUser = user;
	m_strPasswords = password;

	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID = NET_DVR_Login_V30((char *)m_strIP.c_str(), m_dwPort, (char *)(m_strUser.c_str()), (char *)(m_strPasswords.c_str()), &DeviceInfoTmp);
	m_lLoginHandle = lLoginID;
	if (-1 == lLoginID)
	{
		std::cout << "登录失败(Failed)：" << GetLastErrorString().c_str() << std::endl;
		throw std::exception("Login failed");
	}

	m_nChannels = DeviceInfoTmp.byIPChanNum;

	return lLoginID;
}

void HKVendor::Logout(const long loginHandle)
{
	if (m_lLoginHandle > 0 && !NET_DVR_Logout(m_lLoginHandle))
	{
		std::cout << "退出失败：" << GetLastErrorString().c_str() << std::endl;
		throw std::exception("Logout failed");
		return;
	}
}

void HKVendor::SearchAll(const long loginHandle)
{

}

void HKVendor::Search(const long loginHandle, const size_t channel, const time_range& range)
{
	if (-1 == m_lLoginHandle)
	{
		std::cout << "请先登录!" << std::endl;
		return;
	}

	if (range.start >= range.end)
	{
		std::cout << "时间范围不对!" << std::endl;
		return;
	}

	std::vector<time_range> trInfor = MakeTimeRangeList(range);

	std::vector<time_range>::iterator it;
	for (it = trInfor.begin(); it != trInfor.end(); ++it)
	{
		NET_DVR_TIME ndtStartTime;
		NET_DVR_TIME ndtEndTime;

		trTOndt(ndtStartTime, ndtEndTime, *it);

		NET_DVR_FILECOND FileCond;
		FileCond.lChannel = channel;
		FileCond.dwFileType = 0xff;
		FileCond.dwIsLocked = 0xff;
		FileCond.dwUseCardNo = 0;
		FileCond.struStartTime = ndtStartTime;
		FileCond.struStopTime = ndtEndTime;

		LONG hFindHandle = NET_DVR_FindFile_V30(m_lLoginHandle, &FileCond);
		if (-1 == hFindHandle)
		{
			std::cout << "Error:"<<GetLastErrorString() << endl;
				return;
		}
		else
		{
			NET_DVR_FINDDATA_V30 FindData;  //查找到的文件信息
			RecordFile rf;

			int ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
			//int ItemIndex = 0;
			tm tmStart;
			tm tmEnd;
			
			while (ret > 0)
			{

				if (NET_DVR_FILE_EXCEPTION == ret)
				{
					std::cout << "查找文件时异常!" << GetLastErrorString() << std::endl;
					throw std::exception("SearchFile unusual failed");
					break;
				}
				else if (NET_DVR_FILE_NOFIND == ret)
				{
					std::cout << "没有录像文件" << std::endl;
					break;

				}
				else if (NET_DVR_NOMOREFILE == ret)   //查找结束
				{
					std::cout << "查询结束！" << std::endl;
					break;
				}
				else if (NET_DVR_ISFINDING == ret)  //正在查找
				{
					ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);
					//Sleep(5);
				}
				else if (NET_DVR_FILE_SUCCESS == ret)  //获取文件信息成功
				{
					timeDHToStd(&FindData.struStartTime, &tmStart);
					timeDHToStd(&FindData.struStopTime, &tmEnd);


					//保存文件信息
					rf.name = FindData.sFileName;
					rf.channel = channel;
					rf.size = FindData.dwFileSize / 1024 / 1024;
					
					rf.beginTime = mktime(&tmStart);
					rf.endTime = mktime(&tmEnd);

					m_files.push_back(rf);

					ret = NET_DVR_FindNextFile_V30(hFindHandle, &FindData);

					std::cout << "GetRecordFileList 文件名:" << rf.name <<endl<< "  " << "文件大小:" << rf.size << "  " << "通道:" << rf.channel << endl;

				}
			}
			//关闭查找，释放句柄
			NET_DVR_FindClose_V30(hFindHandle);
		}

	}

	//保存到JSON文件
	SaveSearchFileListToFile();
	//写入到数据库中
	WriteFileListToDB();
}

void HKVendor::Download(const long loginHandle, const size_t channel, const time_range& range)
{
	if (-1 == m_lLoginHandle)
	{
		std::cout << "请先登录!" << std::endl;
		return;
	}

 	NET_DVR_TIME ndtStime;
 	NET_DVR_TIME ndtEtime;
 
	trTOndt(ndtStime, ndtEtime, range);

 
 	CreatePath(channel);
 
 	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\海康\\通道%d\\channel%d-%d%02d%02d%02d%02d%02d-%d%02d%02d%02d%02d%02d.mp4", channel, channel, ndtStime.dwYear, ndtStime.dwMonth, ndtStime.dwDay,
		ndtStime.dwHour, ndtStime.dwMinute, ndtStime.dwSecond, ndtEtime.dwYear, ndtEtime.dwMonth, ndtEtime.dwDay, ndtEtime.dwHour, ndtEtime.dwMinute, ndtEtime.dwSecond);
	std::cout << "strName:" << szTime << std::endl;

	LONG lRet = NET_DVR_GetFileByTime(m_lLoginHandle, channel, &ndtStime, &ndtEtime, szTime);
	
	if (-1 == lRet)
	{
		
		std::cout << "downLoadByRecordFile 下载录像失败，错误原因：" << GetLastErrorString() << std::endl;
		throw std::exception("Download by Record file failed");

		return;
	}
	else
	{
		NET_DVR_PlayBackControl(lRet, NET_DVR_PLAYSTART, 0, NULL);
		std::cout << "downLoadByRecordFile 下载文件成功！" << std::endl;
	}	
}

void HKVendor::PlayVideo(const long loginHandle, const size_t channel, const time_range& range)
{
	if (-1 == m_lLoginHandle)
	{
		std::cout << "PlayVideo 在线播放失败原因：" << GetLastErrorString() << std::endl;
		throw std::exception("Login handle by Record file failed");
		return;
	}

	NET_DVR_TIME ndtStime;
	NET_DVR_TIME ndtEtime;

	trTOndt(ndtStime, ndtEtime, range);

	TestWindows Test;
	Test.Init();

	//BOOL lPlayID = CLIENT_PlayBackByTimeEx(m_lLoginHandle, channel, &ntStime, &ntEtime, g_hWnd, PlayCallBack, (DWORD)this, PBDataCallBack, (DWORD)this);
	LONG hPlayHandle = NET_DVR_PlayBackByTime(m_lLoginHandle, channel, &ndtStime, &ndtEtime, g_hWnd);

	if (hPlayHandle < 0)
	{
		std::cout << "播放失败原因：" << GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}

	if (!NET_DVR_PlayBackControl(hPlayHandle, NET_DVR_PLAYSTART, NULL, 0))
	{
		std::cout << "播放失败原因：" << GetLastErrorString() << std::endl;
		throw std::exception("Play back by time failed");
	}

	system("PAUSE");
}

void HKVendor::Download(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (-1 == m_lLoginHandle)
	{
		std::cout << "请先登录!" << std::endl;
		return;
	}

	CreatePath(channel);

	char szTime[512];
	ZeroMemory(szTime, 512);
	sprintf_s(szTime, "D:\\DownLoadVideo\\海康\\通道%d\\", channel);

	char szBuf[] = ".mp4";
	strcat_s(szTime, (char *)filename.c_str());
	strcat_s(szTime, szBuf);

	cout << szTime << endl;

	std::vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{	
			LONG lRet = NET_DVR_GetFileByName(m_lLoginHandle, (char *)filename.c_str(), szTime);

			if (0 < lRet)
			{
				std::cout << "downLoadByRecordFile 下载录像失败，错误原因：" << GetLastErrorString() << std::endl;
				throw std::exception("Download by Record file failed");
				return;
			}
			else
			{
				NET_DVR_PlayBackControl(lRet, NET_DVR_PLAYSTART, 0, NULL);
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

void HKVendor::PlayVideo(const long loginHandle, const size_t channel, const std::string& filename)
{
	if (-1 == m_lLoginHandle)
	{
		cout << "PlayVideo 在线播放失败原因：" << GetLastErrorString() << endl;
		return;
	}

	std::vector<RecordFile>::iterator it;
	int nSize = 0;
	for (it = m_files.begin(); it != m_files.end(); ++it)
	{
		if (it->name == filename)
		{
			TestWindows Test;
			Test.Init();

			LONG hPlayHandle = NET_DVR_PlayBackByName(m_lLoginHandle, (char *)filename.c_str(), g_hWnd);

			if (hPlayHandle < 0)
			{
				std::cout << "播放失败原因：" << GetLastErrorString() << std::endl;
				throw std::exception("Play back by time failed");
			}

			if (!NET_DVR_PlayBackControl(hPlayHandle, NET_DVR_PLAYSTART, NULL, 0))
			{
				std::cout << "播放失败原因：" << GetLastErrorString() << std::endl;
				throw std::exception("Play back by time failed");
			}

			system("PAUSE");
		}

		if (m_files.size() - 1 == nSize)
		{
			std::cout << "播放文件名不存在！" << std::endl;
		}
		nSize++;
	}

}

void HKVendor::SetDownloadPath(const std::string& Root)
{

}

void HKVendor::throwException()
{

}

void HKVendor::timeDHToStd(NET_DVR_TIME *pTimeDH, tm *pTimeStd)
{
	pTimeStd->tm_year = pTimeDH->dwYear - 1900;
	pTimeStd->tm_mon = pTimeDH->dwMonth - 1;
	pTimeStd->tm_mday = pTimeDH->dwDay;
	pTimeStd->tm_hour = pTimeDH->dwHour;
	pTimeStd->tm_min = pTimeDH->dwMinute;
	pTimeStd->tm_sec = pTimeDH->dwSecond;
}

void HKVendor::trTOndt(NET_DVR_TIME &ndtStartTime, NET_DVR_TIME &ndtEndTime, const time_range range)
{
	tm tmStartTime;
	tm tmEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);

	timeStdToDH(&tmStartTime, &ndtStartTime);
	timeStdToDH(&tmEndTime, &ndtEndTime);
}

void HKVendor::timeStdToDH(tm *pTimeStd, NET_DVR_TIME *pTimeDH)
{
	pTimeDH->dwYear = pTimeStd->tm_year + 1900;
	pTimeDH->dwMonth = pTimeStd->tm_mon + 1;
	pTimeDH->dwDay = pTimeStd->tm_mday;
	pTimeDH->dwHour = pTimeStd->tm_hour;
	pTimeDH->dwMinute = pTimeStd->tm_min;
	pTimeDH->dwSecond = pTimeStd->tm_sec;
}

void HKVendor::SaveSearchFileListToFile()
{
	Document document;
	string configfile = "SearchFileList.config";
	document.Parse(configfile.c_str());
	ofstream ofs(configfile);
	OStreamWrapper osw(ofs);
	Document::AllocatorType& alloc = document.GetAllocator();

	Value root(kObjectType);

	for (size_t i = 0; i < m_files.size(); i++)
	{
		string fileKey = "videoFile";
		Value key(fileKey.c_str(), fileKey.length(), alloc);

		RecordFile file = m_files[i];
		Value name(file.name.c_str(), file.name.length(), alloc);
		Value channel(MakeStrByInteger(file.channel).c_str(), MakeStrByInteger(file.channel).length(), alloc);
		Value beginTime(MakeStrTimeByTimestamp(file.beginTime).c_str(), MakeStrTimeByTimestamp(file.beginTime).length(), alloc);
		Value endTime(MakeStrTimeByTimestamp(file.endTime).c_str(), MakeStrTimeByTimestamp(file.endTime).length(), alloc);
		Value size(MakeStrByInteger(file.size / 1024 / 1024).c_str(), MakeStrByInteger(file.size / 1024 / 1024).length(), alloc);

		Value a(kArrayType);
		a.PushBack(name, alloc).PushBack(channel, alloc).PushBack(beginTime, alloc).PushBack(endTime, alloc).PushBack(size, alloc);
		root.AddMember(key.Move(), a.Move(), alloc);
	}

	Writer<OStreamWrapper> writer(osw);
	root.Accept(writer);
}

std::string HKVendor::MakeStrTimeByTimestamp(time_t time)
{
	char cTime[50];
	struct tm ttime;

	localtime_s(&ttime, &time);
	strftime(cTime, 50, "%Y%m%d%H%M%S", &ttime);

	string strTime(cTime);

	return strTime;
}
std::string HKVendor::MakeStrByInteger(int data)
{
	char cData[50];

	sprintf_s(cData, 50, "%d", data);

	string strTime(cData);

	return strTime;
}

void HKVendor::WriteFileListToDB()
{
	//获取指针
	QMSqlite *pDb = QMSqlite::getInstance();
	//删除表
	//pDb->dropTable(DROP_SEARCH_VIDEO_TABLE);
	//创建记录表
	pDb->createTable(CREATE_SEARCH_VIDEO_TABLE);
	//一次插入所有数据
	std::vector<writeSearchVideo> RecordList;
	for (size_t i = 0; i < m_files.size(); i++)
	{
		writeSearchVideo sr;
		RecordFile record = m_files[i];
		//文件名称
		sr.set<0>(record.name);
		//通道号
		sr.set<1>(record.channel);
		//开始时间
		sr.set<2>(record.beginTime);
		//结束时间
		sr.set<3>(record.endTime);
		sr.set<4>(record.size);
		RecordList.push_back(sr);
	}

	std::string sql(INSERT_SEARCH_VIDEO);
	pDb->writeDataByVector(sql, RecordList);
}

std::vector<time_range> HKVendor::MakeTimeRangeList(const time_range& range)
{
	time_t timeStart = range.start;
	time_t timeEnd = range.end;
	std::vector<time_range> timeRangeList;

	tm tmStartTime;
	tm tmEndTime;
	NET_DVR_TIME ntStartTime;
	NET_DVR_TIME ntEndTime;

	_localtime64_s(&tmStartTime, (const time_t*)&range.start);
	_localtime64_s(&tmEndTime, (const time_t*)&range.end);


	timeStdToDH(&tmStartTime, &ntStartTime);
	timeStdToDH(&tmEndTime, &ntEndTime);

	if (timeEnd - timeStart <= ONE_DAY)
	{
		if (ntStartTime.dwDay == ntEndTime.dwDay)
		{
			timeRangeList.push_back(range);
		}
		else
		{
			time_range rangeItem;
			rangeItem.start = timeStart;
			time_t diff = (23 - ntStartTime.dwHour) * ONE_HOUR + (59 - ntStartTime.dwMinute) * ONE_MINUTE + (59 - ntStartTime.dwSecond);
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
		int day = (int)(diff / ONE_DAY + (diff%ONE_DAY > 0 ? 1 : 0));

		if (ntStartTime.dwHour == 0 && ntStartTime.dwMinute == 0 && ntStartTime.dwSecond == 0)
		{
			for (size_t i = 0; i < day - 1; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
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
			time_t diff = (23 - ntStartTime.dwHour) * ONE_HOUR + (59 - ntStartTime.dwMinute) * ONE_MINUTE + (59 - ntStartTime.dwSecond);
			rangeItem.end = timeStart + diff;
			timeRangeList.push_back(rangeItem);

			timeStart = timeStart + diff + 1;
			for (size_t i = 0; i < day - 2; i++)
			{
				time_range rangeItem;
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
			}

			if (timeEnd > timeStart + ONE_DAY - 1)
			{
				rangeItem.start = timeStart;
				rangeItem.end = timeStart + ONE_DAY - 1;;
				timeRangeList.push_back(rangeItem);

				timeStart = timeStart + ONE_DAY;
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

void HKVendor::CreatePath(const size_t channel)
{
	std::string strPath = "D:\\DownLoadVideo\\";

	BOOL bOne = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bOne)
	{
		std::cout << "Error!" << std::endl;
		return;
	}

	strPath.append("海康\\");
	BOOL bTwo = CreateDirectoryA(strPath.c_str(), NULL);
	if (!bTwo)
	{
		std::cout << "Error!" << std::endl;
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

std::string HKVendor::GetLastErrorString()
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

#include "catch.hpp"

TEST_CASE_METHOD(HKVendor, "Init HK SDK", "[HKVendor]")
{

	time_range range;
	range.start = 1467475200;
	//range.end = 1466524800;
	range.end = 1467561600;
	//range.end = 1478833871;
	REQUIRE_NOTHROW(Init());
 
 	REQUIRE_NOTHROW(Login("192.168.0.92", 8000, "admin", "admin123"));

 	REQUIRE_NOTHROW(Search(0, 33, range));
 	//REQUIRE_NOTHROW(Search(1, range));
	
 	//REQUIRE_NOTHROW(Download(0, 33, range));
	//REQUIRE_NOTHROW(Download(0, 1,"ch0001_00000000137000400"));
	//REQUIRE_NOTHROW(PlayVideo(0, 0, "ch0003_00000000008000000"));
 	//REQUIRE_NOTHROW(PlayVideo(0, 35, range));
	// 	REQUIRE_NOTHROW(Logout());
}