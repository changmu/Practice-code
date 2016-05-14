#include <stdio.h>
#include <string.h>

// Judge if two ints adding together is overflow
int is_overflow(int a, int b) {
    if ((a >> 31) && (b >> 31))
        return a + b > 0;
    else if (!((a >> 31) || (b >> 31)))
        return a + b < 0;
    return 1;
}
