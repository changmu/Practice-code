#include <myLinux.h>

#define MAP_LEN 0x1000

struct Stu {
    int id;
    char name[20];
    char sex;
};

int main(int argc, char **argv)
{
    struct Stu *mp;
    int fd;

    fd = open(argv[1], O_RDONLY);

    mp = mmap(NULL, MAP_LEN, PROT_READ, MAP_SHARED, fd, 0);
    if (mp == MAP_FAILED) errExit("mmap");

    close(fd);
    unlink(argv[1]);

    while (1) {
        printf("%d\n%s\n%c\n-----------\n", mp->id, mp->name, mp->sex);
        sleep(1);
    }
    munmap(mp, MAP_LEN);

    return 0;
}
