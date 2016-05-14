#include <iostream>
#include <string>

using namespace std;

// literal class
class Debug {
public: 
    constexpr Debug(bool b = true): hw(b), io(b), other(b) {}
    constexpr Debug(bool h, bool i, bool o):
        hw(h), io(i), other(o) {}
    constexpr bool any() { return hw || io || other; }
    void set_io(bool b) { io = b; }
    void set_hw(bool b) { hw = b; }
    void set_other(bool b) { other = b; }
private:
    bool hw;    // hardware error
    bool io;    // io error
    bool other;
};

int main()
{
    Debug db;
    if (db.any()) cout << "error occurs." << endl;
    return 0;
}
