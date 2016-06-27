#pragma once

#include "IniFile.h"

//ȫ��
class CPub_Data
{
public:
	CPub_Data(void);
	virtual ~CPub_Data(void);

public:
	CStringArray g_strArrayLoginIp;
	CStringArray g_strArraySavedIp;//�Ѿ���ӵ��豸IP
	CStringArray g_strArrayPlaySuccessIp;  //���ųɹ���IP

	int g_iWarnIpIndex;
	int g_iCountDevice;            //��ӵ��豸����
	HTREEITEM     g_hAddItem[GET_MAX_DEV_NUM];   //������ӵĽڵ�

	CString g_strUser;             //�豸��½�û���
	CString g_strPassword;       // �豸��½����
	int g_iDevicePort;          //�豸�˿ں�

	int g_iChancel[GET_MAX_DEV_NUM];   //��Ƶ����ͨ����
	int g_iIsPlayNum;              //���ڲ��ŵĴ�����

	bool g_bOpenStreamInAttribute;
	
	CString g_strLoginIp;       //��½��IP

	int g_iStaticScreenWidth;       //��̬���ڵĿ�
	int g_iStaticScreenHeight;		//��̬���ڵĸ�
	int g_iCutScreenNum;          //�ָ�Ĵ�����
	int g_iCutScreenWidth;         //�ָ�Ĵ��ڵĿ�
	int g_iCutScreenHeight;		//�ָ�Ĵ��ڵĸ�

	CRect g_RectDlg ;           //������λ��
	int g_iMainWindowX;         //�����������Ļ������X
	int g_iMainWindowY;			//�����������Ļ������Y
	int g_iMainWindowW;         //�����������Ļ������width
	int g_iMainWindowH;			//�����������Ļ������height

	int g_iMoveX;        //��̬����ڶԻ��� x λ��
	int g_iMoveY;       //��̬����ڶԻ��� y λ��
	int g_iStaticToScreenX; //��̬�������Ļ x λ��
	int g_iStaticToScreenY;//��̬�������Ļ y λ��
	
	int g_iAttributeIng;    //���ڽ����������õĴ���

	int g_iCountPerson[GET_MAX_DEV_NUM]; //����ͳ������

	int g_iStreamType[GET_MAX_DEV_NUM];
	long g_vLoginHandle[GET_MAX_DEV_NUM];
	long g_vStreamHandle[GET_MAX_DEV_NUM];
	long g_vTalkHandle[GET_MAX_DEV_NUM];

	int g_iCountHide[GET_MAX_DEV_NUM];    //�ڵ���������
	int g_iCountInterset[GET_MAX_DEV_NUM]; //����Ȥ��������
	int g_iCountHideAlarm[GET_MAX_DEV_NUM];  //�ڵ�������������
	int g_iCountMoveSense[GET_MAX_DEV_NUM];    //�ƶ������������
	HTREEITEM g_hTreeItem[GET_MAX_DEV_NUM];  //���Ľڵ�
	int g_iRecFileID[GET_MAX_DEV_NUM];    //¼��ID
	int g_iPlayBack;                 //��Ƶ�طŵĴ���
	int g_iWarningCount;

	int g_iReplace;     //ɾ�����滻�±�
	int g_iDeleteDevice;  // ɾ�����豸�±�
	int g_iFullScreenId;   //ȫ���Ĵ���
//////////////////////////////
	int g_iVideoTime1;      //����ʱ��
	int g_iVideoTime2;
	int g_iVideoTime3;
	int g_iVideoTime4;

	int g_iLanguageFlag;


	COleDateTime g_STime1;
	COleDateTime g_STime2;
	COleDateTime g_STime3;
	COleDateTime g_STime4;

	COleDateTime g_ETime1;
	COleDateTime g_ETime2;
	COleDateTime g_ETime3;
	COleDateTime g_ETime4;
/////////////////////////////////////
	int g_iShowTimeDlgFlag;    //��������ʱ��Ի�����
	int m_iVideoIndex;
	CString g_StrPath;
	long g_setPos;
	BOOL g_bTalk[GET_MAX_DEV_NUM];
	BOOL g_DeviceClose[GET_MAX_DEV_NUM];  //�豸�Ƿ�Ͽ�

	int g_iDownIndex; //����ʱ�Ĵ���
	bool g_bDragDown; //���±�־
	bool g_bAllFullScreen;
	int g_bUpgDeviceClose;  //�Ƿ������е�����
	int g_iIoWarningChannelNul;
	int g_iChannelNo;
	JDevCap g_jDevCap[GET_MAX_DEV_NUM];//�豸������



	int g_iDownLoadPos;

	/////////////////////////////////
	//0.����osd 1.��Ƶ�ڵ� 2.ͼ����� 3.����Ȥ���� 4.�ڵ����� 5.�ƶ���� 6.��ʧ����
	int g_iPlayWndFlag;			//	���������д򿪴�����Ƶ�Ĵ��ڱ�־ 
	/////////////////////////////////

	long g_lRecodeStartTime[GET_MAX_DEV_NUM];  //¼��ʼʱ��
	long g_lRecodeEndTime[GET_MAX_DEV_NUM]; 
	long g_lFrameCount[GET_MAX_DEV_NUM];   //֡��

	BOOL FreeObj(void*  pObj);

	HDC g_drawHdc;
	int g_iServerFlag[GET_MAX_DEV_NUM];  //�Ƿ����½��־
	int m_nDrawCount[GET_MAX_DEV_NUM];

	int  m_iJointActionChannel;
	int  m_iJointRecTime;
	int  m_iJointOut;
	int  m_iJointOutTime;
	int  m_iJointTakePicChannel;
	int  m_iJoinTakePicLever;
	int  m_iTakePicFtpUp;
	int  m_iTakePicCount;
	int  m_iJointVoice;
	int  m_iJointVoiceTime;
	int  m_iJointEmail;

	int g_iFormatProgress;
	int g_iControlDevRet;

	CString g_strSetMessage;
	CString g_strGetMessage;

	CIniFile g_iniFile;




	CString g_strTalkS;
	CString g_strTalkE;

	CString g_strRecordS;
	CString g_strRecordE;

	CString g_strListenS;
	CString g_strListemE;


	bool g_bHideAlarmDrawBorder;  //�Ƿ񻭿�
	bool g_bMoveAlarmDrawBorder;
	bool g_bHideDrawBorder;
	bool g_bInterestDrawBorder;



	
};

extern CPub_Data g_PubData;