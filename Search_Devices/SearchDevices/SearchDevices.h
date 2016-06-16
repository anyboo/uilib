#ifndef SearchDevices_INCLUDED
#define SearchDevices_INCLUDED

#include <iostream>
#include <list>
#include <assert.h>
#include "poco/SharedLibrary.h"
#include "Poco/Thread.h"  
#include "Poco/Runnable.h"
#include "Poco/ThreadPool.h"
#include "Poco/Path.h"
#include <WinSock.h>

#include "SearchDevicesImpl.h"

#define CALL_METHOC  __cdecl




//DA HUA 
namespace DH
{
#include "DHVideoServer/dhnetsdk.h"

	// 
	typedef BOOL(CALL_METHOD *PCLIENT_Init)(DH::fDisConnect cbDisConnect, LDWORD dwUser);

	// start sync search IPC¡¢NVS devices
	typedef LLONG(CALL_METHOD *PCLIENT_StartSearchDevices)(DH::fSearchDevicesCB cbSearchDevices, void* pUserData, char* szLocalIp);
	// stop sync search IPC¡¢NVS devices
	typedef BOOL(CALL_METHOD *PCLIENT_StopSearchDevices)(LLONG lSearchHandle);

	// 
	typedef void (CALL_METHOD *PCLIENT_Cleanup)();
}

//jie xin jie
namespace JXJ
{
#include "JXJVideoServer/JNetSDK.h"
#include "JXJVideoServer/mb_api.h"


	//sdk init
	typedef int(CALL_METHOC *PJNetInit)(LPCTSTR lpszLogName);
	// sdk cleanup
	typedef int(CALL_METHOC *PJNetCleanup)();
	//

	// broadcast set	
	typedef long (CALL_METHOC *PJNetMBOpen)(LPCTSTR lpszMuIP, int iMuPort, JXJ::fcbJMBNotify* pfcbMBNotify, void* pUserParam, DWORD dwProtocol, long& lMBHandle);
	// broadcast stop
	typedef long (CALL_METHOC *PJNetMBClose)(long lMBHandle);
	// broadcast search
	typedef long (CALL_METHOC *PJNetMBSearch)(long lMBHandle, int iTimeOut);
}

//Di Zhi Pu
namespace DZP
{
#include "DiZhiPuVideoServer/netsdk.h"

	//*** */SDK init
	typedef long (CALL_METHOD *PH264_DVR_Init)(fDisConnect cbDisConnect, unsigned long dwUser);

	//*** */SDK clean up
	typedef bool (CALL_METHOD *PH264_DVR_Cleanup)();

	//search network devices
	typedef bool (CALL_METHOD *PH264_DVR_SearchDevice)(char* szBuf, int nBufLen, int* pRetLen, int nSearchTime);

}


using Poco::SharedLibrary;
using Poco::Path;



class  SearchDevices : public Poco::Runnable
{
public:
	
	SearchDevices();

	SearchDevices(int id);

	BOOL SearchDeviceInit(char *strPath = NULL);

	BOOL SearchdeviceStop();

	BOOL SearchDeviceCleanUp();

	VOID SetFactoryId(int id);

	virtual void run();

	std::list<NET_DEVICE_INFO> GetDeviceList();
	
private:
	BOOL DH_Loadlibrary(char *strPath = NULL);

	BOOL DZP_Loadlibrary(char *strPath = NULL);

	BOOL JXJ_Loadlibrary(char *strPath = NULL);

	BOOL DH_Init();

	BOOL DZP_Init();

	BOOL JXJ_Init();	

	int DZP_Start_Searchdevice();

	int DH_Start_Searchdevice(void *pUserData);

	int JXJ_Start_Searchdevice(void *pUserData);

	BOOL DH_Stop_Searchdevice();

	int DZP_Stop_Searchdevice();

	long JXJ_Stop_Searchdevice();

	void DH_Freelibrary();

	void DZP_Freelibrary();

	void JXJ_Freelibrary();

	void DH_CleanUp();

	void DZP_CleanUp();

	void JXJ_CleanUp();


	static long  CALLBACK DH_SearchDevicesCB(DH::DEVICE_NET_INFO_EX *pDevNetInfo, void *pUserData);

	static int CALLBACK JXJ_fcbJMBNotify(long lHandle, DWORD dwPortocol, int iErr, int iMsgID, LPCTSTR lpszDstID, void* pData, int iDataLen, void* pUserParam);
	

private:
	//dahua dll (sdk DHVideoServer)
	SharedLibrary m_DHLib;

	//jiaxinjie dll (sdk JXJVideoServer)
	SharedLibrary m_JXJLib;

	//dizhipu dll (sdk DiZhiPuVideoServer)
	SharedLibrary m_DZPLib;

	//dahua search handle
	long m_DHSearchHandle;

	//jxj search handle
	long m_JXJSearchHandle;

	// net device type
	int m_id;

	//output data
	std::list<NET_DEVICE_INFO> m_list;


};



#endif