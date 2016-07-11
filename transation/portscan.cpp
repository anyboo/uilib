
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_REMOTE

#include <winsock2.h>
#include <string>
#include "portscan.h"
#include "windowutils.h"
#include "netdefine.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/AutoPtr.h"
#include "PING.h"

using Poco::Notification;
using Poco::NotificationQueue;
using Poco::ThreadPool;
using Poco::Thread;
using Poco::Runnable;
using Poco::FastMutex;
using Poco::AutoPtr;

FastMutex SendData::_mutex;

PortScan::PortScan()
{
	_adhandle = NULL;
	initPcapDev();
	WindowUtils::getLocalIPsMac(_localIps, _localMac);
};


PortScan::~PortScan()
{
	pcap_close(_adhandle);
	_adhandle = NULL;
}



void ReceiveData::analyzePacket(const u_char *pkt_data, u_short size, vector<long> ips, vector<short> ports, vector<SCANRESULT>& outIps)
{
	struct iphdr *ip = (struct iphdr *)(pkt_data + sizeof(struct ethhdr));
	struct tcphdr *tcp = (struct tcphdr *)(pkt_data + sizeof(struct ethhdr) + sizeof(struct iphdr));


	if ((htons(tcp->dport) == 23456) && (tcp->flag == 0x12))
	{		
		in_addr srcaddr;		
		srcaddr.S_un.S_addr = ip->sourceIP;
		string srcip(inet_ntoa(srcaddr));
		cout << "src ip:" << srcip << endl;

		int i, j;
		for (i = 0; i < ips.size(); i++)
		{
			if (ip->sourceIP == ips[i])
			{
				for (j = 0; j < ports.size(); j++)
				{
					if (htons(tcp->sport) == ports[j])
					{
						char log[512] = { 0 };
						sprintf_s(log, " port src: %d, dst: %d, len: %d, flag: %02x\n",
							htons(tcp->sport), htons(tcp->dport), size, tcp->flag);
						cout << string(log) << endl;


						SCANRESULT out = { 0 };
						char *sip = inet_ntoa(srcaddr);
						memcpy(out.ip, sip, strlen(sip));
						out.port = htons(tcp->sport);
						outIps.push_back(out);
					}
				}
				
			}
		}		

		
	}

}

bool PortScan::initPcapDev()
{
	pcap_if_t *alldevs;	
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;

	//get all net device
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		cout <<  "Error in pcap_findalldevs: " << errbuf << endl;
		return false;
	}

	if (!alldevs)
	{
		cout << "cannot find net device!  install WinPcap?";
		return false;
	}

	cout << alldevs->description << alldevs->name;

	//get local device
	string uuid = WindowUtils::getLocalUuid();

	string pcap_name = WindowUtils::ConvertNICUUIDtoPcapName(alldevs, uuid);

	if ((_adhandle = pcap_open_live(pcap_name.data(), 65536, 1, 1000, errbuf)) == NULL)
	{
		cout << "kevin : pcap_open_live failed!  not surpport by WinPcap ? alldev->name : %1";
		pcap_freealldevs(alldevs);
		return false;
	}

	if (pcap_datalink(_adhandle) != DLT_EN10MB || alldevs->addresses == NULL) {
		cout << "kevin : pcap_datalink(adhandle) != DLT_EN10MB || alldevs->addresses == NULL";
		return false;
	}

	//set netmask
	netmask = ((struct sockaddr_in *)(alldevs->addresses->netmask))->sin_addr.S_un.S_addr;
	pcap_freealldevs(alldevs);
	
		
	//编译过滤器，只捕获tcp包
	char packet_filter[] = "ip and tcp";
	struct bpf_program fcode;

	if (pcap_compile(_adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{
		cout << "unable to compile the packet filter.Check the syntax.";
		return false;
	}

	//设置过滤器
	if (pcap_setfilter(_adhandle, &fcode) < 0)
	{
		cout << "Error setting the filter.";
		return false;
	}	

	return true;
}

bool PortScan::searchFactory( vector<short> ports, vector<SCANRESULT>& outIps)
{
	
	int i, j;
	NotificationQueue queue;

	outIps.clear();
	

	vector<string> Ips;
	std::shared_ptr<bool> bpCancel = std::make_shared<bool>(false);

	CPing::instance().ScanIp(Ips, false, bpCancel);
	for (i = 0; i < Ips.size(); i++)
	{
		cout << i << "   ip:  " << Ips[i] << endl;
	}

	//获取mac地址
	vector<IPMAC> IpMacs;
	if (Ips.size() > 0)
	{
		WindowUtils::getMacByArpTable(Ips, IpMacs);
	}

	for (i = 0; i < IpMacs.size(); i++)
	{
		IN_ADDR ip;
		ip.S_un.S_addr = IpMacs[i].ip;
		char sztmp[512] = { 0 };
		sprintf_s(sztmp, "111 i: %d, ip: %s, mac: %02x-%02x-%02x-%02x-%02x-%02x \n", i, inet_ntoa(ip),
			IpMacs[i].mac[0], IpMacs[i].mac[1], IpMacs[i].mac[2],
			IpMacs[i].mac[3], IpMacs[i].mac[4], IpMacs[i].mac[5]);
		printf(sztmp);
	}

	
	vector <SendData> sendThreads;
	for (i = 0; i < _localIps.size(); i++)
	{
		IPMAC localIpMac = { 0 };
		localIpMac.ip = inet_addr(_localIps[i].c_str());
		memcpy(localIpMac.mac, _localMac, 6);

		vector<SendData> sendThreads;
		for (j = 0; j < IpMacs.size(); i++)
		{
			IPMAC dstMac = { 0 };
			dstMac.ip = IpMacs[j].ip;
			memcpy(dstMac.mac, IpMacs[j].mac, 6);
			SendData sendthread(localIpMac, dstMac, ports, queue, _adhandle);
			sendThreads.push_back(sendthread);
		}
	}
	
	for (i = 0; i < sendThreads.size(); i++)
	{
		ThreadPool::defaultPool().start(sendThreads[i]);
	}

	vector<long> destIps;
	for (i = 0; i < IpMacs.size(); i++)
	{
		destIps.push_back(IpMacs[i].ip);
	}
	//start listen thread
	ReceiveData receiveThread(destIps, ports, _adhandle);
	ThreadPool::defaultPool().start(receiveThread);

	//send 
	for (int i = 0; i < sendThreads.size(); ++i)
	{
		queue.enqueueNotification(new ScanNotification(1));
	}

	while (!queue.empty()) 
		Thread::sleep(200);
	Thread::sleep(1000*10);

	queue.wakeUpAll();
	ThreadPool::defaultPool().joinAll();

	outIps = receiveThread.getScanResult();
	

	return true;
}

//build tcp head and ip head
void SendData::initHeader(struct iphdr *ip, struct tcphdr *tcp, struct pseudohdr *pseudoheader, long dst_ip, USHORT dst_port)
{
	int len = sizeof(struct iphdr) + sizeof(struct tcphdr);
	// IP头部数据初始化
	ip->hl = (4 << 4 | sizeof(struct iphdr) / sizeof(unsigned int));
	ip->tos = 0;
	ip->total_len = htons(len);
	ip->id = 1;
	ip->frag_and_flags = 0x40;
	ip->ttl = 128;
	ip->proto = IPPROTO_TCP;
	ip->checksum = 0;
	//ip->sourceIP = 0;
	ip->destIP = dst_ip;

	// TCP头部数据初始化
	tcp->sport = htons(23456);
	tcp->dport = htons(dst_port);
	tcp->seq = htonl(rand() % 90000000 + 2345);
	tcp->ack = 0;
	tcp->lenres = (sizeof(struct tcphdr) / 4 << 4 | 0);
	tcp->flag = 0x02;
	tcp->win = htons(32768);
	tcp->sum = 0;
	tcp->urp = 0;

	//TCP伪头部
	pseudoheader->zero = 0;
	pseudoheader->protocol = IPPROTO_TCP;
	pseudoheader->length = htons(sizeof(struct tcphdr));
	pseudoheader->daddr = dst_ip;
	srand((unsigned)time(NULL));

}

//create send packet
void SendData::buildTcpPacket(IPMAC srcip, IPMAC dstip, vector<short> ports, vector<SendPacket>& packets)
{
	int  i;
	struct iphdr ip;		//IP头部
	struct tcphdr tcp;		//TCP头部
	struct pseudohdr pseudoheader;	//TCP伪头部



	for (i = 0; i < ports.size(); i++)
	{
		char data[100] = { 0 };
		ip.sourceIP = srcip.ip;
		initHeader(&ip, &tcp, &pseudoheader, dstip.ip, ports[i]);
		memcpy(data, &ip, sizeof(struct iphdr));
		ip.checksum = checksum((u_short *)data, sizeof(struct iphdr));

		pseudoheader.saddr = ip.sourceIP;

		//计算TCP校验和
		memset(data, 0, sizeof(data));
		memcpy(data, &pseudoheader, sizeof(pseudoheader));
		memcpy(data + sizeof(pseudoheader), &tcp, sizeof(struct tcphdr));
		tcp.sum = checksum((u_short *)data, sizeof(pseudoheader) + sizeof(struct tcphdr));

		memset(data, 0, sizeof(data));
		unsigned short ethlen = htons(ETH_P_IP);
		memcpy(data, dstip.mac, ETH_ALEN);
		memcpy(data + ETH_ALEN, srcip.mac, ETH_ALEN);
		memcpy(data + ETH_ALEN * 2, &ethlen, sizeof(short));
		memcpy(data + sizeof(struct ethhdr), &ip, sizeof(struct iphdr));
		memcpy(data + sizeof(struct ethhdr) + sizeof(struct iphdr), &tcp, sizeof(struct tcphdr));

		SendPacket senddata(data, sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct tcphdr));
		packets.push_back(senddata);
	}

}

void SendData::sendData(pcap_t * adhandle, vector<SendPacket> packets)
{
	int i;

	for (i = 0; i < packets.size(); i++)
	{
		if (pcap_sendpacket(adhandle, (const u_char *)packets[i].getPacket(), packets[i].getPacketSize()) != 0)
		{
			cout << "send error: " << string(pcap_geterr(adhandle)) << endl;
		}
	}
}

void SendData::run()
{
	Poco::Random rnd;
	for (;;)
	{
		Notification::Ptr pNf(_queue.waitDequeueNotification());
		if (pNf)
		{
			ScanNotification::Ptr pWorkNf = pNf.cast<ScanNotification>();
			if (pWorkNf)
			{
				{
					FastMutex::ScopedLock lock(_mutex);
					std::cout << _name << " got work notification " << pWorkNf->data() << std::endl;
					vector<SendPacket> packets;
					buildTcpPacket(_srcIp, _dstIp, _ports, packets);
					sendData(_adhandle, packets);
				}
				Thread::sleep(rnd.next(200));
				std::cout << _name << " worker " << std::endl;
			}
		}
		else
		{
			std::cout << _name << " break " << std::endl;
			break;
		}		
	}
}

void ReceiveData::run()
{
	int start = GetTickCount();
	struct pcap_pkthdr * header;
	const u_char * pkt_data;
	int secondsWait = 10;

	while (true)
	{
		if (GetTickCount() - start > secondsWait * 1000)
		{
			cout << "listen time out";
			break;
		}


		//循环解析数据包
		if (pcap_next_ex(_adhandle, &header, &pkt_data) == 0){
			continue;
		}

		analyzePacket(pkt_data, header->len, _ips, _ports, _outIps);

	}
}

vector<SCANRESULT> ReceiveData::getScanResult()
{
	return _outIps;
}
