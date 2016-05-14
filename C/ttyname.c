#include <myLinux.h>

int main(int argc, char *argv[])
{
    int i;

    for (i = 0; i < 3; ++i)
        printf("fd %d: %s\n", i, ttyname(i));
    return 0;
}
