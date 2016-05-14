#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>

#define MAX_PACKET_LEN 2000
typedef unsigned long long LL;

void convert_ip_readable(const u_char *arr) {
    int i = 0;

    printf("%d", arr[i++]);
    while (i < 4) {
        printf(".%d", arr[i++]);
    }
    puts("");
}

void print_hex(const u_char *arr, int len) {
    int i;
    
    printf("0x ");
    for (i = 0; i < len; ++i)
        printf("%02x ", arr[i]);
    puts("");
}

void reverse_bits(u_char *arr, int len) {
    u_char tmp;
    int i = 0, j = len - 1;

    while (i < j) {
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        ++i;
        --j;
    }
}

void qzg_deal(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    static int count = 1;

    const static int size_ethernet = 14;
    int size_ip;
    int size_tcp;

    LL des, sur;
    u_short ethernet_type;

    printf("\n\nNumber of packet: %d\n", count++);

    printf("---debug---\n");
    print_hex(packet, 14);
    printf("---debug---\n\n");

    printf("Destination of ethernet: ");
    print_hex(packet, 6);
    packet += 6;
    printf("Source of ethernet: ");
    print_hex(packet, 6);
    packet += 6;
    ethernet_type = *(u_short *) packet;

    /*
    printf("\n---debug---\n");
    print_hex((u_char *)&ethernet_type, 2);
    printf("sizeof it: %d\n", sizeof(ethernet_type));
    printf("Type of service %#4x.\n", (int) ethernet_type);
    printf("---debug---\n\n\n");
    */

    reverse_bits((u_char *) &ethernet_type, 2);
    printf("Type of Ethernet is ");
    switch (ethernet_type) {
        case 0x0800:
            puts("IPv4");
            break;
        case 0x86dd:
            puts("IPv6");
            break;
        case 0x0806:
            puts("ARP");
            break;
        case 0x8035:
            puts("RARP");
            break;
        default:
            puts("Other Network Layer Protocol");
    }
    packet += 2;

    u_char ip_ver = *packet >> 4;
    u_char ip_hl = *packet & 0x0f;
    u_char ip_proto;
    size_ip = ip_hl * 4;
    const u_char *TCP_header = packet + size_ip;
    printf("IP version is IPv%d.\n",  ip_ver);
    printf("IP header length is %d Bytes.\n", size_ip);

    packet += 2;
    u_short packet_len = *(u_short *) packet;
    reverse_bits((u_char *) &packet_len, 2);
    printf("The total packet length is %d Bytes.\n", packet_len);

    packet += 7;
    ip_proto = *(u_char *) packet;
    printf("Protocol type is:");
    switch (ip_proto) {
        case 0x06:
            puts("TCP");
            break;
        case 0x11:
            puts("UDP");
            break;
        default:
            puts("Other protocol");
    }

    packet += 3;
    printf("Source IP: ");
    // print_hex(packet, 4);
    convert_ip_readable(packet);
    packet += 4;
    printf("Destination IP: ");
    // print_hex(packet, 4);
    convert_ip_readable(packet);


    u_short dp, sp;
    packet = TCP_header;
    printf("Source port: ");
    sp = *(u_short *) packet;
    reverse_bits((u_char *) &sp, 2);
    printf("%d\n", sp);
    // print_hex(packet, 2);
    packet += 2;
    printf("Destination port: ");
    dp = *(u_short *) packet;
    reverse_bits((u_char *) &dp, 2);
    printf("%d\n", dp);
    // print_hex(packet, 2);
    packet += 8;
    size_tcp = (*packet >> 4) << 2;
    printf("Size of TCP header is %d Bytes.\n\n", size_tcp);
}

int main(int argc, char *argv[]) {
    char *dev = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    int num_packs = 20;

    if ((dev = pcap_lookupdev(errbuf)) == NULL) {
        printf("couldn't find device.\n");
        return 2;
    }

    printf("Device: %s\n", dev);
    printf("Number of packets: %d\n", num_packs);

    if ((handle = pcap_open_live(dev, MAX_PACKET_LEN, 1, 1000, errbuf)) == NULL) {
        printf("couldn't open device %s: %s\n", dev, errbuf);
        return 2;
    }

    pcap_loop(handle, num_packs, qzg_deal, NULL);

    pcap_close(handle);
    printf("Session complete.\n");

    return 0;
}
