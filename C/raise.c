#include <myLinux.h>

void sig_capture(int num)
{
    printf("sig is %d\n", num);
}

int main(int argc, char *argv[])
{
    struct sigaction act;

    act.sa_handler = sig_capture;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaction(SIGUSR1, &act, NULL);
    // raise(atoi(argv[1]));

    while (1);

    return 0;
}
