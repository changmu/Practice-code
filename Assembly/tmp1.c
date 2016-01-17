#include "myLinux.h"

int main(int argc, char *argv[])
{
    unsigned val1 = 1;
    unsigned val2 = 2;
    unsigned val3 = 0;

    printf("val1:%d, val2:%d, val3:%d\n", val1, val2, val3);
    asm volatile (
            "movl $0, %%eax\n\t"
            "addl %1, %%eax\n\t"
            "addl %2, %%eax\n\t"
            "movl %%eax, %0\n\t"
            :"=m"(val3)
            :"c"(val1), "d"(val2)
            );
    printf("val1:%d + val2:%d = val3:%d\n", val1, val2, val3);

    return 0;
}
