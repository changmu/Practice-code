#include "myLinux.h"

int main(int argc, char *argv[])
{
    int input, output, temp;

    input = 1;
    __asm__ __volatile__(
            "movl $0, %%eax;\n\t"
            "movl %%eax, %1;\n\t"
            "movl %2, %%eax;\n\t"
            "movl %%eax, %0;\n\t"
            :"=m"(output), "=m"(temp)
            :"r"(input)
            :"eax"
            );
    printf("%d %d\n", temp, output);
    return 0;
}
