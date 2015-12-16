#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUF_SIZE /* Allow "cc -D" to override */
#define BUF_SIZE 1024
#endif

int main(int argc, char **argv)
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (3 != argc || !strcmp(argv[1], "--help")) {
        fprintf(stderr, "%s old-file new-file\n", argv[0]);
    }

    if((inputFd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "opening file %s\n", argv[1]);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (-1 == outputFd)
        fprintf(stderr, "opening file %s\n", argv[2]);

    /* Transfer data until encounter EOF or error */
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead) {
            fprintf(stderr, "couldn't write whole buffer\n");
            exit(EXIT_FAILURE);
        }
    if (-1 == numRead) {
        fprintf(stderr, "couldn't write whole buffer..\n");
        exit(EXIT_FAILURE);
    }

    if (close(inputFd) == -1)
        fprintf(stderr, "close input err.\n");
    if (close(outputFd) == -1)
        fprintf(stderr, "close outputFd err.\n");

    return 0;
}
