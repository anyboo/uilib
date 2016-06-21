
#ifndef	__AV_PLAYER_H__
#define	__AV_PLAYER_H__

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AVPLAYER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AVPLAYER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef AVPLAYER_EXPORTS
#define AVPLAYER_API extern "C" __declspec(dllexport)
#else
#define AVPLAYER_API extern "C" __declspec(dllimport)
#endif

#define	AVP_CODER_GEN			"GenCodec.dll"	// ͨ�ý�����
#define	AVP_CODER_JXJ			"jxjcodec.dll"	// ���Žݽ�����
#define	AVP_CODER_HI			"HICoder.dll"	// ��˼������
#define	AVPDisDevStrLen			124				// ��ʾ�豸���Ƴ���

// Э������ begin
#define AVP_PROTOCOL_JXJ		0x204A584A
#define AVP_PROTOCOL_JPF		0x204A5046
#define AVP_PROTOCOL_LWZ		0x205a574c
// Э������ begin

// ��������� begin
typedef enum
{
	AVPErrSuccess		= 0,			// �ɹ�
	AVPErrUnInit		= -1,			// δ��ʼ��
	AVPErrHandle		= -2,			// ���������
	AVPErrFile			= -3,			// �ļ���ʧ��
	AVPErrParam			= -4,			// ��������
	AVPErrBuffSize		= -5,			// ������
	AVPErrLoadCoder		= -6,			// ���ؽ�����ʧ��
	AVPErrNoMem			= -7,			// �ڴ治��
	AVPErrNoFreePort	= -8,			// û�п���ͨ��
	AVPErrReaderFormat	= -9,			// ���ݸ�ʽ������
	AVPErrRecMaxSize	= -10,			// ¼���ļ��Ѿ�д��(ָ��Ԥ�����Сʱ)
	AVPErrRecMaxTime	= -11,			// ¼���ļ��Ѿ�д��(ָ���ļ�ʱ��ʱ)
}AVPErr;
// ��������� end

// ���������� begin
typedef enum
{
	AVPStreamUndef		= 0,			// δ����
	AVPStreamFile		= 1,			// �ļ���
	AVPStreamReal		= 2,			// ʵʱ��
	AVPStreamPlayback	= 3,			// �ط���
}AVPStreamType;
// ���������� end

// ���岥��״̬ begin
typedef enum
{
	AVPPlayStatusStop	= 0,			// ֹͣ
	AVPPlayStatusRun	= 1,			// ����
	AVPPlayStatusIdle	= 2,			// ����
}AVPPlayStatus;
// ���岥��״̬ end

// ���岥���ٶ� begin
typedef enum
{
	AVPPlaySpeed_16X	= 0x01,			// ��16����
	AVPPlaySpeed_8X		= 0x02,			// ��8����
	AVPPlaySpeed_4X		= 0x03,			// ��4����
	AVPPlaySpeed_2X		= 0x04,			// ��2����

	AVPPlaySpeed_1X		= 0x5,			// �����ٶ�

	AVPPlaySpeed_1_2X	= 0x06,			// ��2����
	AVPPlaySpeed_1_4X	= 0x07,			// ��4����
	AVPPlaySpeed_1_8X	= 0x08,			// ��8����
	AVPPlaySpeed_1_16X	= 0x09,			// ��16����


	AVPPlaySpeed_0_1X	= 0x20,			// ��2������
	AVPPlaySpeed_0_2X	= 0x21,			// ��2������
	AVPPlaySpeed_0_4X	= 0x22,			// ��4������
	AVPPlaySpeed_0_8X	= 0x23,			// ��8������
	AVPPlaySpeed_0_16X	= 0x24,			// ��16������
	AVPPlaySpeed_0_32X	= 0x25,			// ��32������
}AVPPlaySpeed;
// ���岥���ٶ� end

// ����������� begin
typedef enum
{
	AVPScaleNone		= 0x0,			// ������
	AVPScaleFull		= 0x1,			// ȫ������
	AVPScaleKeep		= 0x2,			// ����ԭ����
	AVPScale4_3			= 0x3,			// 4:3����
	AVPScale16_9		= 0x4,			// 16:9����
}AVPScaleType;
// ����������� end

// ����ص����� begin
typedef enum
{
	AVPDecCBTypeNone	= 0,			// ��
	AVPDecCBTypeAudio16	= 1,			// ��Ƶ.������16khz,������,ÿ��������16λ��ʾ
	AVPDecCBTypeRGB32	= 2,			// ��Ƶ.ÿ����4�ֽ�,���з�ʽ��λͼ����,"BGR0",��һ������λ��ͼ�����½� 
	AVPDecCBTypeYV12	= 3,			// ��Ƶ,yv12��ʽ.����˳��"Y0Y1...","V0V1...","U0U1..."
	AVPDecCBTypeUYVY	= 4,			// ��Ƶ,uyvy��ʽ.����˳��"U0Y0V0Y1U2Y2V2Y3......",��һ������λ��ͼ�����Ͻ�
	AVPDecCBTypeYUV420	= 5,			// ��Ƶ,YUV420��ʽ.����˳��"Y0Y1...","U0U1...","V0V1..."
	AVPDecCBTypeYUYV	= 6,			// ��Ƶ,yuy2��yuyv��ʽ.����˳��"Y0 U0 Y1 V0 Y2 U2 Y3 V2 ... ... ",��һ������λ��ͼ�����Ͻ�.
}AVPDecCBType;
// ����ص����� end

// ����˿ڻص���Ϣ begin
typedef enum 
{
	AVPPlayCBUnknow		= 0,			// δ֪��Ϣ
	AVPPlayCBError		= 1,			// ֪ͨ ���� ��Ϣ,����ֵ�޾�������
	AVPPlayCBStart		= 2,			// ֪ͨ ��ʼ���� ��Ϣ,����ֵ�޾�������,lParam=�ļ��Ŀ�ʼʱ��
	AVPPlayCBEnd		= 3,			// �Ѿ����õ���Ϣ
	AVPPlayCBFrame		= 4,			// ֪ͨ ֡ʱ�䷢���仯(��) ��Ϣ,����ֵ�޾�������,lParam=��ǰʱ��
	AVPPlayCBStop		= 5,			// ֪ͨ �������� ��Ϣ,����ֵ�޾�������,lParam=��
	AVPPlayCBPause		= 6,			// ֪ͨ �������� ��Ϣ,����ֵ�޾�������,lParam=�ļ��Ѳ���ʱ��
	AVPPlayCBContinue	= 7,			// ֪ͨ �������� ��Ϣ,����ֵ�޾�������,lParam=�ļ��Ѳ���ʱ��
	AVPPlayCBSpeed		= 8,			// ֪ͨ �����ٶ� ��Ϣ,����ֵ�޾�������,lParam=��ǰ�����ٶ�
}AVPPlayCBMsg;
// ����˿ڻص���Ϣ end

// �����ļ����Ž���ʱ�Ĵ����� begin
typedef enum
{
	AVPOnEndFileClose	= 0x0,			// �ر�
	AVPOnEndFileReopen	= 0x1,			// ���´�
	AVPOnEndFileNext	= 0x2,			// ��һ�ļ�
}AVPOnEndFileAction;
// �����ļ����Ž���ʱ�Ĵ����� end

// ������Ƶ����ģʽ begin
typedef enum
{
	AVPPlayRealTime		= 0x0,			// ʵʱ����
	AVPPlaySmooth		= 0x1,			// ��������
}AVPPlayPriority;
// ������Ƶ����ģʽ end

// ����¼���ļ�ת����ʽ begin
typedef enum
{
	AVPConverUnknown	= 0,			// δ����
	AVPConverTrunk		= 1,			// ��Ԥ�����¼���г�ȡ
	AVPConverMKV		= 2,			// ת��ΪMKV��ʽ
	AVPConverAVI		= 3,			// ת��ΪAVI��ʽ(��ʱ��֧��)
}AVPConverType;
// ����¼���ļ�ת����ʽ end

// ��ʾ�豸���� begin
typedef struct stDisplayDev
{
	GUID			stGUID;								// ��ʾ�豸֮GUID
	char			szDescription[AVPDisDevStrLen];		// ��ʾ�豸֮����
	char			szDriveName[AVPDisDevStrLen];		// ��ʾ�豸֮��������
	RECT			rcMonitor;							// ��ʾ��������
	RECT			rcWork;								// ��ʾ���Ĺ�������
	DWORD			dwFlags;							// ��ʾ���ı�־λ,MONITORINFOF_PRIMARY ��ʾΪ����ʾ��
	DWORD			dwData;								// ˽��������
	stDisplayDev()
	{
		memset(&stGUID,0,sizeof(GUID));					// ��ʾ�豸֮GUID
		memset(szDescription,0,AVPDisDevStrLen);		// ��ʾ�豸֮����
		memset(szDriveName,0,AVPDisDevStrLen);			// ��ʾ�豸֮��������
		memset(&rcMonitor,0,sizeof(RECT));				// ��Ӧ��ʾ����Ϣ
		memset(&rcWork,0,sizeof(RECT));					// ��Ӧ��ʾ����Ϣ
		dwFlags		= NULL;								// ��Ӧ��ʾ����Ϣ
		dwData		= NULL;								// ˽��������
	}
}* PSTDisplayDev;
// ��ʾ�豸���� end

// ����ص��������ò��� begin
typedef struct stDecFrameCB
{
	int			iPort;					// ����ͨ����
	LPBYTE		lpBuf;					// ����������
	long		lSize;					// ��������ݳ�
	long		lWidth;					// ��
	long		lHeight;				// ��
	long		lStamp;					// ֡ʱ��
	long		lType;					// AVPDecCBType
	long		lParam;					// �û��Զ������
	DWORD		dwRes0;					// ��������0
	DWORD		dwRes1;					// ��������1
	DWORD		dwRes2;					// ��������2
	DWORD		dwRes3;					// ��������3
}* PSTDecFrameCB;
// ����ص��������ò��� end

// ��ʾ�ص��������ò��� begin
typedef struct stDisplayCB
{
	long		lpWnd;					// CWnd*
	void*		hDC;					// HDC
	RECT		rtTag;					// Ŀ������
	RECT		rtSrc;					// Դ����
	int			iScaleType;				// ��������
	time_t		tDevTime;				// �豸ʱ��
}* PSTDisplayCB;
// ��ʾ�ص��������ò��� end

// �ļ������ص��������ò��� begin
typedef struct stFileEndCB
{
	int			iPort;					// ����ͨ����
	char		szNextPath[MAX_PATH];	// ��һ�ļ�·��
	long		lOffset;				// ƫ��λ��
	AVPOnEndFileAction	eAction;		// ��һ��������
	DWORD		dwUserData;				// �û�����
}* PSTFileEndCB;
// �ļ������ص��������ò��� end

// ¼���ļ���ʽת���ص��������ò��� begin
typedef struct stConverRecCB
{
	int			iPort;					// ����ͨ����
	AVPConverType eType;				// ת������
	int			iStop;					// �Ƿ����
	int			iPercent;				// �ٷֱ�
	void*		pUserData;				// �û�����
}* PSTConverRecCB;
// ¼���ļ���ʽת���ص��������ò��� end

// �ص����� begin
// ¼���ص�����
typedef void (CALLBACK fcbPickAudio)(const BYTE* pBuffer, DWORD dwBufSize, void* pUserData);
// ����ص�����,0��ʾ��������,1��ʾ���ܼ�������
typedef int(CALLBACK fcbDecFrame)(PSTDecFrameCB pstDecFrameCB);
// ��ʾ�ص�����,0��ʾ��������,1��ʾ���ܼ�������
typedef int(CALLBACK fcbDisplay)(PSTDisplayCB pstDisplayCB);
// ���Żص�����
typedef int(CALLBACK fcbPlay)(AVPPlayCBMsg eMsg,int iPlayerID,LPARAM lpUserParam,__int64 lParam);
// �ļ����Ž���ʱ�ص�����
typedef void(CALLBACK fcbOnEndFile)(int iPort,PSTFileEndCB pstFileEnd);
// ¼���ļ���ʽת���ص�����,����0��ʾ����,����1��ʾ����
typedef int(CALLBACK fcbOnConverFile)(int iPort,PSTConverRecCB pstConver);
// ����OSD��Ϣ��ʾ�ص�����,����0��ʾ����������,����1��ʾû�н�������
typedef int(CALLBACK fcbOnDrawExtOsd)(HDC hDC,long lWidth,long lHeight,LPCTSTR lpszOSD,DWORD dwTime,void* pUserData);
// �ص����� end


// ϵͳ���� begin
// ��ʼ��
AVPLAYER_API int AVP_Init();
// ��ʼ��¼�����ģ��
AVPLAYER_API int AVP_InitRecMng(int iCount,int iBufMBs);
// ��ʼ��ץ�Ĺ���ģ��
AVPLAYER_API int AVP_InitCaptureMng(int iCount,LPCTSTR lpszCoderPath);
// ע������
AVPLAYER_API int AVP_Cleanup();
// ��ȡ���е���ʾ�豸,�����豸�б���ܴ�С
AVPLAYER_API int AVP_GetDisplayDevList(PSTDisplayDev pstDisDev,int iCount);
// ϵͳ���� end


// �������� begin
// ��ȡ�ļ���Ϣ
AVPLAYER_API int AVP_GetFileInfo(LPCTSTR lpszFilePath, time_t* plStart, time_t* plEnd, long* plSize, long* plProtocal, long* plEncoder);
// ��ȡ�ļ�ͷ����,�����ڴ���¼���ļ�
AVPLAYER_API int AVP_GetFileHeadContent(LPTSTR lpszFileHead);
// �������� end


// ��Ƶ�ɼ����� begin
// ��ʼ��Ƶ�ɼ�
AVPLAYER_API BOOL AVP_StartPickAudio(UINT nSamples,BOOL bAddHeader,fcbPickAudio* fcb,void* pUserData);
// ������Ƶ�ɼ�
AVPLAYER_API BOOL AVP_StopPickAudio();
// ��Ƶ�ɼ����� end


// ͨ������ begin
// ����������
AVPLAYER_API int AVP_GetFreePort();
// �ͷŲ�����
AVPLAYER_API int AVP_ReleasePort(int iPort);
// ���ý���������
AVPLAYER_API int AVP_SetCoder(int iPort, LPCTSTR lpszCoderName);
// ��ȡ����������
AVPLAYER_API int AVP_GetCoder(int iPort, LPTSTR lpszCoderName);
// ��������Э��(��ʽͷ)
AVPLAYER_API int AVP_SetDataProtocol(int iPort, DWORD dwProtocol);
// ��ȡ����Э��(��ʽͷ)
AVPLAYER_API DWORD AVP_GetDataProtocol(int iPort);
// ����������
AVPLAYER_API int AVP_GetStreamType(int iPort);
// ���ز���״̬
AVPLAYER_API int AVP_GetPlayStatus(int iPort);
// ͨ������ end


// ������� begin
// ����һ֡
AVPLAYER_API int AVP_PutFrame(int iPort, void* pFrame);
AVPLAYER_API int AVP_SetDecCallBack(int iPort, fcbDecFrame* pFun, long lParam);
// ������� end


// ý������� begin
// ���һ����ʾ����
AVPLAYER_API int AVP_AddPlayWnd(int iPort, PSTDisplayDev pstDispDev,HWND hWnd,LPRECT lpDrawOffset,LPRECT lpSrcOffset);
// ɾ��һ����ʾ����
AVPLAYER_API int AVP_DelPlayWnd(int iPort, HWND hWnd);
// ���ò�����λ��
AVPLAYER_API int AVP_SetWndPosOffset(int iPort, int iPlayWndID, RECT* lpRect);
// ����Դλ�ü����챶��
AVPLAYER_API int AVP_SetSrcPosOffset(int iPort, int iPlayWndID, RECT* lpRect);
// ��ȡ������λ��
AVPLAYER_API int AVP_GetWndPosOffset(int iPort, int iPlayWndID, RECT* lpRect);
// ��ȡԴλ�ü����챶��
AVPLAYER_API int AVP_GetSrcPosOffset(int iPort, int iPlayWndID, RECT* lpRect);
// ������Ƶ
AVPLAYER_API int AVP_EnableVideo(int iPort, BOOL bEnable);
// ��Ƶ�Ƿ�����
AVPLAYER_API BOOL AVP_IsVideoEnable(int iPort);
// ������Ƶ
AVPLAYER_API int AVP_EnableAudio(int iPort, BOOL bEnable);
// ��Ƶ�Ƿ�����
AVPLAYER_API BOOL AVP_IsAudioEnable(int iPort);
// ��ȡ����
AVPLAYER_API int AVP_GetVolume(int iPort);
// ��������
AVPLAYER_API int AVP_SetVolume(int iPort, int iVolume);
// ������Ƶ��ʾ
AVPLAYER_API int AVP_EnableDisplay(int iPort, BOOL bEnable);
// ˢ�´���
AVPLAYER_API int AVP_RefreshDisplay(int iPort);
// �������췽ʽ
AVPLAYER_API int AVP_SetScaleType(int iPort, int iPlayWndID, int iScaleType);
// ��ȡ���췽ʽ
AVPLAYER_API int AVP_GetScaleType(int iPort, int iPlayWndID);
// ���Ѻ�
AVPLAYER_API BOOL AVP_SetNoTearing(int iPort, int iPlayWndID, BOOL bEnable);
// ��ȡ��ǰ���Ѻ�����
AVPLAYER_API BOOL AVP_IsNoTearing(int iPort, int iPlayWndID);
// ������ʾ�Ļص�����
AVPLAYER_API int AVP_SetDisplayCallBack(int iPort, int iPlayWndID, fcbDisplay* pFun, PSTDisplayCB pstDisplayCB);
// ������Ƶ����ģʽ
AVPLAYER_API int AVP_SetPlayPriority(int iPort, int iPriorityType);
// ��ȡ��Ƶ����ģʽ
AVPLAYER_API int AVP_GetPlayPriority(int iPort);
// ��ȡ�������ڵ�֡��
AVPLAYER_API int AVP_GetBuffSpan(int iPort, int* piMaxSpan);
// �����Ƿ�ʹ��YUV���
AVPLAYER_API int AVP_EnableYUVDraw(int iPort,BOOL bEnable);
// ��ȡ�Ƿ�ʹ��YUV���
AVPLAYER_API BOOL AVP_IsYUVDrawEnable(int iPort);
// ���ö���OSD��Ϣ��ʾ�ص�����
AVPLAYER_API BOOL AVP_SetDrawExtOsd(fcbOnDrawExtOsd* pcbDrawExtOsd,void* pData);
// ��ջ���
AVPLAYER_API int AVP_ClearBuff(int iPort);
// ý������� end


// ¼���ļ����� begin
// д�ļ� begin
// ����¼���ļ�,ָ���ļ���С,0 ��ʾ��Ԥ����
AVPLAYER_API int AVP_CreateRecFileSize(LPCTSTR lpszPath,DWORD dwProtocol,DWORD dwEncoder,long lFileMaxSize);
// ����¼���ļ�
AVPLAYER_API int AVP_CreateRecFile(LPCTSTR lpszPath,DWORD dwProtocol,DWORD dwEncoder);
// ����¼���ļ�,ָ���ļ�ʱ��(��),0 ��ʾ����������
AVPLAYER_API int AVP_CreateRecFileTime(LPCTSTR lpszPath,DWORD dwProtocol,DWORD dwEncoder,long lFileMaxTime);
// д��¼��
AVPLAYER_API int AVP_WriteRecFile(int iFile,void* lpFrame,int iFrameLen,void* lpBuf2,int iBufLen2);
// �ر�¼���ļ�
AVPLAYER_API int AVP_CloseRecFile(int iFile);
// �ļ���ʽת��
AVPLAYER_API int AVP_ConverJAV(LPCTSTR lpszSrc,LPCTSTR lpszTag,AVPConverType eType,DWORD dwRes,fcbOnConverFile* pFun,void* pUser);
// ֹͣ�ļ���ʽת��
AVPLAYER_API int AVP_ConverStop(int iPort);
// д�ļ� end
// �����ļ�
AVPLAYER_API int AVP_PlayFileName(int iPort, LPCTSTR lpszPath,long lFileOffset);
// ��ȡ��֡��
AVPLAYER_API DWORD AVP_GetTotalFrame(int iPort);
// ��ȡ��ǰ֡��
AVPLAYER_API DWORD AVP_GetCurFrame(int iPort);
// ��ȡ֡��
AVPLAYER_API int AVP_GetFrameRate(int iPort);
// ��ȡ�ļ���ʱ��
AVPLAYER_API DWORD AVP_GetFileTotalTimeS(int iPort);
// ��ȡ�ļ��Ŀ�ʼ����ʱ��
AVPLAYER_API time_t AVP_GetFileStartTimeS(int iPort);
// ��ȡ��ǰ����ʱ��
AVPLAYER_API DWORD AVP_GetCurPlayTimeS(int iPort);
// ��ȡ��ǰOSD��Ϣ
AVPLAYER_API LPCTSTR AVP_GetCurOSD(int iPort);
// ��ȡ��ǰд��ʱ��
AVPLAYER_API DWORD AVP_GetCurFrameWriteTime(int iPort);
// ���õ������ļ�����ʱ�Ļص�����
AVPLAYER_API int AVP_SetOnEndFileCallBack(int iPort, fcbOnEndFile* pFun,LPARAM lpParam);
// ¼���ļ����� end


// ý�岥�ſ��Ʋ��� begin
// ����
AVPLAYER_API int AVP_Play(int iPort);
// ��ͣ
AVPLAYER_API int AVP_Pause(int iPort);
// ֹͣ
AVPLAYER_API int AVP_Stop(int iPort);
// ץͼ
AVPLAYER_API int AVP_Capture(int iPort, LPCTSTR lpszPath);
// ��һ֡
AVPLAYER_API int AVP_NextFrame(int iPort);
// �ƶ���ָ��λ��
AVPLAYER_API int AVP_Seek(int iPort, DWORD dwTime);
// ���ò����ٶ�
AVPLAYER_API int AVP_SetPlaySpeed(int iPort, int iSpeed);
// ��ȡ��ǰ�����ٶ�
AVPLAYER_API int AVP_GetPlaySpeed(int iPort);
// ���ò��ŵĻص�����
AVPLAYER_API int AVP_SetPlayCallBack(int iPort, fcbPlay* pFun,LPARAM lpParam);
// ý�岥�ſ��Ʋ��� end

// ץ��ͨ������ begin
// ����ץ��ͨ��
AVPLAYER_API int AVP_CreateCapture(DWORD dwProtocol,DWORD dwEncoder);
// ��ץ��ͨ��������Ƶ֡
AVPLAYER_API int AVP_PutCaptureFrame(int iPort, void* pFrame,LPCTSTR lpszOSD,DWORD dwWriteTime);
// ��ʼץ��
AVPLAYER_API int AVP_CapturePort(int iPort, LPCTSTR lpszPath,BOOL bSync);
// �ر�ץ��ͨ��
AVPLAYER_API int AVP_CloseCapture(int iPort);
// ץ��ͨ������ end

#endif		// __AV_PLAYER_H__
