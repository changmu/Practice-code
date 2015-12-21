#include <myLinux.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <fifoName>\n", argv[0]);
        exit(1);
    }

    if (mkfifo(argv[1], 0644) == -1) errExit("mkfifo");

    int fd_write = open(argv[1], O_WRONLY);
    char buf[1024];

    int sz;
    while ((sz = read(STDIN_FILENO, buf, sizeof(buf))) != 0) {
        write(fd_write, buf, sz);
    }

    return 0;
}
