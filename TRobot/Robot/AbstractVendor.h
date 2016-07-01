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

struct NET_DEVICE_INFO
{
	//NET_DEVICE_INFO& operator= (const NET_DEVICE_INFO& other)
	//{
	//	if (&other == this)
	//	{
	//		return *this;
	//	}
	//	nSDKType = other.nSDKType;

	//	nPort = other.nPort;

	//	setPrivateData(other.pPrivateData, other.PrivateDataDataSize);

	//	return *this;
	//}
	//void setSZ(void* pData, int size)
	//{
	//	if (nullptr != pPrivateData)
	//	{
	//		delete pPrivateData;
	//		pPrivateData = nullptr;
	//		PrivateDataDataSize = 0;
	//	}
	//	if (size > 0)
	//	{
	//		pPrivateData = new char[size];
	//		PrivateDataDataSize = size;
	//		memcpy(pPrivateData, pData, size);
	//	}
	//}
	int					nSDKType;							//device type 
	char				szIp[MAX_IPADDR_LEN];				//ip address
	int					nPort;								//port
	char				szSubmask[MAX_IPADDR_LEN];			//subnet mask
	char				szMac[MAX_MACADDR_LEN];				//mac address
	//int                 iIPVersion;							//ip version 4:ipv4 ; 6:ipv6
};

class AbstractVendor
{
public:

	virtual void Init() = 0;
	virtual long Login(const std::string& ip, size_t port, const std::string& user, const std::string& password) = 0;
	virtual void Logout(const long loginHandle) = 0;

	virtual void StartSearchDevice() = 0;
	virtual std::vector<NET_DEVICE_INFO*>& GetDeviceList() = 0;
	virtual void StopSearchDevice() = 0;

	virtual void SearchAll(const long loginHandle) = 0;
	virtual void Search(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void Download(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void PlayVideo(const long loginHandle, const size_t channel, const time_range& range) = 0;
	virtual void Download(const long loginHandle, const size_t channel, const std::string& filename) = 0;
	virtual void PlayVideo(const long loginHandle, const size_t channel, const std::string& filename) = 0;

	virtual void SetDownloadPath(const std::string& Root) = 0;
	virtual void throwException() = 0;

	void SetSDKType(NET_SDK_TYPE sdkType) { m_eSDKType = sdkType; }
	NET_SDK_TYPE GetSDKType(){ return m_eSDKType; }

private:
	NET_SDK_TYPE m_eSDKType;
};
