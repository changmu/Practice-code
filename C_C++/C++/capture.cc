#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <functional>

using namespace std;

function<void ()> gf;

void f()
{
    auto ptr = make_shared<int>(1);
    gf = [ptr]() {
        cout << ptr.use_count() << endl;
    };
    gf();
}

int main()
{
    f();
    gf();
    return 0;
}
