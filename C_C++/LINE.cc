#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int main()
{
    cout << "Compile info:" << endl
        << __func__ << endl
        << __FILE__ << endl
        << __DATE__ << endl
        << __TIME__ << endl
        << __LINE__ << endl;
    sleep(2);
    cout << "Compile info:" << endl
        << __func__ << endl
        << __FILE__ << endl
        << __DATE__ << endl
        << __TIME__ << endl
        << __LINE__ << endl;
    return 0;
}
