#include <iostream>
#include <cstdio>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class Noncopyable {
protected:
        Noncopyable()
        {
        }
private:
        Noncopyable(const Noncopyable&);
        Noncopyable& operator=(const Noncopyable&);
};

class C : public Noncopyable { 
public:
        C(int a, int b, int& c)
                : a_(a), b_(b), c_(c)
        {
        }


        int a_;
        int b_;
        int c_;
// private:
        // C& operator=(const C& rhs);
};

int main()
{
        int i = 12;

        C a(2, 3, i);
        C b(a);
        a = b;
        printf("C::c:%d\n", a.c_); 
        return 0;
}
