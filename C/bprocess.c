#include "my_unix.h"

void myfun(int a)
{
    printf("Process B received signal.\n");
}

int main()
{
    signal(SIGINT, myfun);
    printf("PID: %d\n", getpid());
    pause();
    return 0;
}
