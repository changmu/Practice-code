#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

#include "ip_stat.h"

#include "threadpool/threadpool.h"

#include <string>
#include <map>
#include <cctype>

// prototype:
// int getifaddrs(struct ifaddrs **ifap);
// void freeifaddrs(struct ifaddrs *ifa);

#define THREAD 20
#define QUEUE  1000

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

struct interfaceInfo interfaceDevices[MAX_INTERFACE_NUM];

int g_interfaceNums;

std::map<std::string, std::string> nameToMacCache;

void getDeviceMac(const char *deviceName, char mac[])
{
    std::string str = nameToMacCache[deviceName];
    if (!str.empty()) {
        strcpy(mac, str.c_str());
        return;
    }

    struct ifreq ifreq;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        ERR_EXIT("getDeviceMac:socket");
    }

    strcpy(ifreq.ifr_name, deviceName);
    if(ioctl(sock, SIOCGIFHWADDR, &ifreq) < 0) {
        ERR_EXIT("getDeviceMac::ioctl");
    }

    int i = 0;
    for(i = 0; i < 6; i++) {
        sprintf(mac + 3 * i, "%02X:", (unsigned char)ifreq.ifr_hwaddr.sa_data[i]);
    }
    int len = strlen(mac);
    mac[--len] = 0;
    for (i = 0; i < len; ++i)
        mac[i] = tolower(mac[i]);

    nameToMacCache[deviceName] = mac;
}


static bool isPrivateIp(const char *ip)
{
    static const unsigned A_private = 0x0a000000;
    static const unsigned A_reserve = 0x7f000000;
    static const unsigned B_private = 0xac100000;
    static const unsigned B_reserve = 0xa9fe0000;
    static const unsigned C_private = 0xc0a80000;

    unsigned addr = ntohl((unsigned) inet_addr(ip));

    return (addr >> 24) == (A_private >> 24)
        || (addr >> 24) == (A_reserve >> 24)
        || (addr >> 20) == (B_private >> 20)
        || (addr >> 16) == (B_reserve >> 16)
        || (addr >> 16) == (C_private >> 16);
}


// 114.114.114.114, 8.8.8.8
// check whether a ip could connect net, timeout is -w 1s
static bool checkConnect(const char *ip)
{
    char cmd[102] = { 0 };
    sprintf(cmd, "ping 114.114.114.114 -l 3 -i 1 -w 1 -I %s 2>&1", ip);
    // sprintf(cmd, "ping 114.114.114.114 -i 0.2 -W 0.1 -c 10 -I %s 2>&1", ip);
    FILE *fp = popen(cmd, "r");
    bool flag = 0;

    char buf[102] = { 0 };
    while (fgets(buf, sizeof(buf), fp)) {
        // printf("debug: %s\n", buf);
        if (strstr(buf, "bytes of data."))
            continue;

        if (strstr(buf, "bytes from")) {
            flag = 1;
        } else {
            flag = 0;
        }
        break;
    }
    pclose(fp);

    return flag;
}

static void thread_task(void *arg)
{
    int i = *(int *) arg; // index of interfaceDevices
    free(arg);
    const char *ip = interfaceDevices[i].ip;
    interfaceDevices[i].valid = checkConnect(ip);
}

void getIpStat()
{
    threadpool_t *pool = threadpool_create(THREAD, QUEUE, 0);

    if (pool == NULL)
        ERR_EXIT("threadpool_create");
    
    struct ifaddrs *ifaStructPtr = NULL;
    struct ifaddrs *ifaPtr = NULL;
    struct in_addr addr;

    int ret = getifaddrs(&ifaStructPtr);
    if (ret == -1)
        ERR_EXIT("getifaddrs");

    char ip[INET_ADDRSTRLEN + 10] = { 0 };

    int i = 0;
    for (ifaPtr = ifaStructPtr; ifaPtr != NULL; ifaPtr = ifaPtr->ifa_next) {
        if (!ifaPtr->ifa_addr || ifaPtr->ifa_addr->sa_family != AF_INET)
            continue;

        addr = ((struct sockaddr_in *) ifaPtr->ifa_addr)->sin_addr;
        strcpy(ip, inet_ntoa(addr));

        // strcpy(interfaceDevices[i].name, ifaPtr->ifa_name);
        strcpy(interfaceDevices[i].ip, ip);

        getDeviceMac(ifaPtr->ifa_name, interfaceDevices[i].mac);
#if 1
        // filter inner ip
        if (isPrivateIp(ip)) {
            // printf("debug: ip %s\n", ip);
            continue;
        }
#endif
        //! interfaceDevices[i].valid = checkConnect(ip);
        
        // printf("debug: name: %s ip:%s\n", ifaPtr->ifa_name, ip);
        int *arg = (int *) malloc(sizeof(int));
        *arg = i;
        threadpool_add(pool, thread_task, arg, 0);

        ++i;
    }
    g_interfaceNums = i;

    freeifaddrs(ifaStructPtr);
    threadpool_destroy(pool, threadpool_graceful);
}

#if 0
int main(int argc, char *argv[])
{
    getIpStat();
    int i;
    char mac[32];
    printf("interface:\n");
    for (i = 0; i < g_interfaceNums; ++i) {
        printf("device name: %s\n", interfaceDevices[i].name);
        printf("MAC: %s\n", mac);
        printf("\tip:%s\n", interfaceDevices[i].ip);
        printf("\tvalid:%d\n", interfaceDevices[i].valid);
    }
    return 0;
}
#endif
