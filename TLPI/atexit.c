#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bye()
{
    printf("Bye\n");
}

void bye2()
{
    printf("Bye2\n");
}

void f()
{
    exit(1);
}

int main()
{
    atexit(bye);
    atexit(bye2);
    printf("hello\n");
    exit(0);
    // return 0;
}
