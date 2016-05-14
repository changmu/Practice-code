#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>

using namespace std;

int add(int a, int b)
{
    return a + b;
}

auto sub = [](int a, int b) {return a - b;};

class A {
public:
    int operator()(int a, int b) const {
        return a * b;
    }
};

map<string, function<int(int, int)>> binops = {
    {"+", add},
    {"-", sub},
    {"*", A()},
    {"/", divides<int>()}
};

int main()
{
    cout << binops["/"](7, 3) << endl;
    return 0;
}
