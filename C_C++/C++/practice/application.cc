#include "application.h"

/*extern */template class A<int>;
/*extern */template int compare(const int &, const int &);

int main()
{
    A<int> a;
    a.f();
    compare(1, 2);
    return 0;
}
