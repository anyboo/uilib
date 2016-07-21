
#include "Device.h"

#pragma comment(lib, "Ws2_32.lib")

Device::Device(const AbstractVendor* sdk)
{
	m_eLoginStatus = Login_Status_No;
	m_sIP = "";
	m_iPort = 0;
	m_sUserName = "";
	m_sPassword = "";
	m_iMaxChannel = 0;

	if (!sdk)
	{
		std::cout << "sdk point is NULL" << std::endl;
		return;
	}
	m_pVendor = const_cast<AbstractVendor*>(sdk);

	assert(m_pVendor);
	Init();
}

Device::Device()
{
	m_eLoginStatus = Login_Status_No;
	m_sIP = "";
	m_iPort = 0;
	m_sUserName = "";
	m_sPassword = "";
	m_iMaxChannel = 0;
}

Device::~Device()
{
	assert(m_pVendor);
	//m_pVendor->Logout(m_lLoginHandle);

	m_eLoginStatus = Login_Status_No;
	m_sIP = "";
	m_iPort = 0;
	m_sUserName = "";
	m_sPassword = "";
	m_iMaxChannel = 0;
}

void Device::setSDK(AbstractVendor* sdk)
{
	if (!sdk)
	{
		std::cout << "sdk point is NULL" << std::endl;
		return;
	}
	m_pVendor = const_cast<AbstractVendor*>(sdk);

	assert(m_pVendor);
	Init();
}

void Device::Init()
{
	assert(m_pVendor);
	m_pVendor->Init();

	std::string sRoot = "D:\\DOWNLOAD_SRC";
	m_pVendor->SetDownloadPath(sRoot);
}

bool Device::LoginChain(const NET_DEVICE_INFO_SIMPLE* pDevInfoSimple, int& indexVendor)
{
	if (Login(pDevInfoSimple->szIP, pDevInfoSimple->nPort))
	{
		//Logout();
		return true;
	}
	else
	{
		if (GetNextDevice() != NULL)
		{
			indexVendor++;
			GetNextDevice()->LoginChain(pDevInfoSimple, indexVendor);
		}
	}
}

bool Device::Login(const std::string& ip, size_t port, const std::string& userName, const std::string& password)
{
	assert(m_pVendor);

	std::string sUserName = userName;
	std::string sPassword = password;

	if (sUserName.empty() || sUserName.compare("") == 0)
	{
		sUserName = m_pVendor->GetDefUsearName();
		sPassword = m_pVendor->GetDefPassword();
	}

	m_lLoginHandle = m_pVendor->Login(ip, port, sUserName, sPassword);
	if (m_lLoginHandle < 0)
	{
		return false;
	}

	m_iMaxChannel = m_pVendor->GetMaxChannel();
	m_eLoginStatus = Login_Status_Yes;
	m_sIP = ip;
	m_iPort = port;
	m_sUserName = sUserName;
	m_sPassword = sPassword;

	return true;
}

void Device::Logout()
{
	assert(m_pVendor);
	m_pVendor->Logout(m_lLoginHandle);

	m_eLoginStatus = Login_Status_No;
}

void Device::StartSearchDevice()
{
	assert(m_pVendor);
	m_pVendor->StartSearchDevice();
}

void Device::StopSearchDevice()
{
	assert(m_pVendor);
	m_pVendor->StopSearchDevice();
}
void Device::SearchAll()
{
	assert(m_pVendor);
	m_pVendor->SearchAll(m_lLoginHandle);
}

void Device::Search(const size_t channel, const time_range& range)
{
	assert(m_pVendor);
	m_pVendor->Search(m_lLoginHandle, channel, range);
}

void Device::Download(const size_t channel, const RecordFile& recordFile)
{
	assert(m_pVendor);
	m_pVendor->Download(m_lLoginHandle, channel, recordFile);
}

void Device::PlayVideo(const HWND hWnd, const size_t channel, const RecordFile& recordFile)
{
	assert(m_pVendor);
	m_pVendor->SetHWnd(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	m_pVendor->PlayVideo(m_lLoginHandle, channel, recordFile);
}

void Device::SetDownloadPath(const std::string& root)
{
	assert(m_pVendor);
	m_pVendor->SetDownloadPath(root);
}
