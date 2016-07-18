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

#endif 