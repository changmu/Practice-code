#pragma once
#include <string>

#define MAX_PARTITIONS_NUM 66

// KByte
typedef unsigned long partitionSize_t;

struct partitions_t {
    std::string name;
    std::string dir;
    partitionSize_t totalSize;
    partitionSize_t freeSize;
    double usedRate;
    bool valid; // read and write is OK
};
extern partitions_t g_partitions[MAX_PARTITIONS_NUM];
extern int g_numsOfPartitions;

void getDiskStat();
