#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s filename filename\n", argv[0]);
        return -1;
    }

    int fd_s, fd_t;
    size_t cnt;
    char buf[BUFSIZE];
    /*
     * Open source file and target file
     * */
    fd_s = open(argv[1], O_RDONLY);
    if (-1 == fd_s) {
        fprintf(stderr, "Couldn't open file %s.\n", argv[1]);
        return -1;
    }
    fd_t = open(argv[2], O_WRONLY | O_CREAT, 0755);
    if (-1 == fd_t) {
        fprintf(stderr, "Couldn't open file %s.\n", argv[2]);
        return -1;
    }

    /*
     * Read and write
     * */
    while ((cnt = read(fd_s, buf, BUFSIZE)) > 0)
        write(fd_t, buf, cnt);

    /*
     * Close file
     * */
    close(fd_t);
    close(fd_s);
    return 0;
}
