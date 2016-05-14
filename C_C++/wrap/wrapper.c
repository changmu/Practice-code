#include <myLinux.h>

int main(int argc, char **argv)
{
    int fd;
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) errExit("open");

    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        exit(1);
    }

    dup2(fd, fileno(stdin));
    execl("b.out", "b.out", NULL);

    return 0;
}
