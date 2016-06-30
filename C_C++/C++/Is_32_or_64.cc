#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int main()
{
    cout << (
            sizeof(void *) == 4 ?
                "x86" : "x64"
            ) << endl;
    return 0;
}
