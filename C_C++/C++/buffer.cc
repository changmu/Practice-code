#include <stdio.h>
#include <string.h>

int main()
{
    int i = 5.01;
    float f = 5;

    printf("%f\n", 5);
    printf("%lf\n", 5.01);
    printf("%f\n", f);
    printf("%d\n", 5.01);
    printf("%d\n", i);
    printf("sizeof=%zu\n", sizeof(int));
    printf("sizeof=%zu\n", sizeof(int *));

    return 0;
}
