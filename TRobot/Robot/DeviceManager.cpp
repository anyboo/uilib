
#include "DeviceManager.h"
#include <Poco/SingletonHolder.h>
#include <cassert>


CDeviceManager::CDeviceManager()
{
}

CDeviceManager::~CDeviceManager()
{
}

CDeviceManager& CDeviceManager::getInstance()
{
	static Poco::SingletonHolder<CDeviceManager> sh;
	return *sh.get();
}

void CDeviceManager::addDevice(Device* pDev)
{
	m_vDeviceList.push_back(pDev);
}

Device& CDeviceManager::getDevice(const std::string ip)
{
	assert(&ip || ip.empty());
	
	for (std::vector<Device*>::iterator iter = m_vDeviceList.begin(); iter != m_vDeviceList.end();)
	{
		Device* pDev = *iter;
		if (ip == pDev->getIP())
		{
			return *pDev;
		}
	}
}

void CDeviceManager::deleteDevice(const std::string ip)
{
	assert(&ip || ip.empty());

	for (std::vector<Device*>::iterator iter = m_vDeviceList.begin(); iter != m_vDeviceList.end();)
	{
		Device *pDev = *iter;
		if (ip == pDev->getIP())
		{
			pDev->Logout();
			iter = m_vDeviceList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}
