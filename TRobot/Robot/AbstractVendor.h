#pragma once
#include <string>
#include <ctime>
#include <list>
#include <vector>

typedef struct _time_range
{
	std::time_t start;
	std::time_t end;
} time_range, *ptime_range;


#define  MAX_IPADDR_LEN				16       //ip address length    
#define  MAX_MACADDR_LEN			32		 //mac address length

typedef enum tagSDK_TYPE
{
	DH_SDK = 1,
	JXJ_SDK,
	DZP_SDK,
}NET_SDK_TYPE;

class AbstractVendor;

struct NET_DEVICE_INFO
{
	int					nSDKType;							//device type 
	char				szIp[MAX_IPADDR_LEN];				//ip address
	int					nPort;								//port
	char				szSubmask[MAX_IPADDR_LEN];			//subnet mask
	char				szMac[MAX_MACADDR_LEN];				//mac address
	//int                 iIPVersion;							//ip version 4:ipv4 ; 6:ipv6

	AbstractVendor*		pVendor;
};

typedef std::vector<NET_DEVICE_INFO*> DEVICE_INFO_LIST;

class AbstractVendor
{
public:
	virtual void Init() = 0;
	virtual long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password) = 0;
	virtual void Logout(const long loginHandle) = 0;

	virtual NET_SDK_TYPE GetSDKType() = 0;
	virtual void StartSearchDevice() = 0;
	virtual DEVICE_INFO_LIST& GetDeviceInfoList() = 0;
	virtual void StopSearchDevice() = 0;
	virtual size_t GetMaxChannel() = 0;

	virtual void SearchAll(const long loginHandle) = 0;
	virtual void Search(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void Download(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void PlayVideo(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void Download(const long loginHandle, const size_t channel, const std::string& filename) = 0;
	virtual void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename) = 0;

	virtual void SetDownloadPath(const std::string& Root) = 0;
	virtual void throwException() = 0;

};
