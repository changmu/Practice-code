#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // char str[100] = { 0 };
    char *str;
    scanf("%5ms", &str);
    puts(str);
    free(str);
    return 0;
}
