#include <myLinux.h>

int main(int argc, char **argv)
{
    int fd[2];

    pipe(fd);

    printf("%ld\n", fpathconf(fd[0], _PC_PIPE_BUF));
    return 0;
}
