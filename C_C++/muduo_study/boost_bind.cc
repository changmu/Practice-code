#include <iostream>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "myLinux.h"

using namespace std;

class Foo {
public:
        void memberFunc(double d, int i, int j) {
                cout << d << endl;
                cout << i << endl;
                cout << j << endl;
                cout << endl;
        }
};

int main(int argc, char *argv[])
{
        Foo foo;
        boost::function<void (int)> fp = boost::bind(&Foo::memberFunc, &foo, 0.5, _1, 10);
        fp(100);
        boost::function<void (int, int)> fp2 = boost::bind(&Foo::memberFunc, boost::ref(foo), 0.5, _1, _2);
        fp2(50, 100);
        return 0;
}