#include <stdio.h>
#include <string.h>

void f(int b[10])
{
    printf("%d\n", sizeof(b));
}

int main()
{
    int a[10] = {0};
    printf("%d\n", sizeof(a));
    f(a);
    return 0;
}
