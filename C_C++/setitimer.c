#include "myLinux.h"

void sig_handler(int sig)
{
    printf("recv SIGALRM\n");
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, sig_handler);

    struct itimerval new_val;
    new_val.it_value.tv_sec = 3;
    new_val.it_value.tv_usec = 0;

    new_val.it_interval.tv_sec = 1;
    new_val.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &new_val, NULL);

    while (1)
        ;
    return 0;
}
