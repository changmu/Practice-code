#include "myLinux.h"

int main(int argc, char *argv[])
{
    if (is64Bits())
        printf("x64\n");
    else
        printf("x86\n");
    return 0;
}
