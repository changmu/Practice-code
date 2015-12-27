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
