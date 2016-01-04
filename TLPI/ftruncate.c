#include <myLinux.h>

int main(int argc, char *argv[])
{
    int fd = open("tmp.out", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    ftruncate(fd, 10);
    close(fd);
    return 0;
}
