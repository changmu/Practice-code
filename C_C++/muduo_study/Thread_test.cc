#include "Thread.h"
#include <unistd.h>


class Foo {
public:
        Foo(int count) : count_(count) {
        }

        void MemberFunction() {
                while (count_-- >= 0) {
                        cout << "This is a test ..." << endl;
                }
        }

        void MemberFunction2(int x) {
                while (count_-- >= 0) {
                        cout << "x = " << x << "This is a test2 ..." << endl;
                }
        }


        int count_;
};

void ThreadFunc()
{
        cout << "ThreadFunc ..." << endl;
}

void ThreadFunc2(int count)
{
        while (count--) {
                cout << "ThreadFunc ..." << endl;
                sleep(1);
        }
}

int main(void)
{
        Thread t1(ThreadFunc);
        Thread t2(boost::bind(ThreadFunc2, 3));

        Foo foo(3);
        Thread t3(boost::bind(&Foo::MemberFunction, &foo));
        Foo foo2(3);
        Thread t4(boost::bind(&Foo::MemberFunction2, &foo2, 5));

        t1.Start();
        t2.Start();
        t3.Start();
        t4.Start();

        t1.Join();
        t2.Join();
        t3.Join();
        t4.Join();
        return 0;
}
