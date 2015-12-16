#include <stdio.h>
#include <string.h>

struct D {};

int main()
{
    struct D d1;
    struct D d2;

    printf("%d\n", sizeof(struct D));
    printf("%d..%0X\n", sizeof(d1), &d1);
    printf("%d..%0X\n", sizeof(d2), &d2);
    return 0;
}
