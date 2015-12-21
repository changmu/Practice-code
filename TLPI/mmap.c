#include <myLinux.h>

int main(int argc, char **argv)
{
    int *p;
    int fd = open(argv[1], O_RDWR);

    p = mmap(NULL, 6, PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) errExit("mmap");
    close(fd);
    p[0] = 0x30313233;
    munmap(p, 6);
    return 0;
}
