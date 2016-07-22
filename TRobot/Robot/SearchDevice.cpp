
#include "WindowUtils.h"
#include "SearchDevice.h"


DEVICE_INFO_LIST CSearchDevice::m_listDeviceInfo;
FastMutex CSearchDevice::m_mutex;

bool CheckDeviceInfo(const NET_DEVICE_INFO* devInfo, const DEVICE_INFO_LIST& list)
{
	for (auto v : list)
	{
		if (strcmp(devInfo->szIp, v->szIp) == 0)
		{
			return true;
		}
	}

	return false;
}
void AddListToList(DEVICE_INFO_LIST& listDest, const DEVICE_INFO_LIST& listSrc)
{
	for (auto v : listSrc)
	{
		if (!CheckDeviceInfo(v, listDest))
		{
			listDest.push_back(v);
		}
	}
}
DEVICE_INFO_SIMPLE_LIST MakeDeviceInfoSimpleList(const DEVICE_INFO_LIST& listDeviceInfo, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfo)
{
	DEVICE_INFO_SIMPLE_LIST list;

	for (auto devSimpleInfo : listDeviceSimpleInfo)
	{
		size_t i = 0;
		for (; i < listDeviceInfo.size(); i++)
		{
			NET_DEVICE_INFO* pDevInfo = listDeviceInfo[i];
			if (strcmp(pDevInfo->szIp, devSimpleInfo->szIP) == 0)
			{
				break;
			}
		}
		if (i >= listDeviceInfo.size())
		{
			list.push_back(devSimpleInfo);
		}
	}

	return list;
}

CSearchDevice::CSearchDevice(const VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfo, NotificationQueue& queue) :
	m_pVendorList(pVendorList),
	m_listDeviceSimpleInfo(listDeviceSimpleInfo),
	m_queue(queue)
{
}
CSearchDevice::~CSearchDevice()
{
}
void CSearchDevice::InitDeviceList(const VENDOR_LIST& pVendorList)
{
	if (pVendorList.size() <= 0)
	{
		throw std::exception("Vendor List is NULL!");
		return;
	}

	for (auto pVendor : pVendorList)
	{
		Device* pDev = new Device;
		pDev->setSDK(pVendor);
		if (pDev->IsSearchDeviceAPIExist() == true)
		{
			m_listDeviceKnown.push_back(pDev);
		}
		else
		{
			m_listDeviceUnknown.push_back(pDev);
		}
	}
	if (m_listDeviceUnknown.size() > 0)
	{
		for (size_t i = 0; i < m_listDeviceUnknown.size() - 1; i++)
		{
			Device* pDev = m_listDeviceUnknown[i];
			Device* pNextDev = m_listDeviceUnknown[i + 1];
			pDev->SetNextDevice(pNextDev);
		}
		m_listDeviceUnknown[m_listDeviceUnknown.size() - 1]->SetNextDevice(NULL);
	}
}

void CSearchDevice::Search(const VENDOR_LIST& pVendorList, const DEVICE_INFO_SIMPLE_LIST& listDeviceSimpleInfo)
{
	DeleteDeviceInfoList();
	DeleteDeviceList();
	InitDeviceList(pVendorList);

	// Known Situation: SDK Search Function Exist
	for (auto pDev : m_listDeviceKnown)
	{
		pDev->StartSearchDevice();
	}
	::Sleep(3000);
	for (auto pDev : m_listDeviceKnown)
	{
		pDev->StopSearchDevice();
	}
	for (auto pDev : m_listDeviceKnown)
	{
		AddListToList(m_listDeviceInfo, pDev->GetDeviceInfoList());
	}
	// Screen Device Simple Info List
	DEVICE_INFO_SIMPLE_LIST list = MakeDeviceInfoSimpleList(m_listDeviceInfo, listDeviceSimpleInfo);

	// Unknown Situation��SDK Search Function not Exist
	if (m_listDeviceUnknown.size() > 0)
	{
		int indexVendor = 0;
		for (size_t i = 0; i < list.size(); i++)
		{
			NET_DEVICE_INFO_SIMPLE* pDevInfoSimple = list[i];

			indexVendor = 0;
			if (m_listDeviceUnknown[0]->LoginChain(pDevInfoSimple, indexVendor))
			{
				Device* pDev = m_listDeviceUnknown[indexVendor];
				NET_DEVICE_INFO* pDevInfo = new NET_DEVICE_INFO;
				memset(pDevInfo, 0, sizeof(NET_DEVICE_INFO));

				memcpy(pDevInfo->szIp, list[i]->szIP, MAX_IPADDR_LEN);
				pDevInfo->nPort = list[i]->nPort;
				pDevInfo->nSDKType = pDev->GetSDKType();
				pDevInfo->pVendor = pDev->GetSDK();

				m_listDeviceInfo.push_back(pDevInfo);
			}
		}
	}
}

void CSearchDevice::DeleteDeviceList()
{
	int a = 0;

	for (auto pDev : m_listDeviceKnown)
	{
		delete pDev;
		pDev = nullptr;
	}
	for (auto pDev : m_listDeviceUnknown)
	{
		delete pDev;
		pDev = nullptr;
	}

	m_listDeviceKnown.clear();
	m_listDeviceUnknown.clear();
}
void CSearchDevice::DeleteDeviceInfoList()
{
	int a = 0;

	for (auto pDevInfo : m_listDeviceInfo)
	{
		delete pDevInfo;
		pDevInfo = nullptr;
	}

	m_listDeviceInfo.clear();
}

void CSearchDevice::run()
{
	static bool bNetStatusLast = false;
	for (;;)
	{
		// Cancel Search Device Thread
		Notification::Ptr pNf(m_queue.waitDequeueNotification(1000));
		if (pNf)
		{
			CNotificationSearchDevice::Ptr pSearchDeviceNf = pNf.cast<CNotificationSearchDevice>();
			if (pSearchDeviceNf)
			{
				if (pSearchDeviceNf->GetNotificationType() == Notification_Type_Search_Device_Cancel)
				{
					FastMutex::ScopedLock lock(m_mutex);
					std::cout << "Search Device Process Cancel Success!" << std::endl;
					break;
				}
			}
		}

		bool bNetStatus = WindowUtils::isOnLine();
		if (bNetStatusLast != bNetStatus)
		{
			NotificationQueue& queue = NotificationQueue::defaultQueue();
			NOTIFICATION_TYPE NfType = (bNetStatus == true ? Notification_Type_Network_status_Connect : Notification_Type_Network_status_Disconnect);
			queue.enqueueNotification(new CNotificationNetworkStatus(NfType));
			bNetStatusLast = bNetStatus;
			Search(m_pVendorList, m_listDeviceSimpleInfo);
			queue.enqueueNotification(new CNotificationSearchDevice(Notification_Type_Search_Device_Finish));
		}
	}
}