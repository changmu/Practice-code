#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

int main()
{
    unsigned a = 0xffffffff;
    cout << a << endl;
    a >>= 1;
    cout << a << endl;
    return 0;
}
