
#include "LoginDevice.h"
#include <Poco/SingletonHolder.h>
#include <cassert>


CLoginDevice::CLoginDevice()
{
}

CLoginDevice::~CLoginDevice()
{
}

CLoginDevice& CLoginDevice::getInstance()
{
	static Poco::SingletonHolder<CLoginDevice> shLoginDevice;
	return *shLoginDevice.get();
}

bool CLoginDevice::Login(AbstractVendor* pVendor, const std::string& ip, const int port, const std::string& userName, const std::string& password)
{
	Device* pDev = new Device();
	pDev->setSDK(pVendor);

	std::string sUserName = userName;
	std::string sPassword = password;

	if (sUserName.empty() || sUserName.compare("") == 0)
	{
		sUserName = pVendor->GetDefUsearName();
		sPassword = pVendor->GetDefPassword();
	}

	if (pDev->Login(ip, port, sUserName, sPassword))
	{
		std::vector<Device*>::iterator result = find(m_listDevice.begin(), m_listDevice.end(), pDev); //≤È’“
		if (result == m_listDevice.end())
		{
			m_listDevice.push_back(pDev);
		}
		return true;
	}
	else
	{
		delete pDev;
		pDev = nullptr;
		return false;
	}
}

void CLoginDevice::Logout(const std::string& ip)
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

Device* CLoginDevice::GetDevice(const std::string ip)
{
	assert(&ip || ip.empty());

	for (std::vector<Device*>::iterator iter = m_listDevice.begin(); iter != m_listDevice.end();)
	{
		Device* pDev = *iter;
		if (ip == pDev->getIP())
		{
			return pDev;
		}
	}

	return NULL;
}