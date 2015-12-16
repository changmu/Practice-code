#include <myLinux.h>

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "usage: %s <filePath> <uid> <gid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    chown(argv[1], atoi(argv[2]), atoi(argv[3]));

    return 0;
}
