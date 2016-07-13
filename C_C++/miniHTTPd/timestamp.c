#include "timestamp.h"
#include <sys/time.h>
#include <time.h>

static const int kMicroSecondsPerSecond = 1000 * 1000;

void timestamp_toString(char *buf, size_t size, int64_t microSecondsSinceEpoch)
{
        int64_t seconds = microSecondsSinceEpoch / kMicroSecondsPerSecond;
        int64_t microseconds = microSecondsSinceEpoch % kMicroSecondsPerSecond;
        snprintf(buf, size, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
}


void timestamp_toFormattedString(char *buf, size_t size, int64_t microSecondsSinceEpoch, int showMicroseconds)
{

        time_t seconds = microSecondsSinceEpoch / kMicroSecondsPerSecond;
        struct tm tm_time;
        gmtime_r(&seconds, &tm_time);

        if (showMicroseconds) {
                int microseconds = microSecondsSinceEpoch % kMicroSecondsPerSecond;
                snprintf(buf, size, "%4d%02d%02d %02d:%02d:%02d.%06d",
                         tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                         tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, 
                         microseconds);
        } else {
                snprintf(buf, size, "%4d%02d%02d %02d:%02d:%02d",
                         tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                         tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        }
}


int64_t timestamp_now()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int64_t seconds = tv.tv_sec;
        return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}


double timestamp_difference(int64_t high, int64_t low)
{
        int64_t diff = high - low;
        return (double) diff / kMicroSecondsPerSecond;
}
