#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

#include <boost/type_index.hpp>

using namespace std;
using boost::typeindex::type_id_with_cvr;

template<typename T>
void f(T&& a) {
    cout << type_id_with_cvr<T>().pretty_name() << endl;
}

int main()
{
    const int a = 10;

    f(a);
    return 0;
}
