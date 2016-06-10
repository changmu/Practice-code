#include <iostream>
#include <cstdio>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class C {
public:
        C(int a, int b, int& c)
                : a_(a), b_(b), c_(c)
        {
        }

        C& operator=(const C& rhs) {
                return *this;
        }

        int a_;
        int b_;
        int& c_;
};

int main()
{
        int i = 12;

        C a(2, 3, i);
        printf("C::c:%d\n", a.c_); 
        return 0;
}
