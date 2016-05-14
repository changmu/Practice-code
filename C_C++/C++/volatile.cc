#include <iostream>
#include <stdio.h>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

struct SR {
    int &a;
    int &aa;
    int &aaaa;
};

int main()
{
    int x = 1;
    int &rx = x;
    const int &crx = x;

    rx = 5;

    printf("x=%d\n", x);
    printf("rx=%d\n", rx);
    printf("crx=%d\n", crx);
    printf("&x=%p\n", &x);
    printf("&rx=%p\n", &rx);

    printf("%d..\n", sizeof(int &));
    printf("%d..\n", sizeof(struct SR));

    return 0;
}
