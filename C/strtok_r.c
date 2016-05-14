#include <myLinux.h>

int main(int argc, char **argv)
{
    char buf[] = "hello world haha qiu";
    char *save = buf, *p;

    while ((p = strtok_r(save, " ", &save)) != NULL)
        printf("%s\n", p);
    return 0;
}
