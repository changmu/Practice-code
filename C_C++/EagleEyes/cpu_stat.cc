#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "cpu_stat.h"

typedef unsigned long cputime_t;

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)


/* 
 * these two global variables are for public usage. 
 */
int g_numsOfCores;
/* [0] is the TOTAL cpu, others are subcores, [0, 100]% */
double g_cpusUsageRate[MAX_NUMS_OF_CORES];

typedef struct cpu_stat_t {
    cputime_t busyTime;
    cputime_t totalTime;
} cpu_stat_t;
cpu_stat_t g_cpus_begin[MAX_NUMS_OF_CORES];
cpu_stat_t g_cpus_end[MAX_NUMS_OF_CORES];


static int getNumsOfCpuCores()
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp)
        ERR_EXIT("fopen");

    char buf[1024] = { 0 };
    int cnt = -1;

    while (fgets(buf, sizeof(buf), fp)) {
        if (strncmp(buf, "cpu", 3) != 0)
            break;
        ++cnt;
    }

    fclose(fp);

    return cnt;
}

/* store a cpu's busy time and total time */
static void readCpuStatFromString(const char *buf, cpu_stat_t *cpuTime_p)
{
    int num, n, i;
    /* skip the first string and calculate the skipped nums */
    sscanf(buf, "%*s%n", &n);
    num = n;

    cputime_t totalTime = 0;
    cputime_t idleTime = 0;
    cputime_t val;

    // extract the first 7 fields
    for (i = 0; i < 7; ++i) {
        sscanf(buf + num, "%lu%n", &val, &n);
        num += n;
        totalTime += val;
        if (i == 3) // this field is idle time
            idleTime = val;
    }

    cpuTime_p->totalTime = totalTime;
    cpuTime_p->busyTime = totalTime - idleTime;
}

/* store cpus' busy time and total time */
static void readCpuStatFromFile(cpu_stat_t *cpus)
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp)
        ERR_EXIT("fopen");

    char buf[1024] = { 0 };

    int i;
    for (i = 0; i <= g_numsOfCores; ++i) { // include the TOTAL Cpu
        fgets(buf, sizeof(buf), fp);
        /* get busy time and total time from a string */
        readCpuStatFromString(buf, &cpus[i]);
    }

    fclose(fp);
}

static void calCpuRate()
{
    int i;
    for (i = 0; i <= g_numsOfCores; ++i) {
        cpu_stat_t cpuTime_begin = g_cpus_begin[i];
        cpu_stat_t cpuTime_end = g_cpus_end[i];
        /* calculate the usage rate of cpu time */
        g_cpusUsageRate[i] = 100.0 * (double) (cpuTime_end.busyTime - 
            cpuTime_begin.busyTime) / (double) (cpuTime_end.totalTime - 
            cpuTime_begin.totalTime);
    }
}

void getCpuStat()
{
    // static const int kSleepMacroSeconds = 500000;

    if (g_numsOfCores == 0)
        g_numsOfCores = getNumsOfCpuCores();

    readCpuStatFromFile(g_cpus_begin);
    // usleep(kSleepMacroSeconds);
    sleep(1);
    readCpuStatFromFile(g_cpus_end);

    calCpuRate();
}


#if 0
int main(int argc, char *argv[])
{
    while (1) {
        getCpuStat();

        int i;
        printf("cpu TOTAL: %lf%%\n", g_cpusUsageRate[0]);
        for (i = 1; i <= g_numsOfCores; ++i)
            printf("cpu %d: %lf%%\n", i, g_cpusUsageRate[i]);
    }
    return 0;
}
#endif
