#include <iostream>
#include <string>
#include <vector>

using namespace std;

class c1 {
friend istream &operator>>(istream &is, c1 &obj);
public:
    c1() = default;
    c1(string str) : name(str + "constructor") {}
    c1 &display() const {
        cout << name << '\t' << a << endl;
        return *const_cast<c1 *>(this);
    }
    /*c1 &display() {
        cout << name << '\t' << a << endl;
        return *this;
    }*/
    c1 &set(const string &str) {
        name = str;
        return *this;
    }
    inline void seta(int b) { a = b; }
private:
    string name = {"hello"};
    int a;
};

istream &operator>>(istream &is, c1 &obj);

int main()
{
    c1 tmp("hi");
    // cout << tmp << endl;
    // cin >> tmp;
    tmp.display();
    tmp.seta(1024);
    tmp.display().set("world");
    return 0;
}

istream &operator>>(istream &is, c1 &obj)
{
    is >> obj.name;
}

