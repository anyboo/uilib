
#include "SearchDevice.h"
#include <Poco/SingletonHolder.h>
#include <cassert>


void AddListToList(DEVICE_INFO_LIST& listDest, const DEVICE_INFO_LIST& listSrc)
{
	for (auto v : listSrc)
	{
		listDest.push_back(v);
	}
}

CSearchDevice::CSearchDevice()
{
}

CSearchDevice::~CSearchDevice()
{
}

CSearchDevice& CSearchDevice::getInstance()
{
	static Poco::SingletonHolder<CSearchDevice> shSearchDevice;
	return *shSearchDevice.get();
}

void CSearchDevice::Init(VENDOR_LIST& pVendorList)
{
	DeleteDeviceList();

	if (pVendorList.size() <= 0)
	{
		throw std::exception("Vendor List is NULL!");
		return;
	}

	for (auto pVendor : pVendorList)
	{
		Device* pDev = new Device;
		pDev->setSDK(pVendor);
		m_listDevice.push_back(pDev);
	}
	for (size_t i = 0; i < m_listDevice.size() - 1; i++)
	{
		Device* pDev = m_listDevice[i];
		Device* pNextDev = m_listDevice[i + 1];
		pDev->SetNextDevice(pNextDev);
	}
	m_listDevice[m_listDevice.size() - 1]->SetNextDevice(NULL);
}

void CSearchDevice::Search(const DEVICE_INFO_SIMPLE_LIST& devInfoSimpleList)
{
	DeleteDeviceInfoList();

	// Known Situation: SDK Search Function Exist
	//for (auto pDev : m_listDevice)
	//{
	//	pDev->StartSearchDevice();
	//}
	//::Sleep(3000);
	//for (auto pDev : m_listDevice)
	//{
	//	pDev->StopSearchDevice();
	//}
	//for (auto pDev : m_listDevice)
	//{
	//	AddListToList(m_listDeviceInfo, pDev->GetDeviceInfoList());
	//}

	// Unknown Situation£ºSDK Search Function not Exist
	int i = 0;
	int indexVendor = 0;
	for (; i < devInfoSimpleList.size(); i++)
	{
		NET_DEVICE_INFO_SIMPLE* pDevInfoSimple = devInfoSimpleList[i];

		indexVendor = 0;
		if (m_listDevice[0]->LoginChain(pDevInfoSimple, indexVendor))
		{
			break;
		}
	}
	if (i < devInfoSimpleList.size())
	{
		Device* pDev = m_listDevice[indexVendor];
		NET_DEVICE_INFO* pDevInfo = new NET_DEVICE_INFO;
		memset(pDevInfo, 0, sizeof(NET_DEVICE_INFO));

		memcpy(pDevInfo->szIp, devInfoSimpleList[i]->szIP, MAX_IPADDR_LEN);
		pDevInfo->nPort = devInfoSimpleList[i]->nPort;
		pDevInfo->nSDKType = pDev->GetSDKType();
		pDevInfo->pVendor = pDev->GetSDK();

		m_listDeviceInfo.push_back(pDevInfo);

	}
	else
	{
		return;
	}
}

void CSearchDevice::DeleteDeviceList()
{
	for (auto pDev : m_listDevice)
	{
		delete pDev;
		pDev = nullptr;
	}

	m_listDevice.clear();
}
void CSearchDevice::DeleteDeviceInfoList()
{
	for (auto pDevInfo : m_listDeviceInfo)
	{
		delete pDevInfo;
		pDevInfo = nullptr;
	}

	m_listDeviceInfo.clear();
}