
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
		CDeviceManager::getInstance().addDevice(pDev);
		return true;
	}
	else
	{
		delete pDev;
		pDev = nullptr;
		return false;
	}
}

void CLoginDevice::Logout(AbstractVendor* pVendor)
{

}
