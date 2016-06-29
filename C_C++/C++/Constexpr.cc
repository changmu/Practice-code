#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

constexpr auto add(auto tar)
{
    decltype(tar) i = 0, result = 0;
    for ( ; i < tar; ++i)
        ++result;
    return result;
}

auto add2(auto tar)
{
    decltype(tar) i = 0, result = 0;
    for ( ; i < tar; ++i)
        ++result;
    return result;
}

int main()
{
    constexpr auto a = add(100000000);
    cout << a << endl;
    cout << add2(1000000000) << endl;
    return 0;
}
