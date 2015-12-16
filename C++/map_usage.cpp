#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <map>

using namespace std;

int main()
{
    map<string, size_t> word_count;

    /*
     * Inset a value-initialized element with key Anna; then assign 1 to its
     * value
     * */
    word_count["Anna"] = 1;
    cout << word_count["Anna"] << endl;

    cout << word_count.size() << endl;
    /*
     * Subscript operator in a map would insert an element, so the size
     * would add one.
     * */
    word_count["asd"];
    cout << word_count.size() << endl;

    map<string, int> authors;
    authors.emplace("Alain de Botton", 20);
    authors.emplace("Alain de Botton", 21);
    string search_item("Alain de Botton");
    auto entries = authors.count(search_item);
    auto iter = authors.find(search_item);
    while (entries) {
        cout << iter->second << endl;
        ++iter;
        --entries;
    }

    for (auto beg = authors.lower_bound(search_item),
              end = authors.upper_bound(search_item);
         beg != end; ++beg)
        cout << beg->second << endl;

    // authors.insert({"Alain de Botton", 21});
    authors["Alain de Botton"] = 21;
    auto pos = authors.equal_range(search_item);
    for ( ; pos.first != pos.second; ++pos.first)
        cout << pos.first->second << endl;
    return 0;
}
