
#include "Device.h"
#include "LoginDevice.h"
#include "DeviceManager.h"

FastMutex CDeviceManager::m_mutex;

CDeviceManager::CDeviceManager(VENDOR_LIST& pVendorList, NotificationQueue& queue) :
	m_pVendorList(pVendorList),
	m_queue(queue)
{
}

CDeviceManager::~CDeviceManager()
{
}

AbstractVendor* getVendotFromVendorList(const VENDOR_LIST& pVendorList, NET_SDK_TYPE type)
{
	AbstractVendor* pVendor = nullptr;

	for (size_t i = 0; i < pVendorList.size(); i++)
	{
		pVendor = pVendorList[i];
		if (pVendor->GetSDKType() == type)
		{
			break;
		}
	}

	return pVendor;
}

void CDeviceManager::run()
{
	for (;;)
	{
		// Cancel Device Manager Thread
		Notification::Ptr pNf(m_queue.waitDequeueNotification(1000));
		if (pNf)
		{
			CNotificationDeviceManager::Ptr pDeviceManagerNf = pNf.cast<CNotificationDeviceManager>();
			if (pDeviceManagerNf)
			{
				if (pDeviceManagerNf->GetNotificationType() == Notification_Type_Device_Manager_Cancel)
				{
					FastMutex::ScopedLock lock(m_mutex);
					std::cout << "Device Manager Process Cancel Success!" << std::endl;
					break;
				}
			}
		}

		FastMutex::ScopedLock lock(m_mutex);
		NotificationQueue& queue = NotificationQueue::defaultQueue();
		std::vector<LoginDeviceInfo> listLoginDevice = CLoginDevice::getInstance().GetLoginDeviceFromDB();
		std::vector<Device*>& listDevice = CLoginDevice::getInstance().GetDeviceList();

		for (size_t i = 0; i < listLoginDevice.size(); i++)
		{
			LoginDeviceInfo ld = listLoginDevice[i];

			NET_SDK_TYPE type = NET_SDK_TYPE(ld.get<0>());
			std::string ip = ld.get<1>();
			int port = ld.get<2>();
			std::string username = ld.get<3>();
			std::string password = ld.get<4>();
			int maxchannel = ld.get<5>();

			size_t j = 0;
			for (; j < listDevice.size(); j++)
			{
				Device* pDev = listDevice[j];
				if (pDev->getIP() == ip)
				{
					if (pDev->getLoginStatus() == Login_Status_No)
					{
						queue.enqueueNotification(new CNotificationDeviceManager(Notification_Type_Device_Manager_Dropped, ip));
						if (pDev->Login(ip, port, username, password))
						{
							queue.enqueueNotification(new CNotificationDeviceManager(Notification_Type_Device_Manager_Online, ip));
						}
					}
					break;
				}
			}
			if (j >= listDevice.size())
			{
				Device* pDev = new Device;
				AbstractVendor* pVendor = getVendotFromVendorList(m_pVendorList, type);
				if (pVendor != nullptr)
				{
					pDev->setSDK(pVendor);
					if (pDev->Login(ip,port,username,password))
					{
						listDevice.push_back(pDev);						
						queue.enqueueNotification(new CNotificationDeviceManager(Notification_Type_Device_Manager_Online, ip));
					}
				}
			}
		}
	}
}
