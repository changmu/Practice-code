#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class A {
public:
    virtual ~A() = 0;
};

A::~A() 
{
}

class B : A {
public:
    B() {
        printf("hello\n"); 
    }
};

int main()
{
    B a;
    return 0;
}
