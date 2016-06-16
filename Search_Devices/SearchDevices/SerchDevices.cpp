

#include "SearchDevices.h"
#include "log.h"
#include "SearchDevicesIo.h"

using namespace Loggering;
using namespace std;



SearchDevices::SearchDevices()
{
	m_DHSearchHandle = 0;
	m_JXJSearchHandle = 0;	
}


SearchDevices::SearchDevices(int id)
{
	m_DHSearchHandle = 0;
	m_JXJSearchHandle = 0;
	m_id = id;
}

VOID SearchDevices::SetFactoryId(int id)
{
	m_id = id;
}

BOOL SearchDevices::SearchDeviceInit(char *strPath)
{	
	switch (m_id)
	{
	case DH_SDK:		
		DH_Loadlibrary(strPath);
		DH_Init();
		break;
	case DZP_SDK :
		DZP_Loadlibrary(strPath);
		DZP_Init();
		break;
	case JXJ_SDK :
		JXJ_Loadlibrary(strPath);
		JXJ_Init();
		 break;
	default:
		break;
	}
	return TRUE;
}

BOOL SearchDevices::SearchdeviceStop()
{
	switch (m_id)
	{
	case DH_SDK:
		DH_Stop_Searchdevice();
		break;
	case DZP_SDK:
		DZP_Stop_Searchdevice();
		break;
	case JXJ_SDK:
		JXJ_Stop_Searchdevice();
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL SearchDevices::SearchDeviceCleanUp()
{
	switch (m_id)
	{
	case DH_SDK:
		DH_CleanUp();
		DH_Freelibrary();
		break;
	case DZP_SDK:
		DZP_CleanUp();
		DZP_Freelibrary();
		break;
	case JXJ_SDK:
		JXJ_CleanUp();
		JXJ_Freelibrary();
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL SearchDevices::DH_Loadlibrary(char *strPath)
{
	std::string strDllpath = Path::current();	
	strDllpath.append(strPath);		
	strDllpath.append("DH\\dhnetsdk.dll");
	try
	{
		m_DHLib.load(strDllpath);
		assert(m_DHLib.isLoaded());
	}
	catch (Poco::Exception& e)
	{		
		poco_error(logger_handle,  e.displayText());
		return FALSE;
	}
	
	return TRUE;
}

BOOL SearchDevices::DZP_Loadlibrary(char *strPath)
{
	std::string strDllpath = Path::current();
	strDllpath.append(strPath);	
	strDllpath.append("DiZhiPu\\NetSdk.dll");
	try
	{
		m_DZPLib.load(strDllpath);
		assert(m_DZPLib.isLoaded());
	}
	catch (Poco::Exception& e)
	{
		poco_error(logger_handle, e.displayText());
		return FALSE;
	}

	return TRUE;
}

BOOL SearchDevices::JXJ_Loadlibrary(char *strPath)
{
	std::string strDllpath = Path::current();
	strDllpath.append(strPath);	
	strDllpath.append("JiaXJ\\JNetSDK.dll");
	try
	{
		m_JXJLib.load(strDllpath);
		assert(m_JXJLib.isLoaded());
	}
	catch (Poco::Exception& e)
	{
		poco_error(logger_handle, e.displayText());
		return FALSE;
	}

	return TRUE;
}

BOOL SearchDevices::DH_Init()
{
	assert(m_DHLib.hasSymbol("CLIENT_Init"));
	DH::PCLIENT_Init pDHInit = (DH::PCLIENT_Init)m_DHLib.getSymbol("CLIENT_Init");
	return pDHInit(NULL, NULL);
}

BOOL  SearchDevices::DZP_Init()
{
	assert(m_DZPLib.hasSymbol("H264_DVR_Init"));
	DZP::PH264_DVR_Init pDZPInit = (DZP::PH264_DVR_Init)m_DZPLib.getSymbol("H264_DVR_Init");	
	return pDZPInit(NULL, NULL);
}

BOOL SearchDevices::JXJ_Init()
{
	assert(m_JXJLib.hasSymbol("JNetInit"));
	JXJ::PJNetInit pJXJInit = (JXJ::PJNetInit)m_JXJLib.getSymbol("JNetInit");		
	return pJXJInit(NULL);
}



BOOL SearchDevices::DH_Start_Searchdevice(void *pUserData)
{
	DH::PCLIENT_StartSearchDevices pStartSearchDevice = (DH::PCLIENT_StartSearchDevices)m_DHLib.getSymbol("CLIENT_StartSearchDevices");
	m_DHSearchHandle = pStartSearchDevice((DH::fSearchDevicesCB)DH_SearchDevicesCB, pUserData, NULL);
	return TRUE;
}


int SearchDevices::DZP_Start_Searchdevice()
{
	DZP::PH264_DVR_SearchDevice pDzpSearchDevice = (DZP::PH264_DVR_SearchDevice)m_DZPLib.getSymbol("H264_DVR_SearchDevice");
	DZP::SDK_CONFIG_NET_COMMON_V2 Device[256] = { 0 };
	int nRetLength = 0;
	


	BOOL bRet = pDzpSearchDevice((char*)Device, sizeof(DZP::SDK_CONFIG_NET_COMMON_V2) * 256, &nRetLength, 3000);
	if (bRet)
	{
		int i;
		for (i = 0; i < nRetLength / sizeof(DZP::SDK_CONFIG_NET_COMMON_V2); i++)
		{
			NET_DEVICE_INFO ndiInfo = { 0 };
			int nLen = 0;
			ndiInfo.nSDKType = DZP_SDK;
			
			struct in_addr in1 = { 0 };
			in1.s_addr = Device[i].HostIP.l;
			nLen = (strlen(inet_ntoa(in1)) < MAX_IPADDR_LEN) ? strlen(inet_ntoa(in1)) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo.szIp, inet_ntoa(in1), nLen);
			

			ndiInfo.nPort = Device[i].TCPPort;

			struct in_addr in2 = { 0 };
			in2.s_addr = Device[i].Submask.l;
			nLen = (strlen(inet_ntoa(in2)) < MAX_IPADDR_LEN) ? strlen(inet_ntoa(in2)) : MAX_IPADDR_LEN;
			memcpy(&ndiInfo.szSubmask, inet_ntoa(in2), nLen);

			nLen = (strlen(Device[i].sMac) < MAX_MACADDR_LEN) ? strlen(Device[i].sMac) : MAX_MACADDR_LEN;
			memcpy(&ndiInfo.szMac, &Device[i].sMac, nLen);
			
			m_list.push_back(ndiInfo);
		}
	}
	return 0;
}

int SearchDevices::JXJ_Start_Searchdevice(void *pUserData)
{
	JXJ::PJNetMBOpen pJxjMBOpen = (JXJ::PJNetMBOpen)m_JXJLib.getSymbol("JNetMBOpen");
	JXJ::PJNetMBSearch pJxjMBSearch = (JXJ::PJNetMBSearch)m_JXJLib.getSymbol("JNetMBSearch");
	char szIp[11] = "224.0.0.99";
	int nPort = 40086;
	
	long lResult = pJxjMBOpen(szIp, nPort, JXJ_fcbJMBNotify, pUserData, JNET_PRO_T_JPF, m_JXJSearchHandle);
	lResult = pJxjMBSearch(m_JXJSearchHandle, 5);
	return 0;
}

BOOL SearchDevices::DH_Stop_Searchdevice()
{
	DH::PCLIENT_StopSearchDevices pStopSearchDevice = (DH::PCLIENT_StopSearchDevices)m_DHLib.getSymbol("CLIENT_StopSearchDevices");
	return pStopSearchDevice(m_DHSearchHandle);	
}

int SearchDevices::DZP_Stop_Searchdevice()
{
	
	return 0;
}

long SearchDevices::JXJ_Stop_Searchdevice()
{
	JXJ::PJNetMBClose pStopSearchDevice = (JXJ::PJNetMBClose)m_JXJLib.getSymbol("JNetMBClose");
	return pStopSearchDevice(m_JXJSearchHandle);	
}

void SearchDevices::DH_CleanUp()
{
	DH::PCLIENT_Cleanup pDHCleanUp = (DH::PCLIENT_Cleanup)m_DHLib.getSymbol("CLIENT_Cleanup");
	pDHCleanUp();
}

void SearchDevices::DZP_CleanUp()
{
	DZP::PH264_DVR_Cleanup pDZPCleanUp = (DZP::PH264_DVR_Cleanup)m_DZPLib.getSymbol("H264_DVR_Cleanup");
	bool bRet = pDZPCleanUp();
	if (bRet)
		poco_information(logger_handle, "true");
	else
		poco_information(logger_handle, "false");
}

void SearchDevices::JXJ_CleanUp()
{
	JXJ::PJNetCleanup pJXJCleanUp = (JXJ::PJNetCleanup)m_JXJLib.getSymbol("JNetCleanup");
	pJXJCleanUp();
}

void SearchDevices::DH_Freelibrary()
{
	m_DHLib.unload();	
}

void SearchDevices::DZP_Freelibrary()
{
	m_DZPLib.unload();
}

void SearchDevices::JXJ_Freelibrary()
{
	m_JXJLib.unload();
}

long  CALLBACK SearchDevices::DH_SearchDevicesCB(DH::DEVICE_NET_INFO_EX *pDevNetInfo, void *pUserData)
{
	
	SearchDevices *pSD = (SearchDevices *)pUserData;
	NET_DEVICE_INFO ndiInfo = {0};

	assert(pDevNetInfo);
	assert(strlen(pDevNetInfo->szIP) < MAX_IPADDR_LEN);
	assert(strlen(pDevNetInfo->szSubmask) < MAX_IPADDR_LEN);
	assert(strlen(pDevNetInfo->szMac) < MAX_MACADDR_LEN);

	if ((NULL == pDevNetInfo) || (strlen(pDevNetInfo->szIP) == 0))
		return 0;

	ndiInfo.nSDKType = DH_SDK;	
	int nLen = (strlen(pDevNetInfo->szIP) < MAX_IPADDR_LEN) ? strlen(pDevNetInfo->szIP) : MAX_IPADDR_LEN;
	memcpy(&ndiInfo.szIp, &pDevNetInfo->szIP, nLen);

	nLen = (strlen(pDevNetInfo->szSubmask) < MAX_IPADDR_LEN) ? strlen(pDevNetInfo->szSubmask) : MAX_IPADDR_LEN;
	memcpy(&ndiInfo.szSubmask, &pDevNetInfo->szSubmask, nLen);

	nLen = (strlen(pDevNetInfo->szMac) < MAX_MACADDR_LEN) ? strlen(pDevNetInfo->szMac) : MAX_MACADDR_LEN;
	memcpy(&ndiInfo.szMac, &pDevNetInfo->szMac, nLen);
	ndiInfo.nPort = pDevNetInfo->nPort;
	
	pSD->m_list.push_back(ndiInfo);
	
	return 0;
}


int CALLBACK SearchDevices::JXJ_fcbJMBNotify(long lHandle, DWORD dwPortocol, int iErr, int iMsgID, LPCTSTR lpszDstID, void* pData, int iDataLen, void* pUserParam)
{
	SearchDevices *pSD = (SearchDevices *)pUserParam;
	NET_DEVICE_INFO ndiInfo = { 0 };

	
	assert(pData);
	assert(iDataLen);	

	if (NULL == pData || iDataLen == 0)
	{
		return 0;
	}

	ndiInfo.nSDKType = JXJ_SDK;

	JXJ::j_Device_T jdtTmp = { 0 };

	JXJ::j_Device_T *pDev = (JXJ::j_Device_T *)(pData);
	int copy_size = 0;	

	if (iDataLen != sizeof(JXJ::j_Device_T))
	{
		copy_size = sizeof(JXJ::J_SysCfg_T);
		if (pDev->SysSize < sizeof(JXJ::J_SysCfg_T))
		{
			copy_size = pDev->SysSize;

		}
		jdtTmp.SysSize = sizeof(JXJ::J_SysCfg_T);
		memcpy(&jdtTmp.SysCfg, &pDev->SysCfg, copy_size);

		copy_size = sizeof(JXJ::JNetworkInfo);
		if (pDev->NetSize < sizeof(JXJ::JNetworkInfo))
		{
			copy_size = pDev->NetSize;

		}
		jdtTmp.NetSize = sizeof(JXJ::JNetworkInfo);
		memcpy(&jdtTmp.NetworkInfo, &pDev->NetworkInfo, copy_size);
	}
	else
	{
		memcpy(&jdtTmp, pData, sizeof(JXJ::j_Device_T));
	}
	
	int nLen = (strlen((char *)jdtTmp.NetworkInfo.network[0].ip) < MAX_IPADDR_LEN) ? strlen((char *)jdtTmp.NetworkInfo.network[0].ip) : MAX_IPADDR_LEN;
	memcpy(&ndiInfo.szIp, &jdtTmp.NetworkInfo.network[0].ip, nLen);

	nLen = (strlen((char *)jdtTmp.NetworkInfo.network[0].netmask) < MAX_IPADDR_LEN) ? strlen((char *)jdtTmp.NetworkInfo.network[0].netmask) : MAX_IPADDR_LEN;
	memcpy(&ndiInfo.szSubmask, &jdtTmp.NetworkInfo.network[0].netmask, nLen);

	nLen = (strlen((char *)jdtTmp.NetworkInfo.network[0].mac) < MAX_MACADDR_LEN) ? strlen((char *)jdtTmp.NetworkInfo.network[0].mac) : MAX_MACADDR_LEN;
	memcpy(&ndiInfo.szMac, &jdtTmp.NetworkInfo.network[0].mac, nLen);
	ndiInfo.nPort = jdtTmp.NetworkInfo.cmd_port;

	pSD->m_list.push_back(ndiInfo);

	return 0;
}

void SearchDevices::run()
{
	switch (m_id)
	{
	case DH_SDK:
		DH_Start_Searchdevice(this);
		break;
	case DZP_SDK:
		DZP_Start_Searchdevice();
		break;
	case JXJ_SDK:
		JXJ_Start_Searchdevice(this);
		break;
	default:
		break;
	}

}

std::list<NET_DEVICE_INFO> SearchDevices::GetDeviceList()
{
	return m_list;
}