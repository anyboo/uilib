#pragma once

#include <vector>
#include <string>
#include <set>

class CWindowUtils
{
public:
	static bool isValidNetMacaddress(const std::string& macaddress);
	static void GetIPfromLocalNIC(std::vector<std::string> &IPs);
	static void getLocalIPs(std::vector<std::string> &IPs);
	static void getLocalIPs(const std::string& sHostName, std::vector<std::string> &IPs);
	static bool setNetConfig(const std::string& sName, const std::string& sIP, const std::string& sMask = "255.255.255.0", const std::string& sGate = "", bool bWait = false);
	static bool setNetDhcp(const std::string& sName);
	static bool isConnecteTo(const std::string& IP, int millSeconds = 500);
	static bool getDirectDevice(std::string& ip, std::string& netGate);
	static bool getDirectDevice(std::string& ip, std::string& netGate, std::set<std::string>& otherIPS, int secondsWait = 60);
	static bool setIPByDHCP(std::string& ip, std::string& mask, std::string& netGate);
	static const std::string& getLoacalNetName();
	static const std::string& getLocalUuid();
	static bool isOnLine();

private:
	CWindowUtils();
};

