#include <myLinux.h>

int main(int argc, char **argv)
{
    int counter;
    alarm(1);
    for (counter = 0; 1; ++counter)
        printf("counter = %d\n", counter);
    return 0;
}
