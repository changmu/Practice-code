#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

class test {
private:
    int data;
public:
    void f() {
        printf("%s", this == NULL ? "NULL\n" : "Hello\n");
    }
};

int main()
{
    test *p = NULL;
    p->f();
    return 0;
}
