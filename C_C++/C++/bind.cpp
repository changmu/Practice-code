#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <deque>
#include <functional>
#include <algorithm>

using namespace std;
using namespace std::placeholders;

int main()
{
    int sz = 1;
    auto f = [](int a, int b) -> bool { 
        if (a > b) cout << b << endl;
        return a > b;
    };

    deque<int> vec(5, 1);
    auto it = front_inserter(vec);
    for (int i = 2; i < 20; ++i)
        it = i * i;
    for (auto i : vec) cout << i << '\t';
    
    cout << endl << endl;
    for_each(vec.begin(), vec.end(), bind(f, 100, _1));
    return 0;
}
