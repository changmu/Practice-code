#include <myLinux.h>

int main(int argc, char *argv[])
{
    struct utimbuf times = {200101010909, 199909091010};
    utime(argv[1], &times);
    return 0;
}
