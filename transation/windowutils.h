#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H



#include <set>
#include <iostream>
#include <vector>

using namespace std;

class WindowUtils
{
public:
    static bool isValidNetMacaddress(const std::string& macaddress);
	static void GetIPfromLocalNIC(std::vector<string> &IPs);
	static void getLocalIPs(std::vector<string> &IPs);
    static void getLocalIPs(const string& sHostName, std::vector<string> &IPs);
    static bool findProcessByName(const char* szFileName);
    static void terminateProcess(const char* szFileName);
    //static void copy(const QFileInfo& source, const QDir& dest, const string& destName = "");
    static bool setNetConfig(const string& sName, const string& sIP, const string& sMask = "255.255.255.0", const string& sGate = "", bool bWait = false);
    static bool setNetDhcp(const string& sName);

    static bool isConnecteTo(const string& IP, int millSeconds = 500);
    static bool getDirectDevice(string& ip, string& netGate);
    static bool getDirectDevice(string& ip, string& netGate, std::set<string>& otherIPS, int secondsWait = 60);
    static bool setIPByDHCP(string& ip, string& mask, string& netGate);
    static const string& getLoacalNetName();
    static bool isOnLine();
private:
    WindowUtils();
};

#endif // WINDOWUTILS_H
