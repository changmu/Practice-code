#include <myLinux.h>

int main(int argc, char **argv)
{
    printf("%ld..\n", pathconf(argv[1], _PC_NAME_MAX));
    return 0;
}
