#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    if (fd < 0)
        printf("Fail to open.\n");

    char buf[10];
    const char *sam = "hello";
    write(fd, sam, 5);
/*
    read(fd, buf, 5);
    printf("%s\n", buf);

    lseek(fd, 1, SEEK_SET);
    read(fd, buf, 5);
    printf("%s\n", buf);
    read(fd, buf, 5);
    printf("%s\n", buf);
*/
    close(fd);
    return 0;
}
