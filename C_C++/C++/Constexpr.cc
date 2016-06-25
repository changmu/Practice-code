#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

constexpr int pow(int base, int exp)
{
    // return exp == 0 ? 1 : base * pow(base, exp - 1);
    auto result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

int main()
{
    array<int, pow(2, 3)> a;
    // cout << pow(2, 20) << endl;
    return 0;
}
