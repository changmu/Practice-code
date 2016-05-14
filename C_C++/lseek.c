#include <myLinux.h>

int main()
{
    int fd = open("out", O_WRONLY | O_CREAT | O_EXCL, 0664);
    if (fd < 0) {
        perror("open out");
        exit(1);
    }

    lseek(fd, 1UL << 32, SEEK_END);
    write(fd, "EOF", 3);

    close(fd);
    return 0;
}
