
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

//void CSearchDevice::Init(VENDOR_LIST& pVendorList)
//{
//	for (size_t i = 0; i < pVendorList.size()-1; i++)
//	{
//		AbstractVendor* pVendor = pVendorList[i];
//		AbstractVendor* pNextVendor = pVendorList[i + 1];
//		pVendor->SetNextVendor(pNextVendor);
//	}
//	pVendorList[pVendorList.size() - 1]->SetNextVendor(NULL);
//}

void CSearchDevice::Search(VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& devInfoSimpleList)
{
	DeleteDeviceInfoList();

	// Known Situation: SDK Search Function Exist
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
		AddListToList(m_listDeviceInfo, dev.GetDeviceInfoList());
	}

	// Unknown Situation£ºSDK Search Function not Exist
	//std::vector<Device*> listDevice;
	//for (size_t i = 0; i < pVendorList.size() - 1; i++)
	//{
	//	Device* pDev = new Device;
	//	Device* pNextDev = new Device;

	//	AbstractVendor* pVendor = pVendorList[i];
	//	AbstractVendor* pNextVendor = pVendorList[i + 1];
	//	pVendor->SetNextVendor(pNextVendor);
	//}
	//pVendorList[pVendorList.size() - 1]->SetNextVendor(NULL);

	//bool isLogin = false;
	//for (auto pVendor : pVendorList)
	//{
	//}
}

void CSearchDevice::LoginCheck(AbstractVendor* pVendor, const DEVICE_INFO_SIMPLE_LIST& devInfoSimpleList)
{
	Device* dev = new Device;
	dev->setSDK(pVendor);

	for (auto devInfoSim : devInfoSimpleList)
	{
		//if (dev->GetNextVendor()->Login(devInfoSim->szIP, devInfoSim->nPort))
		//{

		//}
		/*while (dev->GetNextVendor() != NULL)
		{

		}*/

		//if (dev.Login(devInfoSimple->szIP, devInfoSimple->nPort))
		//{
		//}
		//else
		//{
		//	dev.GetNextVendor.Login(devInfoSimple->szIP, devInfoSimple->nPort);
		//}
	}
	
}

void CSearchDevice::DeleteDeviceInfoList()
{
	for (auto devInfo : m_listDeviceInfo)
	{
		delete devInfo;
		devInfo = nullptr;
	}

	m_listDeviceInfo.clear();
}