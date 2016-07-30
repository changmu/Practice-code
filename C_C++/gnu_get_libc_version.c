#include "myLinux.h"
#include <gnu/libc-version.h>

int main(int argc, char *argv[])
{
    puts(gnu_get_libc_version());
    puts(gnu_get_libc_release());
    return 0;
}
