#include <iostream>
#include <algorithm>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using std::endl;
using std::string;
using std::size_t;
using std::cout;
// using std::;

class HasPtr {
    friend void swap(HasPtr &lhs, HasPtr &rhs) {
        cout << "local swap(); " << endl;
        using std::swap;
        swap(lhs.ps, rhs.ps);
        swap(lhs.i, rhs.i);
    }
public:
    HasPtr(const string &s = string()):
        ps(new string(s)), i(0), use(new size_t(1)) {}
    HasPtr(const HasPtr &p):
        ps(p.ps), i(p.i), use(p.use) { ++*use; }
    // HasPtr& operator=(const HasPtr&);
    HasPtr& operator=(const HasPtr);
    ~HasPtr() {
        if (!--*use) {
            delete ps;
            delete use;
        }
    }
private:
    string *ps;
    int i;
    size_t *use;
};

/*
HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
    ++*rhs.use;
    if (!--*use) {
        delete ps;
        delete use;
    }
    ps = rhs.ps;
    i = rhs.i;
    use = rhs.use;
    return *this;
}
*/

HasPtr& HasPtr::operator=(const HasPtr rhs)
{
    swap(*this, rhs);
    return *this;
}

int main()
{
    HasPtr a("a"), b("b");
    using std::swap;
    std::swap(a, b);
    return 0;
}
