
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_REMOTE

#include <winsock2.h>
#include <string>
#include "portscan.h"
#include "windowutils.h"
#include "netdefine.h"

//build tcp head and ip head
void PortScan::initHeader(struct iphdr *ip, struct tcphdr *tcp, struct pseudohdr *pseudoheader, long dst_ip, USHORT dst_port)
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
void PortScan::buildTcpPacket(IPMAC srcip, IPMAC dstip, vector<short> ports, vector<SendPacket>& packets)
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

void PortScan::sendData(pcap_t * adhandle, vector<SendPacket> packets)
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

void PortScan::analyzePacket(const u_char *pkt_data, u_short size)
{
	struct iphdr *ip = (struct iphdr *)(pkt_data + sizeof(struct ethhdr));
	struct tcphdr *tcp = (struct tcphdr *)(pkt_data + sizeof(struct ethhdr) + sizeof(struct iphdr));


	if ((htons(tcp->dport) == 34567) || (htons(tcp->sport) == 34567))
	{
		in_addr srcaddr;
		in_addr dstaddr;
		srcaddr.S_un.S_addr = ip->sourceIP;
		printf("ip src:");
		printf(inet_ntoa(srcaddr));
		printf("\n");
		dstaddr.S_un.S_addr = ip->destIP;
		printf("ip dst:");
		printf(inet_ntoa(dstaddr));
		printf("\n");

		char log[512] = { 0 };
		sprintf_s(log, " port src: %d, dst: %d, len: %d, flag: %02x\n",
			htons(tcp->sport), htons(tcp->dport), size, tcp->flag);
		cout << string(log) << endl;
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
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
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

	if ((m_adhandle = pcap_open_live(pcap_name.data(), 65536, 1, 1000, errbuf)) == NULL)
	{
		cout << "kevin : pcap_open_live failed!  not surpport by WinPcap ? alldev->name : %1";
		pcap_freealldevs(alldevs);
		return false;
	}

	if (pcap_datalink(m_adhandle) != DLT_EN10MB || alldevs->addresses == NULL) {
		cout << "kevin : pcap_datalink(adhandle) != DLT_EN10MB || alldevs->addresses == NULL";
		return false;
	}

	//set netmask
	netmask = ((struct sockaddr_in *)(alldevs->addresses->netmask))->sin_addr.S_un.S_addr;
	pcap_freealldevs(alldevs);
	
		
	//编译过滤器，只捕获tcp包
	char packet_filter[] = "ip and tcp";
	struct bpf_program fcode;

	if (pcap_compile(m_adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{
		cout << "unable to compile the packet filter.Check the syntax.";
		return false;
	}

	//设置过滤器
	if (pcap_setfilter(m_adhandle, &fcode) < 0)
	{
		cout << "Error setting the filter.";
		return false;
	}

	

	return true;
}

