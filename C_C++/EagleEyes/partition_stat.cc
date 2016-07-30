/* wipe compile Warning of get_current_dir_name() */
// #define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "partition_stat.h"

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

int g_numsOfPartitions;
partitions_t g_partitions[MAX_PARTITIONS_NUM];

static void getNumsOfPartitions()
{
    FILE *fp = popen("mount -l", "r");
    char buf[4096];
    char str[4096];
    int i = 0;

    /* /dev/mapper/vg_aone-lv_root on / type ext4 (rw) */
    while (fgets(buf, sizeof(buf), fp)) {
        if (strncmp(buf, "/dev/", 5))
            continue;

        sscanf(buf, "%s", str);
        g_partitions[i].name = str;
        sscanf(buf, "%*s%*s%s", str);
        if (!strncmp(str, "/boot", 5) || !strncmp(str, "/swap", 5))
            continue;
        g_partitions[i].dir = str;
        sscanf(buf, "%*s%*s%*s%*s%*s%s", str);
        if (strncmp(str, "(rw", 3))
            continue;

        ++i;
    }
    g_numsOfPartitions = i;

    pclose(fp);
}

static bool checkPartition(const char *dir) 
{
    static const char *kTestString = "The proper function of man is to live, not to exist.";
    static const int kStringLength = strlen(kTestString);
    static const char *kFileName = "partition_stat.writeTopartitionOK.tmp";

    bool flag = true;
    char buf[40] = { 0 };
    char fileName[4096];
    int ret;

    sprintf(fileName, "%s/%s", dir, kFileName);
    int fd = open(fileName, O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        flag = false;
        // printf("debug: %s\n", "could not open/create a file.");
        return flag;
    } else {
        int ret = write(fd, kTestString, kStringLength);
        if (ret == -1) {
            // printf("debug: %s\n", "could not write to file.");
            flag = false;
            close(fd);
            unlink(fileName);
            return flag;
        }
    }
    
    fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        flag = false;
        // printf("debug: %s\n", "could not open file.");
        return flag;
    } else {
        ret = read(fd, buf, kStringLength);
        if (ret != kStringLength || strncmp(kTestString, buf, kStringLength)) {
            // printf("debug: %s\n", "could not strncmp.");
            flag = false;
        }
    }

    // printf("debug: buf:[%s]\t[%d:%d]\n", buf, ret, kStringLength);
    close(fd);
    unlink(fileName);

    return flag;
}

static void getPartitionStat(const char *dir, partitions_t *part)
{
    struct statfs partition;
    int ret;

    /* get partition info. */
    ret = statfs(dir, &partition);
    // ret = statfs("/", &partition);
    if (ret == -1)
        ERR_EXIT("statfs");

    /* unit: 'KByte' */
    part->totalSize = ((unsigned long long) partition.f_blocks * partition.f_bsize) >> 10;
    part->freeSize = ((unsigned long long) partition.f_bfree * partition.f_bsize) >> 10;

    part->usedRate = 100.0 * (part->totalSize - part->freeSize) / part->totalSize;
    part->valid = checkPartition(part->dir.c_str());
}

void getDiskStat()
{
    if (g_numsOfPartitions == 0)
        getNumsOfPartitions();

    int i;
    for (i = 0; i < g_numsOfPartitions; ++i)
        getPartitionStat(g_partitions[i].dir.c_str(), &g_partitions[i]);
}

#if 0
int main(int argc, char *argv[])
{
    while (1) {
        getDiskStat();
        for (int i = 0; i < g_numsOfPartitions; ++i) {
            printf("partition info:\n");
            printf("name: %s\n", g_partitions[i].name.c_str());
            printf("mount on: %s\n", g_partitions[i].dir.c_str());
            printf("total size: %luKB\n", g_partitions[i].totalSize);
            printf("free size: %luKB\n", g_partitions[i].freeSize);
            printf("used: %lf%%\n", g_partitions[i].usedRate);
            printf("valid: %s\n", g_partitions[i].valid ? "yes" : "no");
        }
        sleep(2);
    }
    return 0;
}
#endif
