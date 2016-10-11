#include "myLinux.h"

int main(int argc, char *argv[])
{
    errno = 2;
    int n = 0;
    printf("%m%n\n", &n);
    printf("n = %d\n", n);
    return 0;
}
