#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <deque>
#include <functional>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> vec{1, 2, 3, 4, 5};
    deque<int> deq1, deq2, deq3;
    copy(vec.cbegin(), vec.cend(), front_inserter(deq1));
    copy(vec.cbegin(), vec.cend(), back_inserter(deq2));
    copy(vec.cbegin(), vec.cend(), inserter(deq3, deq3.begin()));

    for (auto i : deq1) cout << i << '\t';
    cout << endl;
    for (auto i : deq2) cout << i << '\t';
    cout << endl;
    for (auto i : deq3) cout << i << '\t';
    cout << endl;
    return 0;
}
