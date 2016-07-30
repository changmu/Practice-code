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

#include "net_stat.h"

#include <string>

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

typedef unsigned long long ULL;

static struct netInterfaces {
    char name[22];
    ULL  recvBytes;
    ULL  recvPacks;
    ULL  sendBytes;
    ULL  sendPacks;
} netDevices_old[MAX_DEVICE_NUM],
  netDevices_new[MAX_DEVICE_NUM];

int g_numsOfNetcards;
struct netcard_t g_netcards[MAX_DEVICE_NUM]; // only physical netcards

static bool g_hasGotPhysicalNetcardNames;

static void getPhysicalNetcardNames()
{
    if (g_hasGotPhysicalNetcardNames)
        return;

    static const char *cmd = "ip link";
    FILE *fp = popen(cmd, "r");

    char line0[512], line1[512];
    char name[128];
    char buf[86];
    size_t i = 0;

    while (fgets(line0, sizeof(line0), fp)) {
        fgets(line1, sizeof(line1), fp);

        sscanf(line0, "%*s%s", name);
        if (strchr(name, '@')) // eg. vlan400@eth1
            continue;

        name[strlen(name) - 1] = '\0'; // wipe trail ':'
        strcpy(g_netcards[i].name, name);

        sscanf(line1, "%s", buf);
        if (strcmp(buf, "link/ether") != 0)
            continue;

        sscanf(line1, "%*s%s", g_netcards[i].mac);
        ++i;
    }
    g_numsOfNetcards = i;

    pclose(fp);
    g_hasGotPhysicalNetcardNames = true;
}

static bool isPhysicalNetcardName(const char *name)
{
    for (int i = 0; i < g_numsOfNetcards; ++i)
        if (!strcmp(name, g_netcards[i].name))
            return true;

    return false;
}

static void readStatFromFile(struct netInterfaces devices[])
{
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp)
        ERR_EXIT("fopen");

    char buf[1024] = { 0 };

    fgets(buf, sizeof(buf), fp);
    fgets(buf, sizeof(buf), fp);

    const char *scan_fmt = "%s%llu%llu%*d%*d%*d%*d%*d%*d%llu%llu";
    int i = 0;

    while (fgets(buf, sizeof(buf), fp)) {
        sscanf(buf, scan_fmt, devices[i].name,
            &devices[i].recvBytes, &devices[i].recvPacks,
            &devices[i].sendBytes, &devices[i].sendPacks);
        char *p = devices[i].name;
        p[strlen(p) - 1] = '\0'; //  wipe trail ':'

        if (!isPhysicalNetcardName(devices[i].name))
            continue;

        ++i;
    }

    fclose(fp);
}

static int getIdxOfNetcardByName(const char *name)
{
    for (int i = 0; i < g_numsOfNetcards; ++i)
        if (strcmp(name, g_netcards[i].name) == 0)
            return i;
    return -1;
}

static void calRate()
{
    int i, idx;
    memset(&g_netcards[g_numsOfNetcards], 0, sizeof(*g_netcards)); // the TOTAL netcard info.
    for (i = 0; i < g_numsOfNetcards; ++i) {
        idx = getIdxOfNetcardByName(netDevices_new[i].name);
        if (idx == -1)
            ERR_EXIT("getIdxOfNetcardByName");
        
        g_netcards[idx].recvMBitsPerSec = (netDevices_new[i].recvBytes
            - netDevices_old[i].recvBytes) * 8.0 / (1 << 20);  // mbps
        g_netcards[idx].sendMBitsPerSec = (netDevices_new[i].sendBytes
            - netDevices_old[i].sendBytes) * 8.0 / (1 << 20);
        g_netcards[idx].recvKpacksPerSec = (netDevices_new[i].recvPacks
            - netDevices_old[i].recvPacks) / (double) (1 << 10); // kps
        g_netcards[idx].sendKpacksPerSec = (netDevices_new[i].sendPacks
            - netDevices_old[i].sendPacks) / (double) (1 << 10); // kps

        g_netcards[g_numsOfNetcards].recvMBitsPerSec += g_netcards[idx].recvMBitsPerSec;
        g_netcards[g_numsOfNetcards].sendMBitsPerSec += g_netcards[idx].sendMBitsPerSec;
        g_netcards[g_numsOfNetcards].recvKpacksPerSec += g_netcards[idx].recvKpacksPerSec;
        g_netcards[g_numsOfNetcards].sendKpacksPerSec += g_netcards[idx].sendKpacksPerSec;
    }
}

static void addIpToNetcard()
{
    for (int i = 0; i < g_numsOfNetcards; ++i)
        g_netcards[i].ipInfo.clear();

    for (int i = 0; i < g_interfaceNums; ++i) {
        for (int j = 0; j < g_numsOfNetcards; ++j) {
            if (!strcmp(interfaceDevices[i].mac, g_netcards[j].mac)) {
                g_netcards[j].ipInfo.push_back(&interfaceDevices[i]);
                break;
            }
        }
    }
}

void getNetStat()
{
    getPhysicalNetcardNames();
    getIpStat();
    readStatFromFile(netDevices_old);
    sleep(1);
    readStatFromFile(netDevices_new);
    calRate();
    addIpToNetcard();
}

#if 0
int main(int argc, char *argv[])
{
    while (1) {
        getNetStat();
        int i;
        printf("net transfer:\n");
        for (i = 0; i < g_numsOfNetcards; ++i) {
            printf("device %d:\n", i);
            printf("\tname:%s\n", g_netcards[i].name);
            printf("\trecvMBitsPerSec:\t%lf mbps\n", g_netcards[i].recvMBitsPerSec);
            printf("\tsendMBitsPerSec:\t%lf mbps\n", g_netcards[i].sendMBitsPerSec);
            printf("\trecvKpacksPerSec:\t%lf kps\n", g_netcards[i].recvKpacksPerSec);
            printf("\tsendKpacksPerSec:\t%lf kps\n", g_netcards[i].sendKpacksPerSec);
        }
    }
    return 0;
}
#endif
