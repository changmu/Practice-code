#include <stdio.h>
#include <string.h>

double sum_elements(double a[], unsigned length) {
    int i;
    double result = 0;

    for (i = 0; i <= length - 1; ++i)
        result += a[i];
    return result;
}

int main()
{
    double a[] = {1, 2, 3};

    printf("%lf\n", sum_elements(a, 3));
    printf("%lf\n", sum_elements(a, 0));
    return 0;
}
