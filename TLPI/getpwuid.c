#include <myLinux.h>

int main(int argc, char **argv)
{
    /*
    struct passwd *pd;
    pd = getpwuid(1000);
    printf("%s..\n", pd->pw_name);
    printf("%s..\n", pd->pw_passwd);
    */

    printf("%s..\n", getpwuid(1000)->pw_name);
    return 0;
}
