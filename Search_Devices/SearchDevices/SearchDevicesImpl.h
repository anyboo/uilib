#ifndef SearchDevices_IMPL_INCLUDED
#define SearchDevices_IMPL_INCLUDED

#include <iostream>

#if defined(_WIN32)
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API
#endif


#define  MAX_IPADDR_LEN				16       //ip address length    
#define  MAX_MACADDR_LEN			32		 //mac address length

typedef enum tagSDK_TYPE
{
	DH_SDK = 1,
	JXJ_SDK,
	DZP_SDK,
}NET_SDK_TYPE;

#pragma pack(1)

typedef struct
{
	int					nSDKType;							//device type 
	char				szIp[MAX_IPADDR_LEN];				//ip address
	int					nPort;								//port
	char				szSubmask[MAX_IPADDR_LEN];			//subnet mask
	char				szMac[MAX_MACADDR_LEN];				//mac address
	//int                 iIPVersion;							//ip version 4:ipv4 ; 6:ipv6
}NET_DEVICE_INFO;

#pragma pack()

class  SearchDeviceInterface
{

public:
	virtual bool GetDevice(char *szData, int nBufLen, int* pRetLen) = 0;
	virtual bool GetDeviceToJson(std::string path = NULL) = 0;
	virtual void Release() = 0;	
	virtual void SetDllPath(char *strDllpath) = 0;
private:
	
};

extern "C" LIBRARY_API SearchDeviceInterface* _stdcall CreateSearchObj();
extern "C" LIBRARY_API void _stdcall DestroySearchObj(SearchDeviceInterface* pExport);

#endif