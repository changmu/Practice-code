#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <boost/type_index.hpp>

using namespace std;
using boost::typeindex::type_id_with_cvr;

template<typename T>
void f(T&& a) {
    cout << type_id_with_cvr<T>().pretty_name() << endl;
}

template<typename T>
void f2(T& v)
{
    cout << type_id_with_cvr<T>().pretty_name() << endl;
}

int main()
{
    int a = 10;
    volatile int& b = a;
    // cout << type_id_with_cvr<decltype(b)>().pretty_name() << endl;
    f(a);
    f(b);
    auto c = b;
    // f(forward<int&&>(c));
    cout << c << b << a << endl;
    return 0;
}
