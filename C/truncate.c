#include <myLinux.h>

int main(int argc, char *argv[])
{
    truncate(argv[1], 6);
    return 0;
}
