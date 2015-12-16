#include <iostream>
#include <fstream>
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

    ifstream fin("test.txt");
    istream_iterator<string> in_iter(fin), eof;
    ostream_iterator<string> out_iter(cout, " ");
    vector<string> svec(in_iter, eof);

    copy(svec.crbegin(), svec.crend(), out_iter);
    return 0;
}
