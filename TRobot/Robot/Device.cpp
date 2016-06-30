
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

	if (!sdk)
	{
		std::cout << "sdk point is NULL" << std::endl;
		return;
	}

	m_pVendor = const_cast<AbstractVendor*>(sdk);
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
}

void Device::Init()
{
	assert(m_pVendor);
	m_pVendor->Init();
}

void Device::Login(const std::string& ip, size_t port, const std::string& user, const std::string& password)
{
	Init();

	assert(m_pVendor);
	m_lLoginHandle = m_pVendor->Login(ip, port, user, password);

	m_eLoginStatus = Login_Status_Yes;
	m_sIP = ip;
	m_iPort = port;
	m_sUserName = user;
	m_sPassword = password;
}

void Device::Logout()
{
	assert(m_pVendor);
	m_pVendor->Logout(m_lLoginHandle);
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