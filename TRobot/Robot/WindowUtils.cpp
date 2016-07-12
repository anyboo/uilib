
#include "WindowUtils.h"

#include "windowutils.h"
#include "pcap.h"
#include <windows.h>
#include <Winsock2.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include "PING.h"
#include <IPTypes.h>
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <tchar.h>
#include <Shellapi.h>
#include <NetCon.h>

#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib,"wpcap.lib")


inline bool caseInsCharCompSingle(char a, char b)
{
	return toupper(a) == b;
}

string::const_iterator caseInsFind(string& s, const string& p)
{
	string tmp;
	transform(p.begin(), p.end(), back_inserter(tmp), (int(*)(int))toupper);
	return (search(s.begin(), s.end(), tmp.begin(), tmp.end(), caseInsCharCompSingle));
}

void GetLocalNetCar(std::string& Desc, std::string& AdapterName, std::vector<string>& IPList)
{
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	int netCardNum = 0;
	int IPnumPerNetCard = 0;
	string::const_iterator it;
	std::string sNetName;
	PIP_ADAPTER_INFO pstartIpAdapterInfo;

	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		pstartIpAdapterInfo = pIpAdapterInfo;
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
		if (ERROR_SUCCESS == nRel)
		{
			while (pIpAdapterInfo)
			{
				switch (pIpAdapterInfo->Type)
				{
				case MIB_IF_TYPE_OTHER:
					break;
				case MIB_IF_TYPE_ETHERNET:
					sNetName = std::string(pIpAdapterInfo->Description);
					it = caseInsFind(sNetName, string("pci"));
					if (it != sNetName.end())
					{
						Desc = sNetName;
						AdapterName = pIpAdapterInfo->AdapterName;
						IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
						do
						{
							IPList.push_back(std::string(pIpAddrString->IpAddress.String));
							cout << "IP address:" << pIpAddrString->IpAddress.String << endl;
							pIpAddrString = pIpAddrString->Next;
						} while (pIpAddrString);
					}

					break;
				case MIB_IF_TYPE_TOKENRING:
					break;
				case MIB_IF_TYPE_FDDI:
					break;
				case MIB_IF_TYPE_PPP:
					break;
				case MIB_IF_TYPE_LOOPBACK:
					break;
				case MIB_IF_TYPE_SLIP:
					break;
				default:

					break;
				}

				pIpAdapterInfo = pIpAdapterInfo->Next;
			}
		}
		if (pstartIpAdapterInfo)
		{
			delete[]pstartIpAdapterInfo;
		}
	}
}


string GetNetCardName(std::string uuid)
{
	string CardName;
	INetConnectionManager *pManager;
	INetConnection *pConnection;
	IEnumNetConnection *pEnum;
	ULONG           celtFetched;

	CoInitialize(NULL);
	CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pManager);
	pManager->EnumConnections(NCME_DEFAULT, &pEnum);
	pManager->Release();
	while (pEnum->Next(1, &pConnection, &celtFetched) == S_OK)
	{
		NETCON_PROPERTIES*   properties;
		pConnection->GetProperties(&properties);
		char szGuid[100] = { 0 };
		sprintf_s(szGuid, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
			properties->guidId.Data1, properties->guidId.Data2, properties->guidId.Data3,
			properties->guidId.Data4[0], properties->guidId.Data4[1], properties->guidId.Data4[2], properties->guidId.Data4[3],
			properties->guidId.Data4[4], properties->guidId.Data4[5], properties->guidId.Data4[6], properties->guidId.Data4[7]);
		string sGuid(szGuid);
		if (sGuid.compare(uuid) == 0)
		{
			DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, properties->pszwName, -1, NULL, 0, NULL, FALSE);
			char *psText;
			psText = new char[dwNum];
			if (!psText)
			{
				delete[]psText;
			}
			WideCharToMultiByte(CP_OEMCP, NULL, properties->pszwName, -1, psText, dwNum, NULL, FALSE);
			CardName = string(psText);
			delete[]psText;
		}
	}
	CoUninitialize();

	return CardName;
}

string ConvertNICUUIDtoPcapName(pcap_if_t* devs, const string& uuid)
{
	string pcap_name;

	for (pcap_if_t *d = devs; d && d->next; d = d->next)
	{
		string tmp(d->name);
		//if (tmp.contains(uuid, Qt::CaseInsensitive))
		pcap_name = tmp;
	}

	return pcap_name;
}

CWindowUtils::CWindowUtils()
{
}


bool CWindowUtils::isValidNetMacaddress(const string& macaddress)
{
	return !macaddress.empty() && (macaddress.find("00:00:00:00:00:00:00:E0") == string::npos)
		&& (macaddress.find("00:50:56:C0") == string::npos);
}

void CWindowUtils::GetIPfromLocalNIC(std::vector<string> &IPs)
{
	cout << "GetIPfromLocalNIC start";
	getLocalIPs(IPs);
	cout << "GetIPfromLocalNIC end";
}

void CWindowUtils::getLocalIPs(std::vector<string> &IPs)
{
	IPs.clear();
	string sDesc;
	string uuid;
	GetLocalNetCar(sDesc, uuid, IPs);
}

void CWindowUtils::getLocalIPs(const string& sHostName, std::vector<string> &IPs){
	IPs.clear();
	/*QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
	foreach(QNetworkInterface i, list) {
	if (i.isValid() && isValidNetMacaddress(i.hardwareAddress()) &&
	i.humanReadableName() == WindowUtils::getLoacalNetName())
	{
	cout << i.name() << i.humanReadableName();

	foreach(QHostAddress address, i.allAddresses())
	{
	if (address.protocol() == QAbstractSocket::IPv4Protocol
	&& !address.isLoopback() && Utils::isInnerIP(address.toString()))
	{
	if (IPs.end() == std::find(IPs.begin(), IPs.end(), address.toString()))
	{
	IPs.push_back(address.toString());
	cout <<__FILE__ << __FUNCTION__ << __LINE__ << address.toString();
	}
	}
	}
	}

	}*/
}


bool CWindowUtils::setNetConfig(const string& sName, const string& sIP, const string& sMask, const string& sGate, bool bWait){
	//    return true;
	/*string mask = string("mask=%1").arg(sMask);
	string name = string("name=\"%1\"").arg(sName);
	string addr = string("addr=%1").arg(sIP);
	stringList argList = stringList() << "interface" << "ip" << "set" << "address" << name << "source=static" << addr << mask;

	if (!sGate.empty())
	{
	string gateway = string("gateway=%1").arg(sGate);
	QProcess::execute("netsh", stringList() << "interface" << "ip" << "set" << "address" << name << "source=static" << addr << mask << gateway);
	}
	else{
	QProcess::execute("netsh", stringList() << "interface" << "ip" << "set" << "address" << name << "source=static" << addr << mask );
	}
	if (!bWait)
	{
	return true;
	}*/
	int maxPingTime = 1000 * 3;
	::Sleep(2000);
	while (maxPingTime > 0 && !CPing::instance().Ping(sIP.c_str(), 20)){
		::Sleep(1000);
		maxPingTime -= 1000;
	}

	return maxPingTime > 0;
}

bool CWindowUtils::setNetDhcp(const string& sName){
	string sCmd("interface ip set address name=\"");
	sCmd += sName;
	sCmd.append("\" source=dhcp");
	::ShellExecuteA(NULL, (LPCSTR)"open", (LPCSTR)"netsh.exe", (LPCSTR)sCmd.c_str(), NULL, SW_SHOWNORMAL);
	return true;
}

bool CWindowUtils::isConnecteTo(const string& IP, int millSeconds){
	return CPing::instance().Ping(IP.c_str(), millSeconds);
}


typedef struct arppkt
{
	unsigned short hdtyp;//硬件类型
	unsigned short protyp;//协议类型
	unsigned char hdsize;//硬件地址长度
	unsigned char prosize;//协议地址长度
	unsigned short op;//（操作类型）操作值:ARP/RARP
	u_char smac[6];//源MAC地址
	u_char sip[4];//源IP地址
	u_char dmac[6];//目的MAC地址
	u_char dip[4];//目的IP地址
}arpp;


#define  INNDER_SPECIAL_IP "170.151.24.203"
bool CWindowUtils::getDirectDevice(string& ip, string& netGate)
{
	ip.clear();
	struct tm * timeinfo;
	struct tm *ltime;
	time_t rawtime;

	int result;
	int i = 0, inum;
	pcap_if_t * alldevs;//指向pcap_if_t结构列表指针
	pcap_t * adhandle;//定义包捕捉句柄
	char errbuf[PCAP_ERRBUF_SIZE];//错误缓冲最小为256
	u_int netmask; //定义子网掩码
	char packet_filter[] = "ether proto \\arp";
	struct bpf_program fcode;
	struct pcap_pkthdr * header;
	const u_char * pkt_data;
	//打开日志文件

	//当前所有可用的网络设备
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		cout << "Error in pcap_findalldevs:" << errbuf;
		return false;
	}

	if (!alldevs)
	{
		cout << "cannot find net device!  install WinPcap?";
		return false;
	}

	cout << alldevs->description << alldevs->name;

	if ((adhandle = pcap_open_live(alldevs->name, 65536, 1, 1000, errbuf)) == NULL)
	{
		cout << "pcap_open_live failed!  not surpport by WinPcap ?" << alldevs->name;
		pcap_freealldevs(alldevs);
		return false;
	}

	if (pcap_datalink(adhandle) != DLT_EN10MB || alldevs->addresses == NULL) {
		cout << "pcap_datalink(adhandle) != DLT_EN10MB || alldevs->addresses == NULL";
		return false;
	}


	netmask = ((struct sockaddr_in *)(alldevs->addresses->netmask))->sin_addr.S_un.S_addr;
	pcap_freealldevs(alldevs);


	//编译过滤器，只捕获ARP包
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{
		cout << "unable to compile the packet filter.Check the syntax.";
		return false;
	}

	//设置过滤器
	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		cout << "Error setting the filter.";
		return false;
	}
	std::vector<string> IPs;
	getLocalIPs(IPs);
	const int nMaxSeconds = 30;
	int start = GetTickCount();
	string specialIP;
	while (true)
	{
		if (GetTickCount() - start > 30 * 1000)
		{
			cout << "arp time out";
			break;
		}
		//循环解析ARP数据包
		if (pcap_next_ex(adhandle, &header, &pkt_data) == 0){
			continue;
		}
		arpp* arph = (arpp *)(pkt_data + 14);
		if (arph->op == 256) //arp
		{
			if (arph->sip[0] == 0)
			{
				continue;
			}
			if (arph->sip[0] == 169 && arph->sip[1] == 254)
			{
				continue;
			}

			/* string source = string("%1.%2.%3.%4").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]).arg(arph->sip[3]);
			if (IPs.end() != std::find(IPs.begin(), IPs.end(), source)){
			continue;
			}
			if (!Utils::isInnerIP(source))
			{
			continue;
			}
			netGate = source;
			if (ip.empty())
			{
			start = GetTickCount() - 29 * 1000;
			}

			ip = string("%1.%2.%3.%4").arg(arph->dip[0]).arg(arph->dip[1]).arg(arph->dip[2]).arg(arph->dip[3]);
			if (arph->dip[0] != arph->sip[0] || arph->dip[1] != arph->sip[1] || arph->dip[2] != arph->sip[2] || ip == netGate)
			{
			if (arph->sip[3] != 44)
			{
			ip = string("%1.%2.%3.44").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]);
			}
			else{
			ip = string("%1.%2.%3.254").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]);
			}
			}*/

			cout << "arp" << ip << netGate;
		}
		else
		{
			if (arph->dip[0] == 0)
			{
				continue;
			}
			if (arph->dip[0] == 169 && arph->dip[1] == 254)
			{
				continue;
			}

			/*string source  = string("%1.%2.%3.%4").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]).arg(arph->sip[3]);
			if (IPs.end() != std::find(IPs.begin(), IPs.end(), source)){
			continue;
			}

			if (!Utils::isInnerIP(source))
			{
			continue;
			}

			netGate = source;
			if (arph->sip[3] != 44)
			{
			ip = string("%1.%2.%3.44").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]);
			}
			else{
			ip = string("%1.%2.%3.254").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]);
			}*/

			break;
		}

	}

	//return !ip.empty();
	return true;
}

bool CWindowUtils::getDirectDevice(string& ip, string& netGate, std::set<string>& otherIPS, int secondsWait){

	ip.clear();
	struct tm * timeinfo;
	struct tm *ltime;
	time_t rawtime;

	int result;
	int i = 0, inum;
	pcap_if_t * alldevs;//指向pcap_if_t结构列表指针
	pcap_t * adhandle;//定义包捕捉句柄
	char errbuf[PCAP_ERRBUF_SIZE];//错误缓冲最小为256
	u_int netmask; //定义子网掩码
	char packet_filter[] = "ether proto \\arp";
	struct bpf_program fcode;
	struct pcap_pkthdr * header;
	const u_char * pkt_data;

	//当前所有可用的网络设备
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		cout << "Error in pcap_findalldevs:" << errbuf;
		return false;
	}

	if (!alldevs)
	{
		cout << "cannot find net device!  install WinPcap?";
		return false;
	}

	cout << alldevs->description << alldevs->name;

	string uuid = CWindowUtils::getLocalUuid();

	string pcap_name = ConvertNICUUIDtoPcapName(alldevs, uuid);

	if ((adhandle = pcap_open_live(pcap_name.data(), 65536, 1, 1000, errbuf)) == NULL)
	{
		//cout << string("kevin : pcap_open_live failed!  not surpport by WinPcap ? alldev->name : %1").arg(alldevs->name);
		pcap_freealldevs(alldevs);
		return false;
	}

	if (pcap_datalink(adhandle) != DLT_EN10MB || alldevs->addresses == NULL) {
		cout << "kevin : pcap_datalink(adhandle) != DLT_EN10MB || alldevs->addresses == NULL";
		return false;
	}


	netmask = ((struct sockaddr_in *)(alldevs->addresses->netmask))->sin_addr.S_un.S_addr;
	pcap_freealldevs(alldevs);
	std::map<string, std::set<string>> mpDestSource;

	//编译过滤器，只捕获ARP包
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{
		cout << "unable to compile the packet filter.Check the syntax.";
		return false;
	}

	//设置过滤器
	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		cout << "Error setting the filter.";
		return false;
	}
	string specialIP;
	std::vector<string> IPs;
	getLocalIPs(IPs);
	int start = GetTickCount();
	const int MAX_WAIT_OTHER_IP_SECONDS = 10;
	while (true)
	{
		if (GetTickCount() - start > secondsWait * 1000)
		{
			cout << "arp time out";
			break;
		}
		//循环解析ARP数据包
		if (pcap_next_ex(adhandle, &header, &pkt_data) == 0){
			continue;
		}

		arpp* arph = (arpp *)(pkt_data + 14);
		if (arph->op == 256) //arp
		{
			if (arph->sip[0] == 0)
			{
				continue;
			}
			if (arph->sip[0] == 169 && arph->sip[1] == 254)
			{
				continue;
			}

			string source /*= string("%1.%2.%3.%4").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]).arg(arph->sip[3])*/;
			string destIP /*= string("%1.%2.%3.%4").arg(arph->dip[0]).arg(arph->dip[1]).arg(arph->dip[2]).arg(arph->dip[3])*/;
			if (IPs.end() != std::find(IPs.begin(), IPs.end(), source)){
				continue;
			}
			if (source == INNDER_SPECIAL_IP)
			{
				specialIP = destIP;
				continue;
			}
			if (!Utils::isInnerIP(source))
			{
				continue;
			}

			/* if (netGate.empty())
			{
			netGate = source;
			}*/

			/*if (ip.empty() && secondsWait * 1000 - GetTickCount() + start > MAX_WAIT_OTHER_IP_SECONDS * 1000)
			{
			start = GetTickCount() - (secondsWait - MAX_WAIT_OTHER_IP_SECONDS) * 1000;
			ip = destIP;
			if (arph->dip[0] != arph->sip[0] || arph->dip[1] != arph->sip[1] || arph->dip[2] != arph->sip[2] || ip == netGate)
			{
			if (arph->sip[3] != 1)
			{
			ip = string("%1.%2.%3.1").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]);
			}
			else{
			ip = string("%1.%2.%3.254").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]);
			}
			}
			}*/


			otherIPS.insert(source);
			if (Utils::isInnerIP(destIP))
			{
				mpDestSource[destIP].insert(source);
			}


			cout << "arp" << source << destIP;
		}
		else
		{
			if (arph->dip[0] == 0)
			{
				continue;
			}
			if (arph->dip[0] == 169 && arph->dip[1] == 254)
			{
				continue;
			}

			string source/* = string("%1.%2.%3.%4").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]).arg(arph->sip[3])*/;
			string dest/* = string("%1.%2.%3.%4").arg(arph->dip[0]).arg(arph->dip[1]).arg(arph->dip[2]).arg(arph->dip[3])*/;
			if (IPs.end() != std::find(IPs.begin(), IPs.end(), source)){
				continue;
			}
			if (!Utils::isInnerIP(source))
			{
				continue;
			}

			netGate = source;
			otherIPS.insert(source);

			/* if (ip.empty() && secondsWait * 1000 - GetTickCount() + start > MAX_WAIT_OTHER_IP_SECONDS * 1000)
			{
			start = GetTickCount() - (secondsWait - MAX_WAIT_OTHER_IP_SECONDS) * 1000;
			}

			ip = string("%1.%2.%3.%4").arg(arph->sip[0]).arg(arph->sip[1]).arg(arph->sip[2]).arg(44);*/
			cout << "aarp" << source << dest;
			// break;
		}

	}

	std::map<string, std::set<string>>::iterator itNetgate = mpDestSource.end();
	for (auto it = mpDestSource.begin(); it != mpDestSource.end(); it++)
	{
		if (it->second.size() > 1 &&
			(itNetgate == mpDestSource.end() || itNetgate->second.size() < it->second.size()))
		{
			itNetgate = it;
		}
	}
	if (itNetgate != mpDestSource.end())
	{
		netGate = itNetgate->first;
		string s /*= netGate.left(netGate.lastIndexOf(".") + 1)*/;
		for (int i = 2; i < 255; i++)
		{
			ip = s + std::to_string(i);
			if (ip != netGate && itNetgate->second.find(ip) == itNetgate->second.end())
			{
				break;
			}
		}
	}
	else{
		/*if (netGate.empty() && !specialIP.empty())
		{
		netGate = specialIP;
		}
		if (!netGate.empty())
		{
		netGate = netGate.left(netGate.lastIndexOf(".") + 1) + "1";
		for (int i = 2; i < 255; i++)
		{
		ip = netGate.left(netGate.lastIndexOf(".") + 1) + string::number(i);
		if (ip != specialIP)
		{
		break;
		}
		}
		}*/
	}


	//return !ip.empty();
	return true;
}
bool CWindowUtils::setIPByDHCP(string& ip, string& mask, string& netGate){
	string sName = GetNetCardName(CWindowUtils::getLocalUuid());
	cout << __FILE__ << __FUNCTION__ << __LINE__ << sName;
	CWindowUtils::setNetDhcp(sName);
	std::vector<string> ips;
	int maxPingTime = 1000 * 3;
	::Sleep(2000);
	for (CWindowUtils::GetIPfromLocalNIC(ips); ips.size() == 0 && maxPingTime > 0; CWindowUtils::GetIPfromLocalNIC(ips)){
		::Sleep(1000);
		maxPingTime -= 1000;
	}

	bool r = false;
	if (ips.size() > 0)
	{
		_IP_ADAPTER_INFO* pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(PIP_ADAPTER_INFO));
		ULONG uOutBufLen = sizeof(PIP_ADAPTER_INFO);//存放网卡信息的缓冲区大小
		//第一次调用GetAdapterInfo获取ulOutBufLen大小
		int errorNo = GetAdaptersInfo(pIpAdapterInfo, &uOutBufLen);
		if (errorNo == ERROR_BUFFER_OVERFLOW)
		{
			//获取需要的缓冲区大小
			free(pIpAdapterInfo);
			pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(uOutBufLen);//分配所需要的内存
			errorNo = (GetAdaptersInfo(pIpAdapterInfo, &uOutBufLen));
		}

		if (NO_ERROR == errorNo)
		{
			while (pIpAdapterInfo && (!r))
			{
				for (IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
					pIpAddrString != NULL && (!r); pIpAddrString = pIpAddrString->Next){
					if (*ips.begin() == pIpAddrString->IpAddress.String)
					{
						if (!CWindowUtils::setNetConfig(sName, *ips.begin(), pIpAddrString->IpMask.String, pIpAdapterInfo->GatewayList.IpAddress.String, true))
						{
							break;
						}
						ip = *ips.begin();
						mask = pIpAddrString->IpMask.String;
						netGate = pIpAdapterInfo->GatewayList.IpAddress.String;
						r = true;
					}


				}
				pIpAdapterInfo = pIpAdapterInfo->Next;
			}
		}
		free(pIpAdapterInfo);
	}

	return r;
}



const string& CWindowUtils::getLoacalNetName()
{
	static string localNetName;
	if (localNetName.empty())
	{
		string sUuid;
		vector<string> IpList;
		GetLocalNetCar(localNetName, sUuid, IpList);
	}

	return localNetName;
}

bool CWindowUtils::isOnLine(){

	string localNetName = CWindowUtils::getLoacalNetName();

	if (localNetName.empty())
	{
		return false;
	}

	DWORD dwSize = sizeof (MIB_IFTABLE);
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	char *pIfTable = new char[dwSize];
	MIB_IFROW *pIfRow;

	if (GetIfTable((MIB_IFTABLE *)pIfTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
		delete pIfTable;
		pIfTable = new char[dwSize];
	}

	bool r = false;
	if ((dwRetVal = GetIfTable((MIB_IFTABLE *)pIfTable, &dwSize, 0)) == NO_ERROR) {
		MIB_IFTABLE *pTable = (MIB_IFTABLE *)pIfTable;
		if (pTable->dwNumEntries > 0) {
			MIB_IFROW IfRow;
			for (i = 0; i < pTable->dwNumEntries; i++) {
				IfRow.dwIndex = pTable->table[i].dwIndex;
				if ((dwRetVal = GetIfEntry(&IfRow)) == NO_ERROR) {

					std::string sName((char *)IfRow.bDescr);
					if (IfRow.dwType != MIB_IF_TYPE_ETHERNET || sName.compare(localNetName) != 0)
					{
						continue;
					}

					switch (IfRow.dwOperStatus) {
					case IF_OPER_STATUS_NON_OPERATIONAL:
						//cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "Non Operational" << endl;
						break;
					case IF_OPER_STATUS_UNREACHABLE:
						cout << __FILE__ << __FUNCTION__ << __LINE__ << "Unreasonable" << endl;
						break;
					case IF_OPER_STATUS_DISCONNECTED:
						cout << __FILE__ << __FUNCTION__ << __LINE__ << "Disconnected" << endl;
						break;
					case IF_OPER_STATUS_CONNECTING:
						cout << __FILE__ << __FUNCTION__ << __LINE__ << "Connecting" << endl;
						r = true;
						break;
					case IF_OPER_STATUS_CONNECTED:
						cout << __FILE__ << __FUNCTION__ << __LINE__ << "Connected" << endl;
						r = true;
						break;
					case IF_OPER_STATUS_OPERATIONAL:
						//cout << __FILE__ << __FUNCTION__ << __LINE__ << "Operational" << endl;
						r = true;
						break;
					default:
						cout << __FILE__ << __FUNCTION__ << __LINE__ << "Unknown status" << IfRow.dwAdminStatus << endl;
						break;
					}
				}

				else {
					cout << __FILE__ << __FUNCTION__ << __LINE__ << "GetIfEntry failed for index with error:" << dwRetVal << endl;
				}
			}
		}
		else {
			cout << __FILE__ << __FUNCTION__ << __LINE__ << "GetIfTable failed with error:" << dwRetVal << endl;
		}

	}
	delete pIfTable;
	return r;
}

const string& CWindowUtils::getLocalUuid()
{
	static string uuid;
	if (uuid.empty())
	{
		string sNetName;
		vector<string> IpList;
		GetLocalNetCar(sNetName, uuid, IpList);
	}
	return uuid;
}