#include<stdio.h>
#include "Include/pcap.h"
#include<winsock2.h>   
#include"capPack.h"

#pragma comment(lib,"Lib/wpcap.lib")   
#pragma comment(lib,"Lib/packet.lib")   
#pragma comment(lib,"ws2_32.lib")
#ifdef _MSC_VER
/*
* we do not want the warnings about the old deprecated and unsecure CRT functions
* since these examples can be compiled under *nix as well
*/
#define _CRT_SECURE_NO_WARNINGS
#endif
/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;			// Type of service 
	u_short tlen;			// Total length 
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;			// Time to live
	u_char	proto;			// Protocol
	u_short crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	u_int	op_pad;			// Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}udp_header;


/* 数据包处理函数声明 */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);



int main()
{
	pcap_if_t *alldevs=NULL;     //存放网卡设备
	pcap_if_t *d=NULL;			//显示网卡设备
	int snum=0;				//输入数字默认为0
	int i = 0;				//遍历计数器
	pcap_t *adhandle=NULL;		//当前设备
	char errbuf[PCAP_ERRBUF_SIZE];//错误码

	/*获取设备列表*/
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs:%s\n", errbuf);
		exit(1);
	}
	/*输出设备列表*/
	for (d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf("(No descrition available)\n");
	}
	if (i == 0)//没有找到设备
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	printf("Enter the interface number (1-%d):", i);
	scanf_s("%d", &snum);
	if(snum < 1 || snum > i)
	{
		printf("\n 输入有误.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	/* 转到选择的设备 */
	for (d = alldevs, i = 0; i< snum - 1; d = d->next, i++)
		;
	/* 打开设备 */
	if ((adhandle = pcap_open_live(d->name, //设备名    
		65536, // 捕捉完整的数据包    
		1, // 混在模式    
		1, // 读入超时    
		errbuf // 错误缓冲    
		)) == NULL)
	{
		printf("Unable to open the adapter");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("\nlistening on %s...\n", d->description);
	pcap_freealldevs(alldevs);		//释放设备列表
	pcap_loop(adhandle, 0, packet_handler, NULL);
	return 0;
}

/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	struct tm *ltime;
	char timestr[16];
	ip_header *ih;
	udp_header *uh;
	u_int ip_len;
	u_short sport, dport;
	time_t local_tv_sec;

	/*
	* unused parameter
	*/
	(VOID)(param);

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* print timestamp and length of the packet */
	printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

	/* retireve the position of the ip header */
	ih = (ip_header *)(pkt_data +
		14); //length of ethernet header

			 /* retireve the position of the udp header */
	ip_len = (ih->ver_ihl & 0xf) * 4;
	uh = (udp_header *)((u_char*)ih + ip_len);

	/* convert from network byte order to host byte order */
	sport = ntohs(uh->sport);
	dport = ntohs(uh->dport);

	/* print ip addresses and udp ports */
	printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
		ih->saddr.byte1,
		ih->saddr.byte2,
		ih->saddr.byte3,
		ih->saddr.byte4,
		sport,
		ih->daddr.byte1,
		ih->daddr.byte2,
		ih->daddr.byte3,
		ih->daddr.byte4,
		dport);
}
