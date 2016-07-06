
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
	PIP_ADAPTER_INFO pfirstIpAdapterInfo;

	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		pfirstIpAdapterInfo = pIpAdapterInfo;
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
		delete[]pfirstIpAdapterInfo;		
	}	
}


//get connect name 
string GetNetCardName(std::string uuid)
{
	string CardName;
	HKEY hKey;
	string strKeyName = "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\";
	strKeyName += uuid;
	strKeyName += "\\Connection";
	LONG lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, strKeyName.c_str(), 0, KEY_QUERY_VALUE, &hKey);
	if (lRet != ERROR_SUCCESS)
		return CardName;

	char sz[20] = { 0 };
	DWORD dwSize = sizeof(sz);
	if (RegQueryValueExA(hKey, "Name", NULL, NULL, (LPBYTE)sz, &dwSize) == ERROR_SUCCESS)
	{		
		CardName = string(sz);
	}

	
	return CardName;
}

string ConvertNICUUIDtoPcapName(pcap_if_t* devs, const string& uuid)
{
	string pcap_name;
	
	for (pcap_if_t *d = devs; d && d->next; d = d->next)
	{
		string tmp(d->name);		
		if (tmp.compare(uuid) == 0)
			pcap_name = tmp;
	}

	return pcap_name;
}

WindowUtils::WindowUtils()
{
}


bool WindowUtils::isValidNetMacaddress(const string& macaddress)
{

	return !macaddress.empty() && (macaddress.find("00:00:00:00:00:00:00:E0") == string::npos)
            && (macaddress.find("00:50:56:C0") == string::npos);
}

void WindowUtils::GetIPfromLocalNIC(std::vector<string> &IPs)
{
	cout << "GetIPfromLocalNIC start";
	getLocalIPs(IPs);
	cout << "GetIPfromLocalNIC end";
}

void WindowUtils::getLocalIPs(std::vector<string> &IPs)
{
    IPs.clear();
	string sDesc;
	string uuid;
	GetLocalNetCar(sDesc, uuid, IPs);
}


bool WindowUtils::setNetConfig(const string& sName, const string& sIP, const string& sMask, const string& sGate, bool bWait){

	string mask(" mask=");
	mask.append(sMask);
	string sNetName = GetNetCardName(sName);
	string name(" name = \"");
	name += sNetName;
	name.append("\"");
	string addr(" addr=");
	addr += sIP;
	string argList("interface ip set address");
	argList += name;
	argList.append(" source = static "); 
	argList = argList + addr + mask;
	
    if (!sGate.empty())
    {        
		string gateway(" gateway=");
		gateway += sGate;
		argList += gateway;    
		
    }
	::ShellExecuteA(NULL, (LPCSTR)"open", (LPCSTR)"netsh.exe", (LPCSTR)argList.c_str(), NULL, SW_SHOWNORMAL);
   
    if (!bWait)
    {
        return true;
    }
    int maxPingTime = 1000 * 3;
    ::Sleep(2000);
    while (maxPingTime > 0 && !CPing::instance().Ping(sIP.c_str(), 20)){
        ::Sleep(1000);
        maxPingTime -= 1000;
    }
    
    return maxPingTime > 0;
}

bool WindowUtils::setNetDhcp(const string& sName){  
	string sCmd("interface ip set address name=\"");
	sCmd += sName;
	sCmd.append("\" source=dhcp");
	::ShellExecuteA(NULL, (LPCSTR)"open", (LPCSTR)"netsh.exe", (LPCSTR)sCmd.c_str(), NULL, SW_SHOWNORMAL);	
    return true;
}

bool WindowUtils::isConnecteTo(const string& IP, int millSeconds){
    return CPing::instance().Ping(IP.c_str(), millSeconds);
}


typedef struct arppkt
{
    unsigned short hdtyp;//Ӳ������
    unsigned short protyp;//Э������
    unsigned char hdsize;//Ӳ����ַ����
    unsigned char prosize;//Э���ַ����
    unsigned short op;//���������ͣ�����ֵ:ARP/RARP
    u_char smac[6];//ԴMAC��ַ
    u_char sip[4];//ԴIP��ַ
    u_char dmac[6];//Ŀ��MAC��ַ
    u_char dip[4];//Ŀ��IP��ַ
}arpp;


#define  INNDER_SPECIAL_IP "170.151.24.203"
bool WindowUtils::getDirectDevice(string& ip, string& netGate)
{
    ip.clear();
    struct tm * timeinfo;
    struct tm *ltime;
    time_t rawtime;

    int result;
    int i = 0, inum;
    pcap_if_t * alldevs;//ָ��pcap_if_t�ṹ�б�ָ��
    pcap_t * adhandle;//�������׽���
    char errbuf[PCAP_ERRBUF_SIZE];//���󻺳���СΪ256
    u_int netmask; //������������
    char packet_filter[] = "ether proto \\arp";
    struct bpf_program fcode;
    struct pcap_pkthdr * header;
    const u_char * pkt_data;
    //����־�ļ�

    //��ǰ���п��õ������豸
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
    

    //�����������ֻ����ARP��
    if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
    {
        cout << "unable to compile the packet filter.Check the syntax.";
        return false;
    }

    //���ù�����
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
        //ѭ������ARP���ݰ�
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

			char szIP[30] = { 0 };
			sprintf_s(szIP, "%d.%d.%d.%d", arph->sip[0], arph->sip[1], arph->sip[2], arph->sip[3]);

			string source = string(szIP);
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

			char szdIP[30] = { 0 };
			sprintf_s(szdIP, "%d.%d.%d.%d", arph->dip[0], arph->dip[1], arph->dip[2], arph->dip[3]);
			ip = string(szdIP);
            if (arph->dip[0] != arph->sip[0] || arph->dip[1] != arph->sip[1] || arph->dip[2] != arph->sip[2] || ip == netGate)
            {
                if (arph->sip[3] != 44)
                {
					char sztmp[30] = { 0 };
					sprintf_s(sztmp, "%d.%d.%d.44", arph->sip[0], arph->sip[1], arph->sip[2]);
					ip = string(sztmp);
                }
                else{
					char sztmp[30] = { 0 };
					sprintf_s(sztmp, "%d.%d.%d.254", arph->sip[0], arph->sip[1], arph->sip[2]);
					ip = string(sztmp);
                }
            }

            cout << "arp    " << ip << "  " << netGate << endl;
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

			char szsource[30] = { 0 };
			sprintf_s(szsource, "%d.%d.%d.%d", arph->sip[0], arph->sip[1], arph->sip[2], arph->sip[3]);
			string source = string(szsource);
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
				char sztmp[30] = { 0 };
				sprintf_s(sztmp, "%d.%d.%d.44", arph->sip[0], arph->sip[1], arph->sip[2]);
				ip = string(sztmp);
            }
            else{
				char sztmp[30] = { 0 };
				sprintf_s(sztmp, "%d.%d.%d.254", arph->sip[0], arph->sip[1], arph->sip[2]);
				ip = string(sztmp);
            }
           
            break;
        }

    }

    return !ip.empty();	
}

bool WindowUtils::getDirectDevice(string& ip, string& netGate, std::set<string>& otherIPS, int secondsWait){

    ip.clear();
    struct tm * timeinfo;
    struct tm *ltime;
    time_t rawtime;

    int result;
    int i = 0, inum;
    pcap_if_t * alldevs;//ָ��pcap_if_t�ṹ�б�ָ��
    pcap_t * adhandle;//�������׽���
    char errbuf[PCAP_ERRBUF_SIZE];//���󻺳���СΪ256
    u_int netmask; //������������
    char packet_filter[] = "ether proto \\arp";
    struct bpf_program fcode;
    struct pcap_pkthdr * header;
    const u_char * pkt_data;

    //��ǰ���п��õ������豸
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
	
	string uuid = WindowUtils::getLocalUuid();
	
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

    //�����������ֻ����ARP��
    if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
    {
        cout << "unable to compile the packet filter.Check the syntax.";
        return false;
    }

    //���ù�����
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
        //ѭ������ARP���ݰ�
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

			char szsip[30] = { 0 };
			sprintf_s(szsip, "%s.%s.%s.%s", arph->sip[0], arph->sip[1], arph->sip[2], arph->sip[3]);
			string source(szsip);
			char szdip[30] = { 0 };
			sprintf_s(szdip, "%s.%s.%s.%s", arph->dip[0], arph->dip[1], arph->dip[2], arph->dip[3]);
            string destIP(szdip) ;
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
            
            if (netGate.empty())
            {
                netGate = source;
            }
            
            if (ip.empty() && secondsWait * 1000 - GetTickCount() + start > MAX_WAIT_OTHER_IP_SECONDS * 1000)
            {
                start = GetTickCount() - (secondsWait - MAX_WAIT_OTHER_IP_SECONDS) * 1000;
                ip = destIP;
                if (arph->dip[0] != arph->sip[0] || arph->dip[1] != arph->sip[1] || arph->dip[2] != arph->sip[2] || ip == netGate)
                {
                    if (arph->sip[3] != 1)
                    {
						char sztmp[30] = { 0 };
						sprintf_s(sztmp, "%s.%s.%s.1", arph->sip[0], arph->sip[1], arph->sip[2]);
						ip = string(sztmp);
                    }
                    else{
						char sztmp[30] = { 0 };
						sprintf_s(sztmp, "%s.%s.%s.254", arph->sip[0], arph->sip[1], arph->sip[2]);
						ip = string(sztmp);
                    }
                }
            }
            
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
            
			char szsource[30] = { 0 };
			sprintf_s(szsource, "%s.%s.%s.%s", arph->sip[0], arph->sip[1], arph->sip[2], arph->sip[3]);
            string source(szsource);
			char szdest[30] = { 0 };
			sprintf_s(szdest, "%s.%s.%s.%s", arph->dip[0], arph->dip[1], arph->dip[2], arph->dip[3]);
			string dest(szdest);
            if (IPs.end() != std::find(IPs.begin(), IPs.end(), source)){
                continue;
            }
            if (!Utils::isInnerIP(source))
            {
                continue;
            }

            netGate = source;
            otherIPS.insert(source);
            
            if (ip.empty() && secondsWait * 1000 - GetTickCount() + start > MAX_WAIT_OTHER_IP_SECONDS * 1000)
            {
                start = GetTickCount() - (secondsWait - MAX_WAIT_OTHER_IP_SECONDS) * 1000;
            }
			char sztmp[30] = { 0 };
			sprintf_s(sztmp, "%s.%s.%s.44", arph->sip[0], arph->sip[1], arph->sip[2]);
			ip = string(sztmp);
            cout << "aarp" << source << dest;           
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
        string s = netGate.substr(0, netGate.find_last_of(".") + 1);
        for (int i  = 2; i < 255; i++)
        {
            ip = s + std::to_string(i);
            if (ip != netGate && itNetgate->second.find(ip) == itNetgate->second.end())
            {
                break;
            }
        }
    }
    else{
        if (netGate.empty() && !specialIP.empty())
        {
            netGate = specialIP;
        }
        if (!netGate.empty())
        {
			netGate = netGate.substr(0, netGate.find_last_of(".") + 1) + "1";
            for (int i = 2; i < 255; i++)
            {
				ip = netGate.substr(0, netGate.find_last_of(".") + 1) + std::to_string(i);
                if (ip != specialIP)
                {
                    break;
                }
            }
        }
    }


    return !ip.empty();
	
}
bool WindowUtils::setIPByDHCP(string& ip, string& mask, string& netGate){
	string sName = GetNetCardName(WindowUtils::getLocalUuid());
    cout<<__FILE__<<__FUNCTION__<<__LINE__<<sName;
    WindowUtils::setNetDhcp(sName);
    std::vector<string> ips;
    int maxPingTime = 1000 * 3;
    ::Sleep(2000);
	for (WindowUtils::GetIPfromLocalNIC(ips); ips.size() == 0 && maxPingTime > 0; WindowUtils::GetIPfromLocalNIC(ips)){
        ::Sleep(1000);
        maxPingTime -= 1000;
    }
    
    bool r = false;

	if (ips.size() == 1)
	{
		string stmp = ips[0].substr(0, ips[0].find_last_of("."));
		stmp = stmp.substr(0, stmp.find_last_of("."));
		if (stmp.compare("169.254") == 0)
			return r;
	}
    if (ips.size() > 0)
    {
        _IP_ADAPTER_INFO* pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(PIP_ADAPTER_INFO));
		_IP_ADAPTER_INFO* pfirstAdapterInfo = pIpAdapterInfo;
        ULONG uOutBufLen = sizeof(PIP_ADAPTER_INFO);//���������Ϣ�Ļ�������С
        //��һ�ε���GetAdapterInfo��ȡulOutBufLen��С
        int errorNo = GetAdaptersInfo(pIpAdapterInfo, &uOutBufLen);
        if (errorNo == ERROR_BUFFER_OVERFLOW)
        {
            //��ȡ��Ҫ�Ļ�������С
            free(pIpAdapterInfo);
            pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(uOutBufLen);//��������Ҫ���ڴ�
			pfirstAdapterInfo = pIpAdapterInfo;
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
						if (!WindowUtils::setNetConfig(pIpAdapterInfo->AdapterName, *ips.begin(), pIpAddrString->IpMask.String, pIpAdapterInfo->GatewayList.IpAddress.String, true))
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
		free(pfirstAdapterInfo);
    }

    return r;
}



const string& WindowUtils::getLoacalNetName()
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

bool WindowUtils::isOnLine(){
    
    string localNetName = WindowUtils::getLoacalNetName();
    
    if (localNetName.empty())
    {
        return false;
    }  

    DWORD dwSize = sizeof (MIB_IFTABLE);
    DWORD dwRetVal = 0;

    unsigned int i;

    /* variables used for GetIfTable and GetIfEntry */
    char *pIfTable = new char[dwSize];   

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
                        cout << __FILE__ << __FUNCTION__ << __LINE__ << "Non Operational";
                        break;
                    case IF_OPER_STATUS_UNREACHABLE:
                        cout << __FILE__ << __FUNCTION__ << __LINE__ << "Unreasonable";
                        break;
                    case IF_OPER_STATUS_DISCONNECTED:
                        cout << __FILE__ << __FUNCTION__ << __LINE__ << "Disconnected";
                        break;
                    case IF_OPER_STATUS_CONNECTING:
                        cout << __FILE__ << __FUNCTION__ << __LINE__ << "Connecting";
                        r = true;
                        break;
                    case IF_OPER_STATUS_CONNECTED:
                        cout << __FILE__ << __FUNCTION__ << __LINE__ << "Connected";
                        r = true;
                        break;
                    case IF_OPER_STATUS_OPERATIONAL:
                        cout << __FILE__ << __FUNCTION__ << __LINE__ << "Operational";
                        r = true;
                        break;
                    default:
                        cout << __FILE__ << __FUNCTION__ << __LINE__ << "Unknown status" << IfRow.dwAdminStatus;
                        break;
                    }
                }

                else {
                    cout << __FILE__ << __FUNCTION__ << __LINE__ << "GetIfEntry failed for index with error:" <<
                         dwRetVal;
                }
            }
        }
        else {
            cout << __FILE__ << __FUNCTION__ << __LINE__ << "GetIfTable failed with error:" << dwRetVal;
        }

    }
    delete pIfTable;
    return r;
}

const string& WindowUtils::getLocalUuid()
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