#include <myLinux.h>

int main(int argc, char **argv)
{
    if (fork()) {
        while (1);
    }
    return 0;
}
