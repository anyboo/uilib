#ifndef PORT_SCAN_H
#define PORT_SCAN_H

#include <iostream>
#include <vector>
#include "pcap.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/AutoPtr.h"

#include "windowutils.h"


using namespace std;
using Poco::Notification;
using Poco::NotificationQueue;
using Poco::ThreadPool;
using Poco::Thread;
using Poco::Runnable;
using Poco::FastMutex;
using Poco::AutoPtr;


typedef struct ScanResult
{
	char ip[22];
	short port;
}SCANRESULT, *PSCANRESULT;




struct SendPacket
{
	SendPacket()
	{
		packet = nullptr;
		size = 0;
	}

	~SendPacket()
	{
		if (nullptr != packet)
		{
			delete packet;
			packet = nullptr;
			size = 0;
		}
	}

	SendPacket(const SendPacket& other)
	{
		size = other.size;
		packet = nullptr;
		setPacketData(other.packet, other.size);
	}

	SendPacket(const char* data, const short len)
	{
		size = len;
		packet = nullptr;
		setPacketData((char *)data, len);
	}


	void setPacketData(char* pData, int len)
	{
		if (nullptr != packet)
		{
			delete packet;
			packet = nullptr;
			size = 0;
		}

		if (len > 0)
		{
			packet = new char[len];
			size = len;
			memcpy(packet, pData, len);
		}
	}

	SendPacket& operator= (const SendPacket& other)
	{
		if (&other == this)
		{
			return *this;
		}
		size = other.size;
		setPacketData(other.packet, other.size);
		return *this;
	}

	//取得私有数据
	char* getPacket() const
	{
		return packet;
	}
	short getPacketSize() const
	{
		return size;
	}

	char *packet;
	short size;
};

class SendData : public Poco::Runnable
{
public:
	SendData(const IPMAC srcIp, const IPMAC dstIp, const std::vector<short> ports, Poco::NotificationQueue& queue, pcap_t *adhandle) :		
		_ports(ports),
		_queue(queue),
		_adhandle(adhandle)
	{
		_srcIp.ip = srcIp.ip;
		memcpy(_srcIp.mac, srcIp.mac, 6);
		_dstIp.ip = dstIp.ip;
		memcpy(_dstIp.mac, dstIp.mac, 6);
		in_addr dstaddr;
		dstaddr.S_un.S_addr = dstIp.ip;
		_name = inet_ntoa(dstaddr);
	}	
	void run();

private:
	void initHeader(struct iphdr *ip, struct tcphdr *tcp, struct pseudohdr *pseudoheader, long dst_ip, USHORT dst_port);
	void buildTcpPacket(IPMAC srcip, IPMAC dstip, vector<short> ports, vector<SendPacket>& packets);
	void sendData(pcap_t * adhandle, vector<SendPacket> packets);

	unsigned short inline checksum(unsigned short *buffer, unsigned short size)
	{

		unsigned long cksum = 0;

		while (size > 1){
			cksum += *buffer++;
			size -= sizeof(unsigned short);
		}

		if (size){
			cksum += *(unsigned char *)buffer;
		}

		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >> 16);

		return((unsigned short)(~cksum));
	}

private:
	pcap_t*			   _adhandle;
	IPMAC			   _srcIp;
	IPMAC			   _dstIp;
	vector<short>      _ports;
	Poco::NotificationQueue& _queue;
	static Poco::FastMutex   _mutex;
	std::string        _name;

};

class ReceiveData : public Poco::Runnable
{
public:
	ReceiveData(const std::vector<long> ips, const std::vector<short> ports, pcap_t *adhandle) :
		_adhandle(adhandle),
		_ips(ips),
		_ports(ports)
	{
		
	}

	void run();
	vector<SCANRESULT> getScanResult();
private:
	void analyzePacket(const u_char *pkt_data, u_short size, vector<long> ips, vector<short> ports, vector<SCANRESULT>& outIps);
private:
	 pcap_t*			   _adhandle;
	 vector<long>     _ips;
	 vector<short>      _ports;	
	 vector<SCANRESULT> _outIps;
};


class ScanNotification : public Notification
	// The notification sent to worker threads.
{
public:
	typedef AutoPtr<ScanNotification> Ptr;

	ScanNotification(int workData) :
		_data(workData)
	{
	}

	int data() const
	{
		return _data;
	}

private:
	int _data;
};


class PortScan
{
public:
	PortScan::PortScan();

	~PortScan();

	bool searchFactory(vector<short> ports, vector<SCANRESULT>& outIps);
private:
		
	bool initPcapDev();

private:
	pcap_t * _adhandle;
	vector<string> _localIps;
	u_char   _localMac[6];
};

#endif