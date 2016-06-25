#include <boost/type_index.hpp>
#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;
using boost::typeindex::type_id_with_cvr;

template<typename T>
void f(T&& a) {
    cout << type_id_with_cvr<T>().pretty_name() << endl;
}

int main()
{
    return 0;
}
