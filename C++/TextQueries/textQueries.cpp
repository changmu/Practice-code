#include "textQueries.h"

TextQuery::TextQuery(ifstream &ifs): input(new vector<string>)
{
    LineNum lineNum(1);
    for (string line; getline(ifs, line); ++lineNum) {
        input->push_back(line);
        istringstream lineStream(line);
        for (string text, word; lineStream >> text; word.clear()) {
            remove_copy_if(text.begin(), text.end(), back_inserter(word), ispunct);
            auto &nol = result[word];
            if (!nol) nol.reset(new set<LineNum>);
            nol->insert(lineNum);
        }
    }
}

QueryResult TextQuery::query(const string &str) const
{
    static shared_ptr<set<LineNum>> noDate(new set<LineNum>);
    auto found = result.find(str);
    if (found == result.end()) return QueryResult(str, noDate, input);
    else return QueryResult(str, found->second, input);
}

ostream &print(ostream &out, const QueryResult &qr)
{
    out << qr.word << " occurs " << qr.nol->size() << " times" << endl;
    for (auto i: *qr.nol)
        out << "\t(line " << i << ") " << qr.input->at(i-1) << endl;

    return out;
}
