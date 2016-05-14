/*
#include <myLinux.h>

void fun(int num)
{
    printf("I am fun(int)\n");
    printf("num = %d\n", num);
}

int main(int argc, char **argv)
{
    struct sigaction sig;
    sig.sa_handler = fun;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;

    sigaction(SIGINT, &sig, NULL);

    while (1) {
        printf("*************************************************************\n");
        sleep(1);
    }

    return 0;
}
*/
#include <myLinux.h>

pid_t pid;

void fun_p(int s)
{
    static int i = 0;
    printf("I am parent: %d\n", i);
    i += 2;

    kill(pid, SIGUSR1);
    sleep(1);
}

void fun_c(int s)
{
    static int i = 1;
    printf("I am child: %d\n", i);
    i += 2;

    kill(pid, SIGUSR1);
    // sleep(1);
}

int main(int argc, char **argv)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if ((pid = fork()) != 0) {
        sa.sa_handler = fun_p;

        sigaction(SIGUSR1, &sa, NULL);
        while (1);

    } else {
        sa.sa_handler = fun_c;

        pid = getppid();

        sigaction(SIGUSR1, &sa, NULL);

        kill(pid, SIGUSR1);
        while (1);
    }
    return 0;
}
