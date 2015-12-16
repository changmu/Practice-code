#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

typedef int (*pp)[10];

// int (*f(int a))[10] {
auto f(int a) -> int(*)[10] {
    pp arr = (pp)malloc(sizeof(int) * 10);
    // pp arr = new int[10];
    for (int i = 0; i < 10; ++i)
        (*arr)[i] = i * a;
    return arr;
}

int main()
{
    pp a = f(2);
    for (int i = 0; i < 10; ++i)
        printf("%d\t", (*a)[i]);
    return 0;
}
