#include <boost/type_index.hpp>
#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;
using boost::typeindex::type_id_with_cvr;

class Abc {
    int a;
};

int main()
{
    Abc a;
    cout 
    << type_id_with_cvr<Abc>().pretty_name() << endl;
    return 0;
}
