#include <iostream>
#include <fstream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class StrBlobPtr;
class StrBlob {
    friend class StrBlobPtr;
public:
    StrBlobPtr begin();
    StrBlobPtr end();
    typedef vector<string>::size_type size_type;
    StrBlob();
    explicit StrBlob(initializer_list<string> il);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const string &t) { data->push_back(t); }
    void pop_back();
    string& front();
    string& back();
private:
    shared_ptr<vector<string>> data;
    void check(size_type i, const string &msg) const;
};

class StrBlobPtr {
public:
    StrBlobPtr(): curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0): wptr(a.data), curr(sz) {}
    string& deref() const;
    StrBlobPtr& incr();
    bool operator!=(const StrBlobPtr &sp) const { return curr != sp.curr; }
private:
    shared_ptr<vector<string>> check(size_t, const string&) const;
    weak_ptr<vector<string>> wptr;
    size_t curr;
};

shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string& msg) const
{
    auto ret = wptr.lock();
    if (!ret)
        throw runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw out_of_range(msg);
    return ret;
}

string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}


StrBlobPtr StrBlob::begin() { return StrBlobPtr(*this); }
StrBlobPtr StrBlob::end() { return StrBlobPtr(*this, data->size()); }
StrBlob::StrBlob(): data(make_shared<vector<string>>()) {}
StrBlob::StrBlob(initializer_list<string> il): 
        data(make_shared<vector<string>> (il)) {}
void StrBlob::check(size_type i, const string &msg) const
{
    if (i >= data->size())
        throw out_of_range(msg);
}
string& StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}
string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}
void StrBlob::pop_back()
{
    check(0, "pop back on empty StrBlob");
    data->pop_back();
}


int main()
{
    {
        StrBlob blob;
        ifstream ifs("./book.txt");

        for (string str; getline(ifs, str); ) {
            blob.push_back(str);
            cout << "+++" << str << "---" << endl;
        }
        cout << "===" << blob.size() << endl;
        for (StrBlobPtr pBeg(blob.begin()), pEnd(blob.end()); pBeg != pEnd; pBeg.incr())
            cout << pBeg.deref() << endl;
    }

    {
        // dynamic array
        unique_ptr<int []> up(new int[10]{1, 2, 3});
        cout << up[1] << endl;
        up.reset();
        // cout << up[1] << endl;
    }

    {
        shared_ptr<int> sp(new int[10], [](int *p) { delete[] p; });
        sp.reset();
    }
    {
        allocator<string> alloc;
        int n = 20;
        auto const p = alloc.allocate(n);
        string *q = p;
        // alloc.construct(p + 2, "AAAAA");
        alloc.deallocate(p, n);
    }
    return 0;
}
