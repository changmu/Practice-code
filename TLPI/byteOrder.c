#include <myLinux.h>

int main()
{
    int a = 0x12345678;

    char *p = (char *) &a;
    printf("%0x %0x %0x %0x\n", p[0], p[1], p[2], p[3]);

    a = htonl(a);
    p = (char *) &a;
    printf("%0x %0x %0x %0x\n", p[0], p[1], p[2], p[3]);
    return 0;
}
