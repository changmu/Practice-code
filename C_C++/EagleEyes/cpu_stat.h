#pragma once

#define MAX_NUMS_OF_CORES 40

void getCpuStat();

extern int g_numsOfCores;
/* [0] is the TOTAL cpu, others are subcores, [0, 100]% */
extern double g_cpusUsageRate[MAX_NUMS_OF_CORES];