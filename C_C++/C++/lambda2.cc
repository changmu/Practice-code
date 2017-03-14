#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int id = 0;
    auto func = [id] () mutable  {
        cout << ++id << endl;
    };
    func();
    func();
    func();
    return 0;
}
