#pragma once

// System
#include <Windows.h>
#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <tchar.h>
#include <map>
#include <assert.h> 
#include <ctime>
#include "io.h"
#include "direct.h"

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

#define Test_Bug
#define Test_Filename

#define Vendor_JXJ	"���Ž�"
#define Vendor_DZP	"������"

#define MAX_SEARCH_COUNT 1000
#define ONE_DAY		 (24 * 60 * 60)
#define ONE_HOUR	 (60 * 60)
#define ONE_MINUTE	 (60)

typedef enum
{
	Err_No = 0,
	Err_LoginSuccess, // Login Success
	Err_LoginFail,	// Login Fail
	Err_DownloadSuccess, // Download Success
}eErrCode;

struct RecordFile
{
	RecordFile()
	{
		channel = 0;
		size = 0;
		pPrivateData = nullptr;
		PrivateDataDataSize = 0;
	}
	~RecordFile()
	{
		if (nullptr != pPrivateData)
		{
			delete pPrivateData;
			pPrivateData = nullptr;
			PrivateDataDataSize = 0;
		}
	}
	RecordFile(const RecordFile& other)
	{
		channel = other.channel;
		size = other.size;
		name = other.name;
		//alias = other.alias;
		strTimeSection = other.strTimeSection;
		beginTime = other.beginTime;
		endTime = other.endTime;
		duration = other.duration;
		pPrivateData = nullptr;
		PrivateDataDataSize = 0;
		setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
	}

	RecordFile& operator= (const RecordFile& other)
	{
		if (&other == this)
		{
			return *this;
		}
		channel = other.channel;
		size = other.size;
		name = other.name;
		//alias = other.alias;
		strTimeSection = other.strTimeSection;
		beginTime = other.beginTime;
		endTime = other.endTime;
		duration = other.duration;
		setPrivateData(other.pPrivateData, other.PrivateDataDataSize);
		return *this;
	}
	// Set Private Data
	void setPrivateData(void* pData, int size)
	{
		if (nullptr != pPrivateData)
		{
			delete pPrivateData;
			pPrivateData = nullptr;
			PrivateDataDataSize = 0;
		}
		if (size > 0)
		{
			pPrivateData = new char[size];
			PrivateDataDataSize = size;
			memcpy(pPrivateData, pData, size);
		}
	}
	// Get Private Data
	void* getPrivateData() const
	{
		return pPrivateData;
	}
	// Get Private Data Size
	__int32 getPrivateDataSize() const
	{
		return PrivateDataDataSize;
	}
	void deletePrivateData()
	{
		delete pPrivateData;
		pPrivateData = nullptr;
	}

	int channel;
	int size;
	std::string name;
	//std::string alias;
	std::string strTimeSection;
	std::time_t beginTime;
	std::time_t endTime;
	unsigned long duration;
	char* pPrivateData;   //Private Data
	__int32 PrivateDataDataSize;//Private Data Size
};

#include "j_sdk.h"

class CCommonUtrl
{
public:
	CCommonUtrl();
	~CCommonUtrl();

	static CCommonUtrl& getInstance();

	// File Name & File Folder
	std::string MakeDownloadFileFolder(const std::string basePath, const std::string& startTimeZero, const std::string& endTimeZero, const std::string& venderName, int channel);
	void MakeFolder(std::string fileName);

	// Video Time Range(JTime - JxjVendor)
	std::vector<time_range> MakeTimeRangeList(const time_range& range);
	void InitSearchTime(JTime& jStartTime, JTime& jStopTime, const __time64_t& timeStart, const __time64_t& timeEnd);
	time_t MakeTimestampByJTime(JTime jTime);
	std::string MakeStrTimeByTimestamp(time_t time);
	
	// Json File Handle
	void SaveSearchFileListToFile(const std::vector<RecordFile>& files);
	std::vector<RecordFile> LoadSearchFileListFromFile();

};


typedef struct STRU_CHANNEL_INFO
{
	int		iDeviceIndex;			//��Ӧ���豸�������±�
	int		iChanIndex;			//���ͨ����
	char	chChanName[100];	//����ͨ���������

	int		iPicResolution;				//ͼƬ�ֱ���
	int		iPicQuality;				//ͼƬ����
	LONG	lRealHandle;          //ʵʱԤ�����

	int     iWinowIndex;           //Ԥ��ʱ��ͨ�������Ĵ�������
	BOOL    isRecording;           //����¼��
	BOOL    bEnable;               //ͨ���Ƿ�ʹ��
	char	chAccessChanIP[16];//ip����ͨ����ip��ַ

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
	int		iDeviceIndex;			//�豸�����±�
	LONG	lLoginID;				//ע��ID
	char	chLocalNodeName[100];	//�����豸�ڵ�����
	char	chDeviceName[100];		//�豸����
	char	chDeviceIP[33];			//����ֱ�����ӵ��豸IP
	UINT	lDevicePort;			//�豸���˿ں�
	char	chLoginUserName[30];	//��½�豸���û��������û������ʱ���õ�
	char	chLoginPwd[30];	//��½�豸������
	//char	chDeviceMultiIP[20];	//�ಥ���ַ
	char	chSerialNumber[50];		//���к�
	int		iDeviceChanNum;		    //�豸��ͨ����
	int		iStartChan;				//�豸��ʼͨ����
	int		iDeviceType;			//�豸����
	int 	iIPChanNum;				//�������ͨ������
	BOOL    bIPRet;                 //�Ƿ�IP����
	//	BOOL	bCycle;					//���豸�Ƿ���ѭ������
	BOOL	bPlayDevice;			//���豸�ǲ�������ͨ�����ڲ���
	BOOL    bIsLogin;               //�Ƿ���ע����豸
	int		iEnableChanNum;			//��Чͨ����
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