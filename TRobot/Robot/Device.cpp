
#include "Device.h"
#include <cassert>
#include <iostream>

Device::Device(const AbstractVendor* sdk)
{
	m_eLoginStatus = Login_Status_No;
	m_sIP = "";
	m_iPort = 0;
	m_sUserName = "";
	m_sPassword = "";
	m_iMaxChannel = 0;
	m_vChannelList.clear();

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
	m_vChannelList.clear();
}

Device::~Device()
{
	assert(m_pVendor);
	m_pVendor->Logout(m_lLoginHandle);

	m_eLoginStatus = Login_Status_No;
	m_sIP = "";
	m_iPort = 0;
	m_sUserName = "";
	m_sPassword = "";
	m_iMaxChannel = 0;
	m_vChannelList.clear();
}

void Device::Init()
{
	assert(m_pVendor);
	m_pVendor->Init();	
}

void Device::Login(const std::string& ip, size_t port, const std::string& userName, const std::string& password)
{
	assert(m_pVendor);
	m_lLoginHandle = m_pVendor->Login(ip, port, userName, password);

	m_eLoginStatus = Login_Status_Yes;
	m_sIP = ip;
	m_iPort = port;
	m_sUserName = userName;
	m_sPassword = password;
}

void Device::Logout()
{
	assert(m_pVendor);
	m_pVendor->Logout(m_lLoginHandle);
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

void Device::Download(const size_t channel, const time_range& range)
{
	assert(m_pVendor);
	m_pVendor->Download(m_lLoginHandle, channel, range);
}

void Device::PlayVideo(const size_t channel, const time_range& range)
{
	assert(m_pVendor);
	m_pVendor->PlayVideo(m_lLoginHandle, channel, range);
}

void Device::Download(const size_t channel, const std::string& fileName)
{
	assert(m_pVendor);
	m_pVendor->Download(m_lLoginHandle, channel, fileName);
}

void Device::PlayVideo(const size_t channel, const std::string& fileName)
{
	assert(m_pVendor);
	m_pVendor->PlayVideo(m_lLoginHandle, channel, fileName);
}

void Device::SetDownloadPath(const std::string& root)
{
	assert(m_pVendor);
	m_pVendor->SetDownloadPath(root);
}

void Device::setChannel(const size_t maxChannel, const std::vector<size_t>& channelList)
{
	m_iMaxChannel = maxChannel;
	m_vChannelList = channelList;
}