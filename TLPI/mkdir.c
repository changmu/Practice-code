#include <myLinux.h>

int main(int argc, char **argv)
{
    mkdir(argv[1], 0777);
    return 0;
}
