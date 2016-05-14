#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

template <typename T>
ostream &print(ostream &os, const T &t)
{
    return os << t << endl; 
}

template <typename T, typename... Args>
ostream &print(ostream &os, const T &t, const Args&... args)
{
    os << t << ", ";
    return print(os, args...);
}

int main()
{
    print(cout, "a", 1, 2, 3, "b", "end");
    return 0;
}
