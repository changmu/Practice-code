#include "textQueries.h"

void runQueries(ifstream &inputFile)
{
    TextQuery tq(inputFile);
    while (true) {
        cout << "enter word to look for, or q to quit: ";
        string str;
        if (!(cin >> str) || str == "q") break;
        print(cout, tq.query(str)) << endl;
    }
}

int main(int argc, char **argv)
{
    ifstream inputFile(argv[1]);
    runQueries(inputFile);
    return 0;
}
