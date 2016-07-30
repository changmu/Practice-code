#pragma once

#include <vector>
#include "ip_stat.h"

#define MAX_DEVICE_NUM 50

struct netcard_t {
    char name[52];
    char mac[22];
    double recvMBitsPerSec;
    double sendMBitsPerSec;
    double recvKpacksPerSec;
    double sendKpacksPerSec;

    std::vector<struct interfaceInfo *> ipInfo;
};

void getNetStat();
extern int g_numsOfNetcards;
extern struct netcard_t g_netcards[MAX_DEVICE_NUM];
