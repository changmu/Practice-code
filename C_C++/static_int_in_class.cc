#include <iostream>
#include <cstdio>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class C {
public:
        enum { c = 10 };
        static const int a = 12;
        int b[a];
};
const int C::a;

int main()
{
        C c;
        printf("a:%d\n", C::a);
        printf("c:%p\n", &c.c);
        cout << sizeof(C::a) << endl;
        cout << sizeof(C) << endl;
        return 0;
}
