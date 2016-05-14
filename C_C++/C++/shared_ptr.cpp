#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <cstdio>

using namespace std;

int main()
{
    shared_ptr<int> p3 = make_shared<int> (42);
    auto p4 = make_shared<string> (10, '9');
    auto p5 = make_shared<int> ();
    auto p6 = make_shared<vector<string>> ();
    cout << *p3 << '\t' << *p4 << '\t' << *p5 << endl;
    return 0;
}
