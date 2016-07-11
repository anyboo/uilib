#ifndef PORT_SCAN_H
#define PORT_SCAN_H

#include <iostream>
#include <vector>
#include "pcap.h"
#include "windowutils.h"

using namespace std;




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



class PortScan
{
public:
	PortScan();
	~PortScan();

	void startScan(vector<IPMAC>& scanResults);
private:
	unsigned short inline checksum(unsigned short *buffer, unsigned short size)
	{

		unsigned long cksum = 0;

		while (size>1){
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

	void initHeader(struct iphdr *ip, struct tcphdr *tcp, struct pseudohdr *pseudoheader, long dst_ip, USHORT dst_port);
	void buildTcpPacket(IPMAC srcip, IPMAC dstip, vector<short> ports, vector<SendPacket>& packets);
	void sendData(pcap_t * adhandle, vector<SendPacket> packets);
	void analyzePacket(const u_char *pkt_data, u_short size);
	bool initPcapDev();
private:
	pcap_t * m_adhandle;
};

#endif