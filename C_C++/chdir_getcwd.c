#include <myLinux.h>

char buf[1024];

int main(int argc, char **argv)
{
    printf("%s\n", getcwd(buf, sizeof(buf)));
    chdir("/home");
    printf("%s\n", getcwd(buf, sizeof(buf)));
    return 0;
}
