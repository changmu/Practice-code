#define DEBUG

#include <iostream>
#include <fstream>
#include <string>

#include "jsoncpp/json/json.h"
#include <curl/curl.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include "cpu_stat.h"
#include "mem_stat.h"
#include "partition_stat.h"
#include "net_stat.h"


static char buf[100000];

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

static const int g_kIntervalSeconds = 5 * 60;

volatile bool g_stop;



static void SIGUSR1_handle(int sig)
{
    g_stop = 1;
}

static void myNanoSleep(double seconds)
{
    time_t secs = (time_t) seconds;
    double fractional = seconds - secs;

    struct timespec ts;
    ts.tv_sec = secs;
    ts.tv_nsec = (long) (fractional * 1e9);

    int ret;
    do {
        ret = nanosleep(&ts, &ts);
    } while (ret == -1 && errno == EINTR);
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, SIGUSR1_handle);
#ifdef DEBUG
    FILE *fp = fopen("out.log", "w");
#endif
    daemon(0, 0);
    
    Json::FastWriter writer;
    Json::Value root;
    Json::Value val;
    Json::Value arr;
    Json::Value val2;

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    int i;
    char hostname[1024] = { 0 };
    gethostname(hostname, sizeof(hostname));
    char *cp = strchr(hostname, '.');
    if (cp != NULL)
        *cp = '\0';
    // printf("hostname:%s\n", hostname);

    while (!g_stop) {
        time_t beginInterval = time(NULL);

        root.clear();

        getNetStat(); // include getIpStat();
        getDiskStat();
        getCpuStat();
        getMemStat();

        // get hostname
        root["hostname"] = hostname;        

        
        // get cpu stat
        val.clear();
        arr.clear();
        val["total"] = g_cpusUsageRate[0];
        for (i = 1; i <= g_numsOfCores; ++i) {
            Json::Value subVal;
            subVal["idx"] = i - 1;
            subVal["usage"] = g_cpusUsageRate[i];
            arr.append(subVal);
        }
        val["per_cpu"] = arr;
        root["cpu"] = val;


        // get mem stat 'KB'
        val.clear();
        val["mem_total"] = g_memTotal;
        val["mem_idle"] = g_memIdle;
        val["swap_total"] = g_swapTotal;
        val["swap_idle"] = g_swapFree;
        val["mem_used_rate"] = g_memUsedRate;
        val["swap_used_rate"] = g_swapUsedRate;
        root["mem"] = val;


        // get disk stat 'MB'
        arr.clear();
        for (i = 0; i < g_numsOfPartitions; ++i) {
            val.clear();
            val["name"] = g_partitions[i].name;
            val["mount_dir"] = g_partitions[i].dir;
            val["total_size"] = g_partitions[i].totalSize;
            val["free_size"] = g_partitions[i].freeSize;
            val["used_rate"] = g_partitions[i].usedRate;
            val["rw_ok"] = g_partitions[i].valid;
            arr.append(val);
        }
        val.clear();
        val2.clear();
        // val["name"] = "total";
        val["total_size"] = g_partitions[i].totalSize;
        val["free_size"] = g_partitions[i].freeSize;
        val["used_rate"] = g_partitions[i].usedRate;
        val2["per_disk"] = arr;
        val2["total"] = val;
        root["disk"] = val2;

        // get net stat
        arr.clear();
        for (i = 0; i < g_numsOfNetcards; ++i) {
            val.clear();
            val["name"] = g_netcards[i].name;
            val["mac"] = g_netcards[i].mac;
            val["in_flow"] = g_netcards[i].recvMBitsPerSec;
            val["out_flow"] = g_netcards[i].sendMBitsPerSec;
            val["in_pack"] = g_netcards[i].recvKpacksPerSec;
            val["out_pack"] = g_netcards[i].sendKpacksPerSec;

            Json::Value subArr;
            for (size_t j = 0; j < g_netcards[i].ipInfo.size(); ++j) {
                Json::Value subVal;

                subVal["ipv4"] = g_netcards[i].ipInfo[j]->ip;
                subVal["valid"] = g_netcards[i].ipInfo[j]->valid;
                subArr.append(subVal);
            }
            val["ip"] = subArr;

            arr.append(val);
        }
        // total netcard
        val.clear();
        val["in_flow"] = g_netcards[i].recvMBitsPerSec;
        val["out_flow"] = g_netcards[i].sendMBitsPerSec;
        val["in_pack"] = g_netcards[i].recvKpacksPerSec;
        val["out_pack"] = g_netcards[i].sendKpacksPerSec;

        val2.clear();
        val2["total"] = val;     
        val2["per_netcard"] = arr;
        root["netcard"] = val2;

        time_t leftSeconds = g_kIntervalSeconds - (time(NULL) - beginInterval);

        if (leftSeconds > 0) {
            myNanoSleep(leftSeconds);
        }

        /* get a curl handle */ 
        curl = curl_easy_init();
        if(curl) {
            /* First set the URL that is about to receive our POST. This URL can
               just as well be a https:// URL if that is what should receive the
               data. */ 
            curl_easy_setopt(curl, CURLOPT_URL, "http://183.60.189.19/receiveStreamFile.php");
            /* Now specify the POST data */ 
            strncpy(buf, writer.write(root).c_str(), sizeof(buf) - 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

            /* always cleanup */ 
            curl_easy_cleanup(curl);
#ifdef DEBUG
            // std::cout << time(NULL) << std::endl << writer.write(root) << std::endl;

            fprintf(fp, "%s", buf);
            fflush(fp);
#endif
        }  
    }
    
    curl_global_cleanup();
#ifdef DEBUG
    fclose(fp);
#endif
    return 0;
}
