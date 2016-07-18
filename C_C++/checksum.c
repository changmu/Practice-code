#include "myLinux.h"

struct ints {
    int a;
    int b;
    int c;
} t;

unsigned short f(unsigned char *arr, int count)
{
    int sum = 0;
    while (count > 1) {
        sum += *(unsigned short *) arr;
        arr += 2;
        count -= 2;
    }   

    if (count > 0)
        sum += *(unsigned short *) arr;

    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

int main(int argc, char *argv[])
{
    t.a = 1;
    t.b = 2;
    t.c = f(&t, sizeof(t));
    printf("%hu\n", t.c);
    printf("%hu\n", f(&t, sizeof(t)));
    return 0;
}
