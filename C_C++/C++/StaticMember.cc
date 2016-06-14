#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class C {
public:
    void f(char *str) {
        printf("%d\n", a); 
    }
private:
    static int a;
};

int C::a = 10;

int main()
{
    // printf("%d..\n", C::a); 
    C c;
    c.f("aaa");
    return 0;
}
