#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int a = 1, b = 2;
    string s1 = "hello", s2 = "world";
    auto f = [s0 = move(s1), s1 = move(s2)](auto& v) {
        cout << s0 << s1;
        ++v;
    };
    f(a);
    cout << a << endl;
    return 0;
}
