
#include "SearchDevicesIO.h"
#include "SearchDevices.h"
#include "log.h"

#include <WinSock.h>

using namespace Loggering;
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

long SearchDeviceIO::m_nRef = 0;

bool SearchDeviceIO::GetDevice(char *szData, int nBufLen, int* pRetLen)
{
	assert(szData);
	assert(nBufLen != 0);	

	int nLen = 0;

	try
	{
		
		Poco::ThreadPool pool;
		pool.start(m_DhSearchDevice);
		pool.start(m_DZPSearchDevice);
		pool.start(m_JXJSearchDevice);

		pool.joinAll();

		Sleep(3000);
		m_DhSearchDevice.SearchdeviceStop();
		m_DZPSearchDevice.SearchdeviceStop();
		m_JXJSearchDevice.SearchdeviceStop();

		list<NET_DEVICE_INFO> DhList = m_DhSearchDevice.GetDeviceList();
		list<NET_DEVICE_INFO> DzpList = m_DZPSearchDevice.GetDeviceList();
		list<NET_DEVICE_INFO> JxjList = m_JXJSearchDevice.GetDeviceList();

		list<NET_DEVICE_INFO> DevList;


		list<NET_DEVICE_INFO>::iterator it;
		for (it = DhList.begin(); it != DhList.end(); it++)
		{
			DevList.push_back((NET_DEVICE_INFO)*it);
		}

		for (it = DzpList.begin(); it != DzpList.end(); it++)
		{
			DevList.push_back((NET_DEVICE_INFO)*it);
		}

		for (it = JxjList.begin(); it != JxjList.end(); it++)
		{
			DevList.push_back((NET_DEVICE_INFO)*it);
		}

		DevList.sort(SortByIp);

		DevList.unique(UniqueByIp);


		for (it = DevList.begin(); it != DevList.end(); it++)
		{
			memcpy(szData + nLen, &(*it), sizeof(NET_DEVICE_INFO));
			nLen += sizeof(NET_DEVICE_INFO);
		}

				
	}
	catch (exception& e)
	{
		poco_error(logger_handle, e.what());
		*pRetLen = 0;
		return false;
	}



	if (nBufLen < nLen)
		return false;
	
	*pRetLen = nLen;

	return TRUE;
}

bool SearchDeviceIO::GetDeviceToJson(std::string path)
{
	return TRUE;
}



void SearchDeviceIO::Release()
{
	delete this;
}

SearchDeviceIO::SearchDeviceIO()
{
	Setup_logger();
	memset(m_path, 0, 512);
	sprintf_s(m_path, "factorys\\");	
	
	
	if (1 == InterlockedIncrement(&m_nRef))
	{
		poco_information(logger_handle, "start");
		/*m_DhSearchDevice(DH_SDK);
		m_DZPSearchDevice(DZP_SDK);
		m_JXJSearchDevice(JXJ_SDK);*/
		m_DhSearchDevice.SetFactoryId(DH_SDK);
		m_DZPSearchDevice.SetFactoryId(DZP_SDK);
		m_JXJSearchDevice.SetFactoryId(JXJ_SDK);

		if (strlen(m_path) > 0)
		{
			poco_information(logger_handle, "start init");
			m_DhSearchDevice.SearchDeviceInit(m_path);
			m_DZPSearchDevice.SearchDeviceInit(m_path);
			m_JXJSearchDevice.SearchDeviceInit(m_path);
		}

	}
	
}

SearchDeviceIO::~SearchDeviceIO()
{
	if (0 == InterlockedDecrement(&m_nRef))
	{
		poco_information(logger_handle, "clean up");
		m_DhSearchDevice.SearchDeviceCleanUp();
		m_DZPSearchDevice.SearchDeviceCleanUp();
		m_JXJSearchDevice.SearchDeviceCleanUp();
	}
}

void SearchDeviceIO::SetDllPath(char *strDllpath)
{
	assert(strlen(strDllpath) <= 512 && strlen(strDllpath) > 0);
	
	strncpy(m_path, strDllpath, strlen(strDllpath));
	
}

bool SearchDeviceIO::SortByIp(NET_DEVICE_INFO ndiFirst, NET_DEVICE_INFO ndiSecond)
{
	unsigned long lIp1 = inet_addr(ndiFirst.szIp);
	unsigned long lIp2 = inet_addr(ndiSecond.szIp);
	return lIp1 < lIp2;
}

bool SearchDeviceIO::UniqueByIp(NET_DEVICE_INFO ndiFirst, NET_DEVICE_INFO ndiSecond)
{
	unsigned long lIp1 = inet_addr(ndiFirst.szIp);
	unsigned long lIp2 = inet_addr(ndiSecond.szIp);

	return lIp1 == lIp2;
}

LIBRARY_API SearchDeviceInterface* _stdcall CreateSearchObj()
{
	return new SearchDeviceIO;
}

LIBRARY_API void _stdcall DestroySearchObj(SearchDeviceInterface* pExport)
{
	pExport->Release();
}

// LIBRARY_API bool ReturnSearch(char *szData, int nBuflen, int* pRetLen);