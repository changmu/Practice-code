#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class TD;

int main()
{
    int x;
    const int* const y = NULL;
    // TD<decltype(x)> xType;
    // TD<decltype(y)> yType;
    cout << typeid(x).name() << endl;
    cout << typeid(y).name() << endl;
    return 0;
}
