#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class Timer {
public:
    Timer() {
        f();
    }
private:
    void f() {
        while (1) {
            printf("hello\n");
            sleep(2);
        }
    }
    int hello_;
};

class C : private Timer {
public:
    C() {}
    int i_;
};

int main()
{
    C c;
    return 0;
}
