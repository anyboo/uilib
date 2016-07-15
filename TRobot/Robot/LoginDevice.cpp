
#include <Poco/SingletonHolder.h>
#include <cassert>

#include "LoginDevice.h"
#include "Device.h"
#include "CommonUtrl.h"

static int loginCount = 0;

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
		std::vector<Device*>::iterator result = find(m_listDevice.begin(), m_listDevice.end(), pDev); //查找
		if (result == m_listDevice.end())
		{
			m_listDevice.push_back(pDev);

			LoginDeviceInfo ld;
			//ype
			ld.set<0>(pDev->GetSDKType());
			//ip
			ld.set<1>(pDev->getIP());
			//port
			ld.set<2>(pDev->getPort());
			//username
			ld.set<3>(pDev->getUserName());
			//password
			ld.set<4>(pDev->getPassword());
			//maxchannel
			ld.set<5>(pDev->getMaxChannel());

			//获取指针
			QFileSqlite *pDb = QFileSqlite::getInstance();
			std::string sql(INSERT_LOGIN_DEVICE_INFO);
			pDb->writeData(sql, ld);

			std::cout << CCommonUtrl::getInstance().GetCurTime() << pDev->getIP() << " + " << ++loginCount << std::endl;
		}
		else
		{
			Device* pDev = *result;
			pDev->setLoginStatus(Login_Status_Yes);
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
			//获取指针
			QFileSqlite *pDb = QFileSqlite::getInstance();
			std::string sql(DELETE_IP_LOGIN_DEVICE);
			sql += "'";
			sql.append(ip);
			sql += "'";
			pDb->dropTable(sql);

			std::cout << CCommonUtrl::getInstance().GetCurTime() << pDev->getIP()  << " - " << --loginCount << std::endl;

			pDev->Logout();
			iter = m_listDevice.erase(iter);
			delete pDev;
			pDev = nullptr;
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
		else
		{
			iter++;
		}
	}

	return NULL;
}

std::vector<LoginDeviceInfo> CLoginDevice::GetLoginDeviceFromDB()
{
	std::vector<LoginDeviceInfo> listLoginDevice;

	//获取指针
	QFileSqlite *pDb = QFileSqlite::getInstance();
	std::string sql = SELECT_ALL_LOGIN_DEVICE_INFO;
	pDb->GetData(sql, listLoginDevice);

	return listLoginDevice;
}