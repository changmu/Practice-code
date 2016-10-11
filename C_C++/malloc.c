#include "myLinux.h"

int main(int argc, char *argv[])
{
    char *ptr = malloc(0);
    assert(ptr == NULL);
    return 0;
}
