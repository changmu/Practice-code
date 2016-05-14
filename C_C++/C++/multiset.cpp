#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <deque>
#include <functional>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

struct Node {
    int u, v;
    friend bool operator<(const Node &a, const Node &b) {
        return a.u < b.u;
    }
};

bool cmp(const Node &a, const Node &b) {
    return a.u < b.u;
}

int main()
{
    // multiset<Node, decltype(cmp) *> mst(cmp);
    multiset<Node> mst;
    Node u = {1, 2};
    mst.insert(u);
    u = {2, 3};
    mst.insert(u);
    // for (auto i = mst.begin(); i != mst.end(); ++i)
    for (auto i : mst)
        cout << i.u << endl;
    return 0;
}
