
#include "LoginDevice.h"


CLoginDevice::CLoginDevice()
{
}

CLoginDevice::~CLoginDevice()
{
}

void CLoginDevice::Login(const std::string& ip, const int port, const std::string& userName, const std::string& password, AbstractVendor* pVendor)
{
	Device* pDev = new Device();
	pDev->setSDK(pVendor);
	pDev->Login(ip, port, userName, password);

	CDeviceManager::getInstance().addDevice(pDev);
}

void CLoginDevice::Logout(AbstractVendor* pVendor)
{

}
