#include <stdio.h>
#include <string.h>

typedef unsigned long long LL;

int is_multi_ok(int a, int b) {
    LL c = a * (LL) b;
    return c == (int) c;
}

int main()
{
    int a, b;

    while (scanf("%d%d", &a, &b) == 2) {
        printf(!is_multi_ok(a, b) ? "yichu\n" : "not yichu\n");
    }
    return 0;
}
