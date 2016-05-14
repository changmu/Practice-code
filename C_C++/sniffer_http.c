/*
 * Program name: Sniffer_http
 * Author: QiuZhenggang
 * Complete date: 2015.7.22
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>

#define MAX_PACKET_LEN 2000
u_short g_packet_len;

/*
 * To find if the length of len in arr has substr 'HTTP'
 * */

int find_pwd(const u_char *arr) {
    char *str = malloc(g_packet_len + 1);

    strncpy(str, arr, g_packet_len);
    str[g_packet_len] = '\0';

    static const char *key_word[] = {
        "password",
        "userpass",
        "email",
        "username"
    };

    int i;
    int len = sizeof(key_word) / sizeof(*key_word);

    for (i = 0; i < len; ++i)
        if (strstr(str, key_word[i]))
            break;

    if (i != len)
        printf("\nPassword info:\n\t%s\n", str);

    return 1;
}

int is_next_HTTP(const u_char *arr, int len) {
    char *str = malloc(len + 1);

    strncpy(str, arr, len);
    str[len] = '\0';

    return strstr(str, "HTTP") != NULL;
}

void convert_ip_readable(const u_char *arr) {
    int i = 0;

    printf("%d", arr[i++]);
    while (i < 4) {
        printf(".%d", arr[i++]);
    }
    puts("");
}

void print_hex(const u_char *arr, int len, char ch) {
    int i = 0;

    printf("%02x", arr[i++]);
    for ( ; i < len; ++i)
        printf("%c%02x", ch, arr[i]);
    puts("");
}

void reverse_bits(u_char *arr, int len) {
    u_char tmp;
    int i = 0, j = len - 1;

    for ( ; i < j; ++i, --j) {
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void extract_name(const u_char **packet, const u_char *dns_header, const u_char *pkt_new, int layer) {
    if (*pkt_new == 0xc0) {
        if (layer == 0) {
            *packet = pkt_new + 2;
        }
        pkt_new = dns_header + *(pkt_new + 1);
        extract_name(packet, dns_header, pkt_new, ++layer);
        return;
    }

    u_char cnt = *pkt_new++;
    int i;

    while (cnt) {
        while (cnt--)
            putchar(*pkt_new++);
        cnt = *pkt_new++;

        if (cnt) putchar('.');
        if (cnt == 0xc0) {
            if (0 == layer) *packet = pkt_new + 1;
            pkt_new = dns_header + *pkt_new;
            extract_name(packet, dns_header, pkt_new, ++layer);
            return;
        }
    }
    puts("");

    if (0 == layer) *packet = pkt_new;
}

int deal_ethernet_header(const u_char **packet) {
    const static int size_ethernet = 14;
    u_short ether_proto;

    puts("Ethernet header:");

    printf("\tDestination: ");
    print_hex(*packet, 6, ':');
    *packet += 6;   // Destination(6B)

    printf("\tSource: ");
    print_hex(*packet, 6, ':');
    *packet += 6;   // Source(6B)

    ether_proto = *(u_short *) *packet;
    reverse_bits((u_char *) &ether_proto, 2);
    printf("\tProtocol: ");
    switch (ether_proto) {
        case 0x0800:
            puts("IPv4");
            break;
        case 0x86dd:
            puts("IPv6");
            return 0;
        case 0x0806:
            puts("ARP");
            return 0;
        case 0x8035:
            puts("RARP");
            return 0;
        default:
            puts("Other Protocol");
            return 0;
    }
    *packet += 2;   // Type(2B)

    return 1;
}

int deal_IP_header(const u_char **packet) {
    int size_ip;
    const u_char *TCP_header;
    u_char ip_proto;

    u_char ip_ver = **packet >> 4;
    u_char ip_hl = **packet & 0x0f;

    printf("\nIP header:\n");

    size_ip = ip_hl * 4;
    TCP_header = *packet + size_ip;
    printf("\tVersion: IPv%d.\n",  ip_ver);
    printf("\tHeader length: %d Bytes.\n", size_ip);
    if (size_ip < 20) {
        puts("\t*** Bad packet ***");
        return 0;
    }
    *packet += 2;   // Version + HL + Service(2B)

    g_packet_len = *(u_short *) *packet;
    reverse_bits((u_char *) &g_packet_len, 2);
    printf("\tTotal length: %d Bytes.\n", g_packet_len);
    *packet += 7;   // Total Length + Iden. + Flag + Frag + TTL(7B)

    ip_proto = **packet;
    printf("\tProtocol: ");
    switch (ip_proto) {
        case 0x06:
            puts("TCP");
            break;
        case 0x11:
            puts("UDP");
            return 0;
        default:
            puts("Other protocol");
            return 0;
    }
    *packet += 3;   // Protocol + Checksum(3B)

    printf("\tSource IP: ");
    convert_ip_readable(*packet);
    *packet += 4;   // Source Ip

    printf("\tDestination IP: ");
    convert_ip_readable(*packet);
    *packet = TCP_header;   // Calculated before
    g_packet_len -= size_ip;

    return 1;
}

int deal_TCP_header(const u_char **packet) {
    u_short sp, dp;
    u_char hl;
    const u_char *pkt_old = *packet;
/*
    printf("===debug===\n");
    print_hex(pkt_old, 40, ' ');
    printf("===debug===\n");
*/
    printf("\nTCP header:\n");

    sp = *(u_short *) *packet;
    reverse_bits((u_char *) &sp, 2);
    printf("\tSource port: %d\n", sp);
    *packet += 2;   // Source port

    dp = *(u_short *) *packet;
    reverse_bits((u_char *) &dp, 2);
    printf("\tDestination port: %d\n", dp);
    *packet += 10;  // Des + Seq + Ack

    hl = **packet >> 4 << 2;
    printf("\tHeader length: %d Bytes\n", hl);

    if (80 != sp && 80 != dp)
        return 0;

    *packet = pkt_old + hl;
    g_packet_len -= hl;

    if (!is_next_HTTP(*packet, g_packet_len))
        return 0;

    return 1;
}

int deal_HTTP_header(const u_char **packet) {
    printf("\nHTTP header:\n");

    const u_char *pkt_new = *packet;

    while (*pkt_new != '\r' || *(pkt_new + 1) != '\n') {
        putchar('\t');
        while (*pkt_new != '\r' || *(pkt_new + 1) != '\n') {
            putchar(*pkt_new++);
            --g_packet_len;
        }
        putchar('\n');
        pkt_new += 2;
        g_packet_len -= 2;
    }
    g_packet_len -= 2;

    *packet = pkt_new + 2;
    if (g_packet_len > 0)
        find_pwd(*packet);

    return 1;
}

int deal_UDP_header(const u_char **packet) {
    int size_udp;
    u_short dp, sp;

    printf("\nUDP header:\n");

    printf("\tSource port: ");
    sp = *(u_short *) *packet;
    reverse_bits((u_char *) &sp, 2);
    printf("%d\n", sp);
    *packet += 2;   // Source port

    printf("\tDestination port: ");
    dp = *(u_short *) *packet;
    reverse_bits((u_char *) &dp, 2);
    printf("%d\n", dp);
    *packet += 2;   // Destination port

    size_udp = *(u_short *) *packet;
    reverse_bits((u_char *) &size_udp, 2);
    printf("\tLength: %d Bytes.\n", size_udp);

    if (sp != 53 && dp != 53)   // DNS is at port 53
        return 0;
    *packet += 4;   // Length + Checksum

    return 1;
}

int deal_DNS_header(const u_char **packet) {
    const u_char *dns_header = *packet;  // Used for offset
    int i;

    printf("\nDNS header:\n");

    printf("\tTransaction ID: 0x ");
    print_hex(*packet, 2, ' ');
    *packet += 2;   // Transaction ID

    printf("\tResponse: Message is a %s\n", (**packet >> 7) ? "RESPONSE" : "QUERY");
    *packet += 2;   // Flag

    u_short dns_questions = *(u_short *) *packet;
    reverse_bits((u_char *) &dns_questions, 2);
    printf("\tQuestions: %d\n", dns_questions);
    *packet += 2;   // Questions

    u_short dns_answers = *(u_short *) *packet;
    reverse_bits((u_char *) &dns_answers, 2);
    printf("\tAnswers: %d\n", dns_answers);
    *packet += 6;   // Answers

    if (dns_questions) {
        printf("\tQueries:\n");
        while (dns_questions--) {
            printf("\t\tName: ");
            extract_name(packet, dns_header, *packet, 0);
            *packet += 4;    // Type + Class
        }
    }

    int ans_type;
    if (dns_answers) {
        puts("\tAnswers:");
        for (i = 1; i <= dns_answers; ++i) {
            printf("\t\tans %d:\n", i);

            printf("\t\t\tName: ");
            extract_name(packet, dns_header, *packet, 0);

            printf("\t\t\tType: ");
            ans_type =  *(u_short *) *packet;
            switch (ans_type) {
                case 0x0500:    // Little endian
                    puts("CNAME");
                    break;
                case 0x0100:    // Little endian too
                    puts("A");
                    break;
                default:
                    puts("Other type");
                    return 0;
            }
            *packet += 10;      // type + class + TTL + DL

            if (ans_type == 0x0500) {
                printf("\t\t\tPrimaryname: ");
                extract_name(packet, dns_header, *packet, 0);
            } else {
                printf("\t\t\tAddr: ");
                convert_ip_readable(*packet);
                *packet += 4;   // Addr
            }
        }
    }

    return 1; 
}

void deal_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    static int count = 1;
    int i;
    typedef int (*FUNC[4])(const u_char **);
    FUNC *func = (FUNC *) args;

    printf("\n\n============================= Number of packet: %d =============================\n", count++);
    for (i = 0; i < 4; ++i)
        if (0 == (*func)[i](&packet))
            return;
}

int main(int argc, char *argv[]) {
    char *dev = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    char filter_exp[] = "tcp port 80";
    bpf_u_int32 net;    // Ip
    bpf_u_int32 mask;   // Netmask
    pcap_t *handle;
    int num_packs = 200;

    int (*fun_menu[4])(const u_char **) = {
        deal_ethernet_header,
        deal_IP_header,
        deal_TCP_header,
        deal_HTTP_header
    };

    if (argc > 2) {
        puts("Usage: too many args, only one needed.");
        return 2;
    } else if (2 == argc) {
        dev = argv[1];
    } else {
        if ((dev = pcap_lookupdev(errbuf)) == NULL) {
            printf("Couldn't open device: \n", errbuf);
            return 2;
        }
    }

    printf("Device: %s\n", dev);
    printf("Number of packets to capture: %d\n", num_packs);

    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        printf("couldn't get netmask for device: %s\n", errbuf);
        net = 0;
        mask = 0;
    }

    if ((handle = pcap_open_live(dev, MAX_PACKET_LEN, 1, 1000, errbuf)) == NULL) {
        printf("couldn't open device %s: %s\n", dev, errbuf);
        return 2;
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        printf("couldn't parse filter.\n");
        return 2;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        printf("couldn't install filter.\n");
        return 2;
    }

    pcap_loop(handle, num_packs, deal_packet, (u_char *) &fun_menu);

    pcap_close(handle);
    printf("Session complete.\n");

    return 0;
}
