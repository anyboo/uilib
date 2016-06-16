#ifndef SearchDevices_IO_INCLUDED
#define SearchDevices_IO_INCLUDED



#include <iostream>
#include "SearchDevicesImpl.h"
#include "SearchDevices.h"


class SearchDeviceIO : public SearchDeviceInterface
{

public:
	virtual bool GetDevice(char *szData, int nBufLen, int* pRetLen);
	virtual bool GetDeviceToJson(std::string path = NULL);
	virtual void Release();	
	virtual void SetDllPath(char *strDllpath);
	SearchDeviceIO();
	~SearchDeviceIO();	
private:
	static bool SortByIp(NET_DEVICE_INFO ndiFirst, NET_DEVICE_INFO ndiSecond);
	static bool UniqueByIp(NET_DEVICE_INFO ndiFist, NET_DEVICE_INFO ndiSecond);
	
	// dll path
	char m_path[512];	
protected:
	static long m_nRef;
	SearchDevices m_DhSearchDevice;
	SearchDevices m_DZPSearchDevice;
	SearchDevices m_JXJSearchDevice;
};


#endif