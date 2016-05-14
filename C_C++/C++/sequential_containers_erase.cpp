#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

int ia[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 55, 89 };

int main()
{
    vector<int> iv(begin(ia), end(ia));
    list<int> il(begin(ia), end(ia));

    for (auto i = il.begin(); i != il.end(); ) {
        auto j = i++;
        if (*j & 1) il.erase(j);
        // if (*i & 0x1) il.erase(i);
        // else ++i;
    }
    for (auto i : il)
        cout << i << '\t';
    cout << endl;

    for (auto i = iv.begin(); i != iv.end(); ) {
        if (!(*i & 0x1)) iv.erase(i);
        else ++i;
    }
    for (auto i : iv)
        cout << i << '\t';
    cout << endl;
    return 0;
}
