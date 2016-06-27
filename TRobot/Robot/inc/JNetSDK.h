#ifndef	__JNET_SDK_H__
#define	__JNET_SDK_H__

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the JNETSDK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// JNETSDK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#if (defined _WIN32) || (defined _WIN64)
	//#ifdef JNETSDK_EXPORTS
	//#define JNETSDK_API __declspec(dllexport)
	//#else
	//#define JNETSDK_API __declspec(dllimport)

	//#endif
	#define JNETSDK_API
#elif (defined __APPLE_CPP__) || (defined __APPLE_CC__)
	#ifdef JNETSDK_EXPORTS
	#define JNETSDK_API extern "C"
	#else
	#define JNETSDK_API
	#endif
#else// __ANDROID__
	#define JNETSDK_API extern "C"
#endif	// __ANDROID__


// ����Э������ begin
#define JNET_PRO_T				0x20000000			// Э������
#define JNET_PRO_T_JXJ			0x20000002			// JXJ����
#define JNET_PRO_T_JPF			0x20000004			// JPF����
#define	JNET_PRO_T_FILE			0x22000000			// ¼������(¼��ط�)
#define	JNET_PRO_T_FILE_JAV		0x22000001			// JAV¼������
// Э������ begin
#define JNET_READ_PRO_JXJ		0x204A584A			// JXJ����
#define JNET_READ_PRO_JPF		0x204A5046			// JPF����
#define JNET_READ_PRO_LWZ		0x205a574c			// LWZ����
// Э������ begin
// ����Э������ end

// ��������� begin
typedef enum
{
	JNETErrSuccess				= 0,		// �ɹ�
	JNETErrUnInit				= -1,		// δ��ʼ��
	JNETErrHandle				= -2,		// ���������
	JNETErrParam				= -3,		// ��������
	JNETErrBuffSize				= -4,		// ������
	JNETErrNoMem				= -5,		// �ڴ治��
	JNETErrRecv					= -6,		// ���մ���
	JNETErrSend					= -7,		// ���ʹ���
	JNETErrOperate				= -8,		// ��������
	JNETErrURL					= -9,		// URL����
	JNETErrLogining				= -10,		// �û����ڵ�¼
	JNETErrLogout				= -11,		// �Ѿ��ǳ�
	JNETErrNoFreePort			= -100,		// û�п���ͨ��
	JNETErrProtocol				= -101,		// Э�����
	JNETErrXMLFormat			= -102,		// �����XML����
	JNETErrNotSupport			= -103,		// ��֧�ֵĲ���
	JNETErrGetParam				= -104,		// ��ȡ��������
	JNETErrSetParam				= -105,		// ���ò�������
	JNETErrOpenFile				= -106,		// ���ļ�����
	JNETErrUpgOpen				= -107,		// ��������
}JNETErr;
// ��������� end

typedef enum
{
	JNET_EET_UNKNOWN			= -1,		// δ����
											
	JNET_EET_LOGINING			= 0,		// ���ڵ�¼
	JNET_EET_LOGIN_OK			= 1,		// ��¼�ɹ�
	JNET_EET_LOGIN_ERROR		= 2,		// ��¼ʧ��
	JNET_EET_LOGOUT				= 3,		// �ǳ�
	JNET_EET_STREAM_OPENING		= 4,		// �����ڴ�
	JNET_EET_STREAM_OK			= 5,		// ���򿪳ɹ�
	JNET_EET_STREAM_ERROR		= 6,		// ����ʧ��
	JNET_EET_STREAM_CLOSE		= 7,		// ���ر�
	JNET_EET_PB_OPENING			= 8,		// �ط������ڴ�
	JNET_EET_PB_OK				= 9,		// �ط����򿪳ɹ�
	JNET_EET_PB_ERROR			= 10,		// �ط�����ʧ��
	JNET_EET_PB_CLOSE			= 11,		// �ط����ر�
	JNET_EET_TALK_OPENING		= 12,		// �Խ����ڴ�
	JNET_EET_TALK_OK			= 13,		// �Խ��򿪳ɹ�
	JNET_EET_TALK_ERROR			= 14,		// �Խ���ʧ��
	JNET_EET_TALK_CLOSE			= 15,		// �Խ��ر�
	JNET_EET_UPG_START			= 16,		// ������ʼ
	JNET_EET_UPG_OK				= 17,		// �����ɹ�
	JNET_EET_UPG_ERR			= 18,		// ����ʧ��
	JNET_EET_CFG_SET			= 100,		// ��ȡ����
	JNET_EET_CFG_GET			= 101,		// ��������
	JNET_EET_PTZ_CTRL			= 102,		// ��̨����
	JNET_EET_DEV_CTRL			= 103,		// �豸����
	JNET_EET_STREAM_LOST		= 104,		// ����ʧ
	JNET_EET_EVENT_NOTIFY		= 201,		// �����¼�
	JNET_EET_EVENT_REC_DOWN     = 303,      // ����
}eJNetEvent;

typedef enum
{
	JNET_STREAM_CTRL_UNKNOWN		= 0,
	JNET_STREAM_CTRL_I_FRAME_REQ	= 0x1,	// I֡����
}eJNetStreamCtrl;

typedef enum
{
	JNET_PB_CTRL_UNKNOWN		= 0,
	JNET_PB_CTRL_START			= 0x1,		// ��ʼ�ط�
	JNET_PB_CTRL_STOP			= 0x2,		// ֹͣ�ط�
	JNET_PB_CTRL_PAUSE			= 0x3,		// ��ͣ�ط�
	JNET_PB_CTRL_RESUME			= 0x4,		// �ָ��ط�
	JNET_PB_CTRL_FAST			= 0x5,		// ���
	JNET_PB_CTRL_SLOW			= 0x6,		// ����
	JNET_PB_CTRL_NORMAL			= 0x7,		// ��������
	JNET_PB_CTRL_SET_SPEED		= 0x8,		// ���ò����ٶ�
	JNET_PB_CTRL_GET_SPEED		= 0x9,		// ��ȡ�����ٶ�
	JNET_PB_CTRL_FRAME			= 0xa,		// ��֡��
	JNET_PB_CTRL_SET_TIME		= 0xb,		// ���ûط�ʱ��
	JNET_PB_CTRL_GET_TIME		= 0xc,		// ��ȡ�ط�ʱ��
}eJNetPBCtrl;

typedef enum
{

	JNETPlaySpeed_16X		= 0x01,			// ��16����
	JNETPlaySpeed_8X		= 0x02,			// ��8����
	JNETPlaySpeed_4X		= 0x03,			// ��4����
	JNETPlaySpeed_2X		= 0x04,			// ��2����

	JNETPlaySpeed_1X		= 0x05,			// �����ٶ�

	JNETPlaySpeed_1_2X		= 0x06,			// ��2����
	JNETPlaySpeed_1_4X		= 0x07,			// ��4����
	JNETPlaySpeed_1_8X		= 0x08,			// ��8����
	JNETPlaySpeed_1_16X		= 0x09,			// ��16����

}eJNetPlaySpeed;

typedef enum
{
	JNET_PROXY_UNKNOWN			= 0,
	JNET_PROXY_JCMS				= 1,		// CMS���������
	JNET_PROXY_JSW				= 2,		// SW���������
	JNET_PROXY_JPB				= 3,		// PB���������
	JNET_PROXY_JMDS				= 4,		// ת�����������
}eJNetProxyType;

typedef enum
{
	eJNetFrame_I				= 0x1,		// I֡
	eJNetFrame_P				= 0x2,		// P֡
	eJNetFrame_B				= 0x3,		// B֡
	eJNetFrame_A				= 0x4,		// ��Ƶ֡
	eJNetFrame_Str				= 0x5,		// ��Ļ֡
	eJNetFrame_Shape			= 0x6,		// ��״֡
	eJNetFrame_User				= 0xF,		// �û��Զ���֡
}eJNetFrameType;

#define MAX_BUFFER_LEN			8192		// ÿ�����յĻ���,��ʵ2KB�㹻
#define MAX_POST_ACCEPT			10			// ��������
#define	MAX_GET_PARAM_SIZE		96			// ���ͬʱ��ȡ�����ĸ���

#define RTP_PORT_START			80800		// ��ʼ���˿�
#define RTP_PORT_COUNT			1000		// ���˿ڸ���
#define MAX_URL_LEN				256			// URL��
#define JNET_IP_LEN				64			// IP��
#define JNET_NAME_LEN			64			// ���Ƴ�
#define JNET_PWD_LEN			64			// ���볤
#define JNET_DATA_LEN			512			// ������

typedef int (__stdcall fcbJStream)(long lHandle, LPBYTE pBuff,DWORD dwRevLen,void* pUserParam);										// ���յ���Ϣ�Ļص�����
typedef int (__stdcall fcbJEvent)(long lHandle, eJNetEvent eType,int iDataType,void* pEventData,int iDataLen,void* pUserParam);		// �¼��ص�����
typedef int (__stdcall fcbJMBNotify)(long lHandle,DWORD dwPortocol,int iErr,int iMsgID,LPCTSTR lpszDstID,void* pData,int iDataLen,void* pUserParam);	// �㲥�ص�
typedef int (__stdcall fcbJGetParam)(long lHandle,int iChn,int iParamID,void* lpData,int iDataLen,void* pUserParam);				// ��ȡ�����Ļص�

// �������
typedef struct stJProxyParam
{
	char			m_szIP[JNET_IP_LEN];			// �����������IP
	int				m_iPort;						// ����������Ķ˿�
	char			m_szUser[JNET_NAME_LEN];		// ������������û���
	char			m_szPwd[JNET_PWD_LEN];			// ���������������
	eJNetProxyType	m_eType;						// �������������
	int				m_iDataLen;						// ��չ������
	char			m_szData[JNET_DATA_LEN];		// ��չ������
}* PSTJProxyParam;

// VVP2P����
typedef struct stJP2PParam
{
	long m_lStreamPort;								// ���˿�
	long m_lTalkPort;								// �Խ��˿�
}* PSTJP2PParam;

// ϵͳ���� begin
// ��ȡ�����İ汾��
JNETSDK_API DWORD JNetGetVersion();
// ��ȡ���õ�SOCKET�˿�
JNETSDK_API int JNetGetValidSocketPort(bool bUdp);
// ��ȡ���е�IPv4,����IPv4�б�<IP;>
JNETSDK_API LPCTSTR JNetGetIPv4List(int& iCount);
// ��ʼ��
JNETSDK_API int JNetInit(LPCTSTR lpszLogName);
// ע������
JNETSDK_API int JNetCleanup();
// ϵͳ���� end

// �㲥���� begin
// ��ʼ��
JNETSDK_API long JNetMBOpen(LPCTSTR lpszMuIP, int iMuPort,fcbJMBNotify* pfcbMBNotify,void* pUserParam,DWORD dwProtocol,long& lMBHandle);
// ע������
JNETSDK_API long JNetMBClose(long lMBHandle);
// ��������
JNETSDK_API long JNetMBSearch(long lMBHandle,int iTimeOut);
// ���ò���
JNETSDK_API long JNetMBSet(long lMBHandle,int iMsgID,LPCTSTR lpszDstID,
						   LPCTSTR lpszUser,LPCTSTR lpszPsw,void* pData,int iDataLen,int iTimeOut);
// �㲥���� end

// ��¼��� begin
JNETSDK_API long JNetLoginEx(LPCTSTR lpszConn,fcbJEvent* pfcbEvent,void* pUserEvent,long& lLogin);
JNETSDK_API long JNetLogin(LPCTSTR lpszIP,int iDevPort,LPCTSTR lpszUser,LPCTSTR lpszPassword,
						   int iTimeOut,fcbJEvent* pfcbEvent,void* pUserParam,DWORD dwProtocol,long& lLogin,PSTJProxyParam pstProxy);
JNETSDK_API long JNetLogout(long lLogin);
JNETSDK_API DWORD JNetGetProtocol(long lLogin);
// ��¼��� end

// ������ begin
// ����,���ؾ��
JNETSDK_API long JNetStreamOpen(long lLogin,int iChn,int iStreamID,int iProtocol,fcbJStream* pfcbStream,void* pUserData,long& lStream);
// ����,���ؾ��
// lpszConn:Protocol=0x20000002;DevIP=;DevChn=;StreamID=;DevPort=;UserName=;Password=;Breath=;
// lpszConn:Protocol=0x20000004;URL=;
JNETSDK_API long JNetStreamOpenEx(LPCTSTR lpszConn,fcbJStream* pfcbStream,void* pUserStream,fcbJEvent* pfcbEvent,void* pUserEvent,long& lStream);
// ������,������Ҫ���Ƶ�ֵ
JNETSDK_API long JNetStreamCtrl(long lStream,eJNetStreamCtrl eCtrlType,void* pCtrlData,int iDataLen);
// �ر���,������Ҫ�رյľ��
JNETSDK_API long JNetStreamClose(long lStream);
// ������ end

// �Խ����� begin
// �򿪶Խ�,���ؾ��
JNETSDK_API long JNetTalkOpen(long lLogin,int iSamples,int iEncodeType,int iChannels,int iBits,fcbJStream* pfcbStream,void* pUserData,long& lTalk);
// �򿪶Ի������ؾ��
// lpszConn:Protocol=0x20000002;DevIP=;DevChn=;StreamID=;DevPort=;UserName=;Password=;Breath=;
// lpszConn:Protocol=0x20000002;DevIP=;DevChn=;StreamID=;DevPort=;UserName=;Password=;Breath=;
JNETSDK_API long JNetTalkOpenEx(LPCTSTR lpszConn,int iSamples,int iEncodeType,int iChannels,int iBits,fcbJStream* pfcbStream,void* pUserData,long& lTalk);
// ���ͶԽ�����
JNETSDK_API long JNetTalkSend(long lTalk,void* lpData,int iLen);
// �رնԽ�,������Ҫ�رյľ��
JNETSDK_API long JNetTalkClose(long lTalk);
// �Խ����� end

// ������ȡ������ begin
JNETSDK_API long JNetGetParam(long lLogin,int iChn,int iParamID,void* lpData,int iDataLen,fcbJGetParam* pfcbGetParam,void* pUser);
JNETSDK_API long JNetSetParam(long lLogin,int iChn,int iParamID,void* lpData,int iDataLen,bool bSync);
// ������ȡ������ end

// �豸���� begin
JNETSDK_API long JNetDevCtrl(long lLogin,int iChn,int iCtrlCmd,int iCtrlValue);
JNETSDK_API long JNetUpgOpen(long lLogin,LPCTSTR lpszPath,long& lUpg);
JNETSDK_API long JNetUpgClose(long lUpg);
// �豸���� end

// ¼����� begin
// iStreamID:0~2,time:YYYYMMDDhhmmss
JNETSDK_API long JNetRecQuery(long lLogin,LPCTSTR lpszDevIP,int iChn,int iStreamID,LPCTSTR lpszBeginTime,LPCTSTR lpszEndTime,int iRecType);
JNETSDK_API long JNetRecOpen4Item(long lLogin,void* pItem,int iIsPlay,fcbJStream* pfcbStream,void* pUserData,long& lRec);
JNETSDK_API long JNetRecOpen4Time(long lLogin,
								  LPCTSTR lpszDevIP,int iChn,int iStreamID,LPCTSTR lpszBeginTime,LPCTSTR lpszEndTime,int iRecType,
								  int iIsPlay,fcbJStream* pfcbStream,void* pUserData,long& lRec);
JNETSDK_API long JNetRecOpen4TimeEx(LPCTSTR lpszConn,
								  LPCTSTR lpszDevIP,int iChn,int iStreamID,LPCTSTR lpszBeginTime,LPCTSTR lpszEndTime,int iRecType,
								  int iIsPlay,fcbJStream* pfcbStream,void* pUserData,fcbJEvent* pfcbEvent,void* pUserEvent,long& lRec);

JNETSDK_API long JNetRecCtrl(long lRec,eJNetPBCtrl eCtrlCmd,void* lpCtrlValue);
JNETSDK_API long JNetRecClose(long lRec);
// ¼����� end

//����WWP2P����(zc20140228)
JNETSDK_API long JNetSetWWP2PParam(long lLogin, PSTJP2PParam pstP2PParam);

#endif	// __JNET_SDK_H__
