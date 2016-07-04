
#include "SearchDevice.h"

CSearchDevice::CSearchDevice()
{
}

CSearchDevice::~CSearchDevice()
{
}

void AddListToList(DEVICE_INFO_LIST& listDest, const DEVICE_INFO_LIST& listSrc)
{
	for (auto v : listSrc)
	{
		listDest.push_back(v);
	}
}

void CSearchDevice::Search(std::vector<AbstractVendor*> pVendorList)
{
	DeleteDeviceInfoList();

	std::list<Device> listDevice;

	for (auto pVendor : pVendorList)
	{
		Device dev(pVendor);
		listDevice.push_back(dev);
		dev.StartSearchDevice();
	}
	for (auto dev : listDevice)
	{
		dev.StopSearchDevice();
	}
	for (auto dev : listDevice)
	{
		AddListToList(m_listDeviceInfo, dev.GetDeviceInfoList());
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