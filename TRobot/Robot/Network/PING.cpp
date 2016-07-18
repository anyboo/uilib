// #include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ping.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include "windowutils.h"
#include <memory>
#include <mutex>

OnePingInfo::OnePingInfo(){
	m_bStop = false;
	m_event = WSA_INVALID_EVENT;
	m_sockRaw = SOCKET_ERROR;
	int nICMPDataSize = DEF_PACKET_SIZE + sizeof(ICMPHeader);
	m_szICMPData = (char*)malloc(nICMPDataSize);


	memset(m_szICMPData, 0, nICMPDataSize);
}

OnePingInfo::~OnePingInfo(){
	m_bStop = true;

	if (m_event != WSA_INVALID_EVENT)
	{
		CloseHandle(m_event);
	}
	if (SOCKET_ERROR != m_sockRaw)
	{
		closesocket(m_sockRaw);
	}
	if (NULL != m_szICMPData)
	{
		free(m_szICMPData);
	}


}


ULONG CPing::s_usPacketSeq = 0;
CPing* CPing::spInstance = NULL;
bool CPingInit = false;
CPing::CPing()
{
	if (!CPingInit)
	{
		WSADATA WSAData;
		WSAStartup(MAKEWORD(1, 1), &WSAData);
		CPingInit = true;
	}

	m_usCurrentProcID = (USHORT)GetCurrentProcessId();


}
bool CPing::init(std::shared_ptr<OnePingInfo> pingInfo){
	if (!pingInfo)
	{
		return false;
	}

	pingInfo->m_event = WSACreateEvent();
	if (pingInfo->m_event == WSA_INVALID_EVENT)
	{
		return false;
	}


	pingInfo->m_sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, 0);
	if (SOCKET_ERROR == pingInfo->m_sockRaw)
	{
		int err = WSAGetLastError();
		wprintf(L"WSASocket function failed with error = %d\n", WSAGetLastError());
		return false;
	}

	WSAEventSelect(pingInfo->m_sockRaw, pingInfo->m_event, FD_READ);

	return true;
}
CPing::~CPing()
{
	WSACleanup();
}

bool GetLocalIp(vector<string> &vecIp)
{
	char szHostName[128];
	// char szT[20];

	if (gethostname(szHostName, 128) == 0)
	{
		// Get host adresses
		struct hostent * pHost;
		int i;
		pHost = gethostbyname(szHostName);

		for (i = 0; pHost != NULL && pHost->h_addr_list[i] != NULL; i++)
		{
			char* psz = inet_ntoa(*(struct in_addr *)pHost->h_addr_list[i]);
			vecIp.push_back(string(psz));
		}
	}
	return true;
}

void CPing::ScanIp(vector<string> &vecIp, bool bArp, std::shared_ptr<bool> bpCancel)
{
	vector<string> vecIpHost;
	GetLocalIp(vecIpHost);
	for (vector<string>::iterator itr = vecIpHost.begin(); itr != vecIpHost.end() && (!bpCancel || !(*bpCancel)); itr++)
	{
		ScanIPs(vecIp, itr->c_str(), bArp, bpCancel);
	}
}

void CPing::ScanIPs(vector<string> &vecIPs, const string& netIp, bool bArp, std::shared_ptr<bool> bpCancel){

	unsigned long ulIp = inet_addr(netIp.c_str());
	in_addr addr;
	addr.s_addr = ulIp;

	in_addr addrTmp = addr;
	char szBuf[MAX_PATH];

	for (unsigned char i = 2; i < 255 && (!bpCancel || !(*bpCancel)); ++i)
	{
		//unsigned char *pc = (unsigned char *)&i;
		//pc += 3;
		memcpy(&addrTmp.S_un.S_un_b.s_b4, &i, 1);
		string strIpTmp = inet_ntoa(addrTmp);
		if (addrTmp.S_un.S_un_b.s_b4 == addr.S_un.S_un_b.s_b4)
		{
			continue;
		}

		if (Ping((char *)strIpTmp.c_str(), 8, bArp))
		{
			vecIPs.push_back(strIpTmp.c_str());
		}
	}
}

bool CPing::ScanOneIP(string &ip, const string& netIp, bool bArp){
	string netPre = netIp.substr(0, netIp.find_last_of(".") + 1);
	for (int i = 1; i < 255; i++)
	{
		ip = netPre + std::to_string(i);
		cout << ip;
		if (netIp != ip && Ping((char *)ip.c_str(), 20, bArp))//Ping((char *)strIpTmp.c_str(), &reply, 2000)
		{
			return true;
		}
	}

	return false;
}

bool arpPing(DWORD dwDestIP){
	DWORD dwRetVal;
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG PhysAddrLen = 6;  /* default to length of six bytes */


	BYTE *bPhysAddr;


	memset(&MacAddr, 0xff, sizeof (MacAddr));

	dwRetVal = SendARP(dwDestIP, 0, &MacAddr, &PhysAddrLen);
	//  cout<<dwRetVal;

	switch (dwRetVal) {
	case ERROR_GEN_FAILURE:
		//        cout << " (ERROR_GEN_FAILURE)";
		break;
	case ERROR_INVALID_PARAMETER:
		//        cout << " (ERROR_INVALID_PARAMETER)";
		break;
	case ERROR_INVALID_USER_BUFFER:
		//        cout << " (ERROR_INVALID_USER_BUFFER)";
		break;
	case ERROR_BAD_NET_NAME:
		//        cout << " (ERROR_GEN_FAILURE)";
		break;
	case ERROR_BUFFER_OVERFLOW:
		//        cout << " (ERROR_BUFFER_OVERFLOW)";
		break;
	case ERROR_NOT_FOUND:
		//        cout << " (ERROR_NOT_FOUND)";
		break;
	default:
		break;
	}

	return (dwRetVal == NO_ERROR);
}

BOOL CPing::Ping(DWORD dwDestIP, long dwTimeout, bool bArp)
{
	std::shared_ptr<bool> b = std::make_shared<bool>(false);
	std::shared_ptr<bool> bStop = std::make_shared<bool>(false);
	{
		std::lock_guard<recursive_mutex> lock(mMtLastPingInfo);
		std::shared_ptr<OnePingInfo> pInfo = mpLastPingInfo;
		mpLastPingInfo = std::make_shared<OnePingInfo>();
		mpLastPingInfo->m_pReadThread = std::shared_ptr<std::thread>(new std::thread([=](){
			*b = PingCore(dwDestIP, dwTimeout, mpLastPingInfo, bArp);
			//*b = arpPing(dwDestIP);
			// cout << "PingCore" << *b;
			*bStop = true;
			if (pInfo && pInfo->m_pReadThread)
			{
				pInfo->m_pReadThread->join();
			}
		}));
	}

	while (!*bStop && dwTimeout--)
	{
		::Sleep(1);
	}

	return *b;
}

BOOL CPing::Ping(const char *szDestIP, long dwTimeout, bool bArp)
{
	if (NULL != szDestIP)
	{
		return Ping(inet_addr(szDestIP), dwTimeout, bArp);
	}
	return FALSE;
}

BOOL CPing::PingCore(DWORD dwDestIP, long dwTimeout, std::shared_ptr<OnePingInfo> pInfo, bool bArp)
{
	if (bArp)
	{
		return arpPing(dwDestIP);
	}
	//判断初始化是否成功
	if (!init(pInfo))
	{
		cout << __FILE__ << __FUNCTION__ << __LINE__;
		return FALSE;
	}

	//配置SOCKET
	sockaddr_in sockaddrDest;
	sockaddrDest.sin_family = AF_INET;
	sockaddrDest.sin_addr.s_addr = dwDestIP;
	int nSockaddrDestSize = sizeof(sockaddrDest);

	//构建ICMP包

	ICMPHeader *pICMPHeader = (ICMPHeader*)pInfo->m_szICMPData;
	pICMPHeader->m_byType = ECHO_REQUEST;
	pICMPHeader->m_byCode = 0;
	pICMPHeader->m_usID = m_usCurrentProcID;
	pICMPHeader->m_usSeq = ++s_usPacketSeq;
	pICMPHeader->m_ulTimeStamp = GetTickCountCalibrate();
	pICMPHeader->m_usChecksum = CalCheckSum((USHORT*)pInfo->m_szICMPData, DEF_PACKET_SIZE + sizeof(ICMPHeader));

	//发送ICMP报文
	if (sendto(pInfo->m_sockRaw, pInfo->m_szICMPData, DEF_PACKET_SIZE + sizeof(ICMPHeader), 0, (struct sockaddr*)&sockaddrDest, nSockaddrDestSize) == SOCKET_ERROR)
	{
		cout << __FILE__ << __FUNCTION__ << __LINE__ << "sendto";
		return FALSE;
	}

	char recvbuf[256] = { "\0" };

	for (ULONG ulTimeOutTick = GetTickCountCalibrate() + dwTimeout; dwTimeout > 0; dwTimeout = ulTimeOutTick - GetTickCountCalibrate())
	{
		if (WSAWaitForMultipleEvents(1, &pInfo->m_event, FALSE, dwTimeout, FALSE) == WSA_WAIT_TIMEOUT){
			return FALSE;
		}

		WSANETWORKEVENTS netEvent;
		WSAEnumNetworkEvents(pInfo->m_sockRaw, pInfo->m_event, &netEvent);
		if (!(netEvent.lNetworkEvents & FD_READ))
		{
			continue;
		}

		int nPacketSize = recvfrom(pInfo->m_sockRaw, recvbuf, 256, 0, (struct sockaddr*)&sockaddrDest, &nSockaddrDestSize);
		if (nPacketSize == SOCKET_ERROR){
			cout << __FILE__ << __FUNCTION__ << __LINE__ << "SOCKET_ERROR";
			continue;
		}

		IPHeader *pIPHeader = (IPHeader*)recvbuf;
		USHORT usIPHeaderLen = (USHORT)((pIPHeader->m_byVerHLen & 0x0f) * 4);
		ICMPHeader *pICMPHeader = (ICMPHeader*)(recvbuf + usIPHeaderLen);
		if (pICMPHeader->m_byType == ECHO_REPLY)
		{
			return TRUE;
		}
		else{

		}
	}

	return FALSE;
}

USHORT CPing::CalCheckSum(USHORT *pBuffer, int nSize)
{
	unsigned long ulCheckSum = 0;
	while (nSize > 1)
	{
		ulCheckSum += *pBuffer++;
		nSize -= sizeof(USHORT);
	}
	if (nSize)
	{
		ulCheckSum += *(UCHAR*)pBuffer;
	}

	ulCheckSum = (ulCheckSum >> 16) + (ulCheckSum & 0xffff);
	ulCheckSum += (ulCheckSum >> 16);

	return (USHORT)(~ulCheckSum);
}

ULONG CPing::GetTickCountCalibrate()
{
	static ULONG s_ulFirstCallTick = 0;
	static LONGLONG s_ullFirstCallTickMS = 0;

	SYSTEMTIME systemtime;
	FILETIME filetime;
	GetLocalTime(&systemtime);
	SystemTimeToFileTime(&systemtime, &filetime);
	LARGE_INTEGER liCurrentTime;
	liCurrentTime.HighPart = filetime.dwHighDateTime;
	liCurrentTime.LowPart = filetime.dwLowDateTime;
	LONGLONG llCurrentTimeMS = liCurrentTime.QuadPart / 10000;

	if (s_ulFirstCallTick == 0)
	{
		s_ulFirstCallTick = GetTickCount();
	}
	if (s_ullFirstCallTickMS == 0)
	{
		s_ullFirstCallTickMS = llCurrentTimeMS;
	}

	return s_ulFirstCallTick + (ULONG)(llCurrentTimeMS - s_ullFirstCallTickMS);
}