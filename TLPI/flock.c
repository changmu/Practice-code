#include <myLinux.h>

int main(int argc, char *argv[])
{
    int fd;
    struct flock f_lock;

    if (argc < 2) {
        printf("./a.out file\n");
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        errExit("open file");
    }

    f_lock.l_type   = F_WRLCK;
    f_lock.l_whence = SEEK_SET;
    f_lock.l_start  = 0;
    f_lock.l_len    = 0; // lock the whole file
    fcntl(fd, F_SETLKW, &f_lock);
    printf("get flock\n");

    sleep(10);
    f_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &f_lock);
    printf("free flock\n");
    close(fd);

    return 0;
}
