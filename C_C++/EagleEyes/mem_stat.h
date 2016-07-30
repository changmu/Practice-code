#pragma once

typedef unsigned mem_t;

void getMemStat();

/* unit is 'KB'. */
extern mem_t g_memTotal;
/* g_memIdle = g_memFree + g_buffers + g_cached; */
extern mem_t g_memIdle;
extern mem_t g_swapTotal;
extern mem_t g_swapFree;

/* g_memUsedRate = (g_memTotal - g_memIdle) / g_memTotal * 100% */
extern double g_memUsedRate;
/* g_swapUsedRate = (g_swapTotal - g_swapFree) / g_swapTotal * 100% */
extern double g_swapUsedRate;
