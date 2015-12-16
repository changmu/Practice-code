#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

int main()
{
    time_t ctime = time(NULL);
    struct tm *tmp = localtime(&ctime);
    printf("Now is %d:%d:%d.\n", tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
    puts(asctime(tmp));
    struct timeval tmvp;
    gettimeofday(&tmvp, NULL);
    printf("%d\t%lld\n", tmvp.tv_sec, tmvp.tv_usec);
    return 0;
}
