#ifndef __HK_HEAD_H__
#define __HK_HEAD_H__


#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h> 

// Json
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ostreamwrapper.h"
#include "istreamwrapper.h"

// Self
#include "AbstractVendor.h"
#include "QMSqlite.h"
#include "TestWindows.h"

using namespace rapidjson;


#define MAX_SEARCH_COUNT 1000
#define ONE_DAY		 (24 * 60 * 60)
#define ONE_HOUR	 (60 * 60)
#define ONE_MINUTE	 (60)


typedef struct STRU_CHANNEL_INFO
{
	int		iDeviceIndex;			//对应的设备的数组下标
	int		iChanIndex;			//监控通道号
	char	chChanName[100];	//本地通道结点名称

	int		iPicResolution;				//图片分辨率
	int		iPicQuality;				//图片质量
	LONG	lRealHandle;          //实时预览句柄

	int     iWinowIndex;           //预览时与通道关联的窗口索引
	BOOL    isRecording;           //正在录像
	BOOL    bEnable;               //通道是否使能
	char	chAccessChanIP[16];//ip接入通道的ip地址

	STRU_CHANNEL_INFO *pNext;

	STRU_CHANNEL_INFO()
	{
		iDeviceIndex = -1;
		iChanIndex = -1;
		chChanName[0] = '\0';
		iPicResolution = 0;
		iPicQuality = 2;
		lRealHandle = -1;
		chAccessChanIP[0] = '\0';
		iWinowIndex = -1;
		isRecording = FALSE;
		bEnable = FALSE;
		pNext = NULL;
	}
}CHANNEL_INFO, *pCHANNEL_INFO;

typedef struct STRU_DEVICE_INFO
{
	int		iDeviceIndex;			//设备数组下标
	LONG	lLoginID;				//注册ID
	char	chLocalNodeName[100];	//本地设备节点名称
	char	chDeviceName[100];		//设备名称
	char	chDeviceIP[33];			//可以直接连接的设备IP
	UINT	lDevicePort;			//设备主端口号
	char	chLoginUserName[30];	//登陆设备的用户名，在用户管理的时候用到
	char	chLoginPwd[30];	//登陆设备的密码
	//char	chDeviceMultiIP[20];	//多播组地址
	char	chSerialNumber[50];		//序列号
	int		iDeviceChanNum;		    //设备的通道数
	int		iStartChan;				//设备开始通道号
	int		iDeviceType;			//设备类型
	int 	iIPChanNum;				//最大数字通道个数
	BOOL    bIPRet;                 //是否IP接入
	//	BOOL	bCycle;					//该设备是否在循环播放
	BOOL	bPlayDevice;			//该设备是不是所有通道都在播放
	BOOL    bIsLogin;               //是否已注册该设备
	int		iEnableChanNum;			//有效通道数
	CHANNEL_INFO struChanInfo[64];
	STRU_DEVICE_INFO *pNext;

	STRU_DEVICE_INFO()
	{
		//  iDeviceIndex  = -1;
		lLoginID = -1;
		chLocalNodeName[0] = '\0';
		chDeviceName[0] = '\0';
		chDeviceIP[0] = '\0';
		lDevicePort = 8000;
		chLoginUserName[0] = '\0';
		chLoginPwd[0] = '\0';
		//chDeviceMultiIP[0]   ='\0';
		chSerialNumber[0] = '\0';
		iDeviceChanNum = -1;
		iStartChan = 0;
		iDeviceType = 0;
		iIPChanNum = 0;
		bIPRet = 0;
		bIsLogin = FALSE;
		//		 bCycle              = FALSE;
		bPlayDevice = FALSE;
		iEnableChanNum = -1;
		pNext = NULL;
	}
}LOCAL_DEVICE_INFO, *pLOCAL_DEVICE_INFO;

#endif 