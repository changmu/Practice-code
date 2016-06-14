#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

struct Test {
    Test(int a = 1) {
        printf("constructor.\n");
    }
/*
    Test() {
    }
*/
    void fun() {
        // printf("hello\n");
    }
};

int main()
{
    Test a(1);
    a.fun();
    Test b;
    b.fun();

    Test *c = new Test();
    c->fun();

    Test().fun();

    return 0;
}
