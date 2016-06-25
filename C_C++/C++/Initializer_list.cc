#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int a = { 1 };
    int b{ 2 };
    int c{};
    int d(0);

    cout << a << b << endl;
    cout << c << d << endl;

    return 0;
}
