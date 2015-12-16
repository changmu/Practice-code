#include <stdio.h>
#include <string.h>

int main()
{
    struct Node {
        unsigned a1: 1;
        unsigned a2: 1;
        unsigned a3: 1;
        unsigned a4: 1;
        unsigned a5: 1;
        unsigned a6: 1;
        unsigned a7: 1;
        unsigned a8: 1;
    } a;
    int b, i;
    a.a1 = 0;
    a.a2 = 1;
    a.a3 = 0;
    a.a4 = 1;
    a.a5 = 0;
    a.a6 = 1;
    a.a7 = 0;
    a.a8 = 1;
    
    unsigned char *p = (char *) &a;
    for (i = 0; i < 8; ++i)
        printf((1 << i) & *p ? "1" : "0");
    return 0;
}
