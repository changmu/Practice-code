#include <stdio.h>
#include <string.h>

/*
 * This header is for var arguments programming
 * */
#include <stdarg.h>

int getSum(int cnt, ...)
{
    va_list ap;         // This is for storing args
    va_start(ap, cnt);  // Initial ap using the first arg.

    int sum = 0, i;

    for (i = 0; i < cnt; ++i) {
        /* 
         * Read argument with denoting type. 
         * ap increases automatically.
         */ 
        sum += va_arg(ap, int);     
    }

    va_end(ap);         // Clean ap

    return sum;
}

int main()
{
    printf("%d\n", getSum(4, 2, 3, 4, 5));
    return 0;
}
