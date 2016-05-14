#include "Sales_item.h"

Sales_item Sales_item::operator+(const Sales_item& book) const {
    Sales_item book2;
    if (book.isbn != isbn)
        return *this;

    book2.copies = copies + book.copies;
    book2.revenue = revenue + book.revenue;

    return book2;
}

Sales_item Sales_item::operator+=(const Sales_item& book) {
    if (isbn != book.isbn)
        return *this;
    copies += book.copies;
    revenue += book.revenue;

    return *this;
}
