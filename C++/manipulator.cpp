#include <iostream>
#include <ios>
#include <iomanip>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

const double PI = acos(-1.0);

int main()
{
    cout << fixed;
    double t = 3;
    cout << t << endl;

    cout << uppercase << showbase << hex 
         << 12 
         << noshowbase << nouppercase << dec
         << endl;

    cout << cout.precision() << endl;
    cout << PI << endl;

    
    cout << setprecision(30); 
    cout << PI << endl;

    cout << boolalpha << true << endl;

    int ch;
    while ((ch = cin.get()) != EOF)
        cout.put(ch);
    return 0;
}
