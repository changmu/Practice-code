#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class T {
public:
    T(int i) {
        cout << "T(int)" << endl;
    }
    T(const T &t) {
        cout << "T(&)" << endl;
    }
};

int main()
{
    T a = 5;
    return 0;
}
