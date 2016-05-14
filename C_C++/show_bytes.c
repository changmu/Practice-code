#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer arr, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        printf("%.2x ", arr[i]);
    puts("");
}

int main()
{
    int b = 0x12345678, i;
    double a = 3.1415926;
    show_bytes((byte_pointer) &a, sizeof(a));
    return 0;
}
