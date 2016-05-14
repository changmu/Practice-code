#include <myLinux.h>

int main(int argc, char **argv)
{
    char buf[1024];

    printf("PWD=%s\n", getenv("PWD"));
    printf("getcwd(): %s\n", getcwd(buf, sizeof(buf)));
    setenv("PWD", "/", 1);
    unsetenv("PWD");
    printf("PWD=%s\n", getenv("PWD"));
    printf("getcwd(): %s\n", getcwd(buf, sizeof(buf)));

    printf("NULL: %s\n", NULL);

    return 0;
}
