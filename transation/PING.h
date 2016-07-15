#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#endif


 #include <windows.h>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <thread>
#include <list>
#include <memory>
#include <mutex>
using namespace std;
//这里需要导入库 Ws2_32.lib，在不同的IDE下可能不太一样 
#pragma comment(lib, "Ws2_32.lib")

#define DEF_PACKET_SIZE 32
#define ECHO_REQUEST 8
#define ECHO_REPLY 0

struct IPHeader
{
	BYTE m_byVerHLen;   //4位版本+4位首部长度
	BYTE m_byTOS;       //服务类型
	USHORT m_usTotalLen; //总长度
	USHORT m_usID;      //标识
	USHORT m_usFlagFragOffset; //3位标志+13位片偏移
	BYTE m_byTTL; //TTL
	BYTE m_byProtocol; //协议
	USHORT m_usHChecksum; //首部检验和
	ULONG m_ulSrcIP; //源IP地址
	ULONG m_ulDestIP; //目的IP地址
};

struct ICMPHeader
{ 
	BYTE m_byType;   //类型
	BYTE m_byCode;   //代码
	USHORT m_usChecksum; //检验和 
	USHORT m_usID; //标识符
	USHORT m_usSeq; //序号
	ULONG m_ulTimeStamp; //时间戳（非标准ICMP头部）
};

struct PingReply
{
	USHORT m_usSeq;
	DWORD m_dwRoundTripTime;
	DWORD m_dwBytes;
	DWORD m_dwTTL;
};
struct OnePingInfo
{
    SOCKET m_sockRaw;
    WSAEVENT m_event;
    bool m_bStop;
    char* m_szICMPData;
    std::shared_ptr<std::thread> m_pReadThread;
    OnePingInfo();
    ~OnePingInfo();
};
class CPing
{

    static CPing* spInstance;
    CPing(); 
    bool init(std::shared_ptr<OnePingInfo> pingInfo);
    ~CPing();
    
public:
    static CPing& instance(){
        if (NULL == spInstance)
        {
            spInstance = new CPing();
        }
        
        return *spInstance;
    }

    void ScanIp(vector<string> &vecIp, bool bArp = false, std::shared_ptr<bool> bpCancel = std::shared_ptr<bool>());
    void ScanIPs(vector<string> &vecIP, const string& netIp, bool bArp = false, std::shared_ptr<bool> bpCancel = std::shared_ptr<bool>());
    bool ScanOneIP(string &ip, const string& netIp, bool bArp = false);
    BOOL Ping(DWORD dwDestIP, long dwTimeout = 20, bool bArp = false);
    BOOL Ping(const char *szDestIP, long dwTimeout = 20, bool bArp = false);

private:
    BOOL PingCore(DWORD dwDestIP, long dwTimeout, std::shared_ptr<OnePingInfo> pingInfo, bool bArp = false);
	USHORT CalCheckSum(USHORT *pBuffer, int nSize);
	ULONG GetTickCountCalibrate();
private:

	USHORT m_usCurrentProcID;



    std::shared_ptr<OnePingInfo> mpLastPingInfo;
    std::recursive_mutex mMtLastPingInfo;
private:
	static ULONG s_usPacketSeq;
    
};