#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer buf, int sz)
{
    int i;
    
    printf("sz:%d\n", sz);
    printf("0x");
    for (i = 0; i < sz; ++i)
        printf("%.2x", buf[i]);
    puts("");
}

int main()
{
    int ix = 3510593;
    float fx = ix;

    show_bytes((byte_pointer)&ix, sizeof(ix));
    show_bytes((byte_pointer)&fx, sizeof(fx));

    return 0;
}
