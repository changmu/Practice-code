#ifndef SALES_ITEM_H
#define SALES_ITEM_H

#include <iostream>
#include <string>

using namespace std;

class Sales_item {
public:
    string isbn;
    int copies;
    double price, revenue;
public:
    // friend istream& operator>>(istream& is, Sales_item& book);
    // friend ostream& operator<<(ostream& os, const Sales_item& book);
    friend istream& operator>>(istream& is) this {
        is >> isbn >> copies >> price;
        revenue = copies * price;
        return is;
    }

    friend ostream& operator<<(ostream& os) this {
        os << isbn << ", " << copies << ", "
            << price << ", " << revenue << endl;
        return os;
    }

    Sales_item operator+(const Sales_item& book);
    Sales_item operator+=(const Sales_item& book);
};

#endif
