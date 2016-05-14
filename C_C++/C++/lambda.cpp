#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sz = 0;
    int *p = find_if(a, a + 10, [sz](int a) { return a > sz; });
    cout << *p << endl;

    int sz = 1;
    auto f = [&]() { return ++sz; };
    cout << f() << endl;
    cout << f() << endl;
    return 0;
}
