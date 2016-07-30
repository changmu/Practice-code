#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h> // for strncasecmp
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mem_stat.h"


#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)


/*
 * for public usage.
 * unit is 'KB'.
 * NOTE: Only after getMemStat() was called can you use them.
 */
mem_t g_memTotal;
/* g_memIdle = g_memFree + g_buffers + g_cached; */
mem_t g_memIdle;
mem_t g_swapTotal;
mem_t g_swapFree;

/* g_memUsedRate = (g_memTotal - g_memIdle) / g_memTotal * 100% */
double g_memUsedRate;
/* g_swapUsedRate = (g_swapTotal - g_swapFree) / g_swapTotal * 100% */
double g_swapUsedRate;


static mem_t g_memFree;
static mem_t g_buffers;
static mem_t g_cached;


void getMemStat()
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp)
        ERR_EXIT("fopen");

    char buf[1024] = { 0 };

    while (fgets(buf, sizeof(buf), fp)) {
        if (!strncasecmp(buf, "MemTotal", 8)) {
            sscanf(buf, "%*s%u", &g_memTotal);
        } else if (!strncasecmp(buf, "MemFree", 7)) {
            sscanf(buf, "%*s%u", &g_memFree);
        } else if (!strncasecmp(buf, "SwapTotal", 9)) {
            sscanf(buf, "%*s%u", &g_swapTotal);
        } else if (!strncasecmp(buf, "SwapFree", 8)) {
            sscanf(buf, "%*s%u", &g_swapFree);
        } else if (!strncasecmp(buf, "Buffers", 7)) {
            sscanf(buf, "%*s%u", &g_buffers);
        } else if (!strncasecmp(buf, "Cached", 6)) {
            sscanf(buf, "%*s%u", &g_cached);
        }
    }

    g_memIdle = g_memFree + g_buffers + g_cached;
    g_memUsedRate = 100.0 * (g_memTotal - g_memIdle) / (double) g_memTotal;
    g_swapUsedRate = 100.0 * (g_swapTotal - g_swapFree) / (double) g_swapTotal;

    fclose(fp);
}

#if 0
int main(int argc, char *argv[])
{
    while (1) {        
        getMemStat();
        
        puts("Memory stat:");
        printf("g_memTotal: %u KB\n", g_memTotal);
        printf("g_memFree: %u KB\n", g_memFree);
        printf("g_swapTotal: %u KB\n", g_swapTotal);
        printf("g_swapFree: %u KB\n", g_swapFree);
        printf("g_memUsedRate: %lf%%\n", g_memUsedRate);
        printf("g_swapUsedRate: %lf%%\n", g_swapUsedRate);
        sleep(1);
    }

    return 0;
}
#endif
