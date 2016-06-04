#include <iostream>
#include <cstdio>

class C {
public:
        C(): count(123) {}
        void f();
        static void f2();
private:
        int count;
};

void C::f() {
        printf("count:f() = %d\n", count);
}

void C::f2() {
        printf("count:f2() = %d\n", this->count);
}

int main(int argc, char *argv[])
{
        C a;
        a.f();
        a.f2();
        return 0;
}