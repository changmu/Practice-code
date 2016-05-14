#include <myLinux.h>

int main(int argc, char *argv[])
{
    int fd;
    struct flock f_lock;
    if (argc != 2) {
        printf("usage: %s <file>\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) < 0)
        errExit("open");

    f_lock.l_type = F_WRLCK;
    f_lock.l_whence = SEEK_SET;
    f_lock.l_start = 0;
    f_lock.l_len = 0;

    fcntl(fd, F_SETLKW, &f_lock);
    printf("get lock\n");
    sleep(10);
    f_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &f_lock);
    printf("release flock\n");

    close(fd);
    return 0;
}
