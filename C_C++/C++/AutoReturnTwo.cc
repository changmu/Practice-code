#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

/*
auto f(int a)
{
    if (a > 0)
        return 1;
    else
        return 1.23;
}
*/

int main()
{
    auto f = [](const auto& a) {
        if (a > 0)
            return 1;
        else
            return 2;
    };

    cout << f(1) << endl;
    cout << f(-1) << endl;
    return 0;
}
