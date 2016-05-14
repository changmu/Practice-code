#include <myLinux.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <fifoName>\n", argv[0]);
        exit(1);
    }

    int fd_read = open(argv[1], O_RDONLY);
    char buf[1024];
    int sz;

    while ((sz = read(fd_read, buf, sizeof(buf))) != 0)
        write(STDOUT_FILENO, buf, sz);

    close(fd_read);

    return 0;
}
