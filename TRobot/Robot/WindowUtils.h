#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <set>
#include <iostream>
#include <vector>
#include <pcap.h>

using namespace std;

typedef struct ip_mac
{
	long ip;
	short port;
	unsigned char mac[6];
}IPMAC, *PIPMAC;

class WindowUtils
{
public:
	static bool isValidNetMacaddress(const std::string& macaddress);
	static void GetIPfromLocalNIC(std::vector<string> &IPs);
	static void getLocalIPs(std::vector<string> &IPs);
	static bool setNetConfig(const string& sName, const string& sIP, const string& sMask = "255.255.255.0", const string& sGate = "", bool bWait = false);
	static bool setNetDhcp(const string& sName);
	static bool isConnecteTo(const string& IP, int millSeconds = 500);
	static bool getDirectDevice(string& ip, string& netGate);
	static bool getDirectDevice(string& ip, string& netGate, std::set<string>& otherIPS, long secondsWait = 60);
	static bool setIPByDHCP(string& ip, string& mask, string& netGate);
	static const string& getLoacalNetName();
	static const string& WindowUtils::getLocalUuid();
	static bool isOnLine();
	static void getMacByArpTable(vector<string> Ips, vector<IPMAC>& IpMacs);
	static string ConvertNICUUIDtoPcapName(pcap_if_t* devs, const string& uuid);
	static void getLocalIPsMac(std::vector<string> &IPs, u_char *mac);
private:
	WindowUtils();
};

#endif // WINDOWUTILS_H