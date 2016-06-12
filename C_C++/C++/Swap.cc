#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace MyWidget {

class Widget {
public:
    Widget(const char* str)
        : p(str)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, Widget& w) {
        return os << w.p;
    }

    void swap(Widget& rhs) {
        printf("member swap invoked.\n"); 
        using std::swap;
        swap(p, rhs.p);
    }
private:
    const char* p;
};

void swap(Widget& lhs, Widget& rhs)
{
    printf("non-member swap invoked.\n"); 
    lhs.swap(rhs);
}

}

namespace std {

template<>
void swap(MyWidget::Widget& lhs, MyWidget::Widget& rhs) {
    printf("std swap invoked.\n"); 
    lhs.swap(rhs);
}

}

int main()
{
    MyWidget::Widget a("123");
    MyWidget::Widget b("abc");

    auto c = 1;

    using std::swap;
    swap(a, b);
    // std::swap(a, b);

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
