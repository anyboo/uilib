
#include "SearchDevice.h"
#include <WinSock.h>


#pragma comment(lib, "Ws2_32.lib")

long CSearchDevice::m_nRef = 0;

CSearchDevice::CSearchDevice()
{
}


CSearchDevice::~CSearchDevice()
{
}

void AddListToList(std::vector<NET_DEVICE_INFO*>& listDest, const std::vector<NET_DEVICE_INFO*>& listSrc)
{
	for (auto v : listSrc)
	{
		listDest.push_back(v);
	}
}

void CSearchDevice::Init(std::vector<AbstractVendor*> pVendorList)
{
	//m_listDeviceInfo.clear();

	std::list<Device> listDevice;

	for (auto pVendor : pVendorList)
	{
		Device dev(pVendor);
		listDevice.push_back(dev);
		dev.StartSearchDevice();
	}

	::Sleep(3000);

	for (auto dev : listDevice)
	{
		dev.StopSearchDevice();
	}
	for (auto dev : listDevice)
	{
		AddListToList(m_listDeviceInfo, dev.GetDeviceList());
	}
}