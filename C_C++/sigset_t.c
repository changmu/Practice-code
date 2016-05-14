#include <myLinux.h>

int main(int argc, char *argv[])
{
    printf("%u\n", sizeof(sigset_t));
    printf("%u\n", sizeof(unsigned long int));
    return 0;
}
