
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
	static Poco::SingletonHolder<CDeviceManager> shDeviceManager;
	return *shDeviceManager.get();
}

void CDeviceManager::addDevice(Device* pDev)
{
	std::vector<Device*>::iterator result = find(m_listDevice.begin(), m_listDevice.end(), pDev); //≤È’“
	if (result == m_listDevice.end())
	{
		m_listDevice.push_back(pDev);
	}
}

Device& CDeviceManager::getDevice(const std::string ip)
{
	assert(&ip || ip.empty());
	
	for (std::vector<Device*>::iterator iter = m_listDevice.begin(); iter != m_listDevice.end();)
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

	for (std::vector<Device*>::iterator iter = m_listDevice.begin(); iter != m_listDevice.end();)
	{
		Device *pDev = *iter;
		if (ip == pDev->getIP())
		{
			pDev->Logout();
			iter = m_listDevice.erase(iter);
			delete pDev;
		}
		else
		{
			iter++;
		}
	}
}
