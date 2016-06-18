// -std=c++14

#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

auto add(int a, int b) // c++14
{
    return a + b;
}

int main()
{
    cout << add(1, 2) << endl;

    int v;
    auto resetV = [&v](const auto& newValue) { v = newValue; }; // c++14

    resetV(10);
    cout << v << endl;
    resetV(12.3);
    cout << v << endl;
    return 0;
}
