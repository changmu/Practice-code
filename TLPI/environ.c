#include <myLinux.h>

int main(int argc, char **argv)
{
    /*
    extern char **environ;
    int i;

    for (i = 0; environ[i] != NULL; ++i)
        printf("%s\n", environ[i]);

    printf("i: %d\n", i);
*/

    int i = argc + 1;
    int j = i + 21;

    for ( ; i < j; ++i)
        printf("%s\n", argv[i]);

    while (1);
    return 0;
}
