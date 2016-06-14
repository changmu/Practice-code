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
        // f();
    }
protected:
    virtual void print() = 0;
    void print2() {
        print();
    }
        /*
    {
        printf("Timer\n"); 
    }*/
private:
    void f()
    {
        while (1) {
            print();
            sleep(1);
        }
    }
    int hello_;
};

class C : private Timer {
public:
    C() {
        Timer::print2();
    }
    int i_;
    virtual void print() {
        printf("C.\n"); 
    }
};

int main()
{
    C c;
    return 0;
}
