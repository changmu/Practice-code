#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class A {
public:
    A(): v(10) {}
    A(const A &B) = delete;
private:
    int v;
};

int main()
{
    A a;
    return 0;
}
