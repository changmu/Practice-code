#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Csv {
public:
    Csv(istream& fin = cin, string sep = ","):
        fin(fin), fieldSep(sep) {}

    int getLine(string&);
    string getField(int n);
    int getNField() const { return nField; }

private:
    istream& fin;           //input file pointer
    string line;            // input line
    vector<string> field;   // field strings
    int nField;             // number of fields
    string fieldSep;        // separator characters

    int split();
    int endOfLine(char);
    int advplain(const string& line, string& fld, int);
    int advquoted(const string& line, string& fld, int);
};

// getLine: get one line, grow as needed
int Csv::getLine(string& str) {
    char c;

    for (line = ""; fin.get(c) && !endOfLine(c); )
        line += c;
    split();
    str = line;

    return !fin.eof();
}

//endOfLine: check for and consume \r, \n, \r\n, or EOF
int Csv::endOfLine(char c) {
    int eol = (c == '\r' || c == '\n');
    if (c == '\r') {
        fin.get(c);
        if (!fin.eof() && c != '\n')
            fin.putback(c);     // read too far
    }

    return eol;
}

// split: split line into fields
int Csv::split() {
    string fld;
    int i, j;

    nField = 0;
    if (line.length() == 0)
        return 0;

    i = 0;
    do {
        if (i < line.length() && line[i] == '"')
            j = advquoted(line, fld, ++i);      // skip quote
        else
            j = advplain(line, fld, i);

        if (nField >= field.size())
            field.push_back(fld);
        else
            field[nField] = fld;

        ++nField;
        i = j + 1;
    } while (j < line.length());

    return nField;
}

// advquoted: quoted field; return index of next separator
int Csv::advquoted(const string& s, string& fld, int i) {
    int j;

    fld = "";
    for (j = i; j < s.length(); ++j) {
        if (s[j] == '"' && s[++j] != '"') {
            int k = s.find_first_of(fieldSep, j);
            if (k > s.length())     // no separator found
                k = s.length();
            for (k -= j; k-- > 0; )
                fld += s[j++];
            break;
        }
        fld += s[j];
    }

    return j;
}

// advplain: unquoted field; return index of next separator
int Csv::advplain(const string& s, string& fld, int i) {
    int j;

    j = s.find_first_of(fieldSep, i);       // look for separator
    if (j > s.length())     // none found
        j = s.length();
    fld = string(s, i, j - i);

    return j;
}

// getField: return n-th field
string Csv::getField(int n) {
    if (n < 0 || n >= nField)
        return "";
    else
        return field[n];
}

int main(void) {
    string line;
    Csv csv;

    while (csv.getLine(line) != 0) {
        cout << "line = '" << line << "'\n";
        for (int i = 0; i < csv.getNField(); ++i)
            cout << "field[" << i << "] = '"
                 << csv.getField(i) << "'\n";
    }

    return 0;
}
