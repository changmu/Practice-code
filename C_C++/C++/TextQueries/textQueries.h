#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

using std::vector;
using std::set;
using std::cout;
using std::cin;
using std::istringstream;
using std::ifstream;
using std::ostream;
using std::map;
using std::shared_ptr;
using std::string;
using std::endl;

class QueryResult;
class TextQuery {
public:
    typedef size_t LineNum;
    TextQuery(ifstream &);
    QueryResult query(const string &) const;
private:
    shared_ptr<vector<string>> input;
    map<string, shared_ptr<set<LineNum>>> result;
};

class QueryResult {
public:
    QueryResult(const string &s, shared_ptr<set<TextQuery::LineNum>> st, shared_ptr<vector<string>> v):
        word(s), nol(st), input(v) {}
private:
    friend ostream &print(ostream &, const QueryResult &);
    string word;
    shared_ptr<set<TextQuery::LineNum>> nol;
    shared_ptr<vector<string>> input;
};

ostream &print(ostream &, const QueryResult &);
