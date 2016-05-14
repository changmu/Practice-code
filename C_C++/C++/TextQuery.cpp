#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>

using namespace std;

typedef map<string, pair<int, set<int>>> tq_t;
class TextQuery {
public:
    TextQuery(ifstream &in);
    void runQueries(ifstream &in);
    tq_t::iterator query(string &s);
    ostream& print(ostream& out, tq_t::iterator it);
private:
    map<string, pair<int, set<int>>> tq;
};

TextQuery::TextQuery(ifstream &in)
{
    vector<string> raw;
    map<string, int> wordCnt;
    map<string, set<int>> wordLine;

    string s;
    // read text as many lines
    while (getline(in, s)) {
        raw.push_back(s);
    }

    // read word from line
    for (size_t i = 0; i != raw.size(); ++i) {
        istringstream is(raw[i]);
        string w;

        while (is >> w) {
            ++wordCnt[w];
            wordLine[w].insert(i + 1); // line num
        }
    }

    // make up
    for (auto it = wordCnt.begin(); it != wordCnt.end(); ++it) {
        tq.emplace(it->first, make_pair(it->second, wordLine[it->first]));
    }
}

tq_t::iterator TextQuery::query(string &s)
{
    return tq.find(s);
}

ostream& TextQuery::print(ostream& out, tq_t::iterator it)
{
    if (it != tq.end()) {
        out << it->first << ": " << it->second.first << endl;
        for (auto beg = it->second.second.begin(); beg != it->second.second.end(); ++beg)
            out << *beg << '\t';
    }

    return out;
}

void runQueries(ifstream &infile)
{
    TextQuery tq(infile);
    while (true) {
        cout << "Enter word to look for, or q to quit: ";
        string s;
        if (!(cin >> s) || s == "q") break;
        tq.print(cout, tq.query(s)) << endl;
    }
}

int main(int argc, char **argv)
{
    ifstream is(argv[1]);
    runQueries(is);
    return 0;
}
