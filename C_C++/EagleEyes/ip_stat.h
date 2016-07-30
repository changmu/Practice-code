#pragma once

#include <arpa/inet.h> // for INET_ADDRSTRLEN

#define MAX_INTERFACE_NUM 5000

void getIpStat();
void getDeviceMac(const char *deviceName, char mac[]);

struct interfaceInfo {
    // char name[22]; // device name
    char mac[22];
    char ip[INET_ADDRSTRLEN];
    bool valid;
};
extern struct interfaceInfo interfaceDevices[MAX_INTERFACE_NUM];

extern int g_interfaceNums;
