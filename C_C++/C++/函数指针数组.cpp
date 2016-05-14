#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef int (*f)(int, int);
vector<f> vecf;
int (*arr[4])(int, int);

int f1(int a, int b) {
    return a + b;
}

int f2(int a, int b) {
    return a - b;
}

int f3(int a, int b) {
    return a * b;
}

int f4(int a, int b) {
    return a / b;
}

int main()
{
    int i;
    vecf.push_back(f1);
    vecf.push_back(f2);
    vecf.push_back(f3);
    vecf.push_back(f4);

    arr[0] = f1;
    arr[1] = f2;
    arr[2] = f3;
    arr[3] = f4;

    for (auto it = vecf.begin(); it != vecf.end(); ++it)
        cout << (*it)(10, 2) << endl;

    for (i = 0; i < 4; ++i)
        cout << (*arr[i])(10, 2) << endl;
    return 0;
}
