#define _GNU_SOURCE
#include "myLinux.h"

int main(int argc, char *argv[])
{
    char *bufp = (char *) get_current_dir_name();
    puts(bufp);
    free(bufp);
    return 0;
}

