#include <cstdio>
#include <utility>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Widget {
public:
    Widget() {
        cout << cnt++ << endl;
    }
    static int cnt;
};

int Widget::cnt = 1;

int main()
{
    vector<bool> v = { true, 1, 1, 0 };
    v[1] = 0;
    for (const auto& i : v)
        cout << i << endl;

    return 0;
}
