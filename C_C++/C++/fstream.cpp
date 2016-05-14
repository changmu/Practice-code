#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    ifstream fin("test.txt");
    ofstream fout("test2.txt", ofstream::app);
    string str, str2, str3;

    ostringstream strout(ostringstream::ate);

    while (getline(fin, str)) {
        istringstream strin(str);
        // fout << str << endl;
        while (strin >> str2) {
            // cout << "hello: " << str2 << endl;
        }

        strout << str.size();
        cout << strout.str() << endl;
    }

    return 0;
}
