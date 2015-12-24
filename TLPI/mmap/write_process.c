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
    int fd, i;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <fileName>\n", argv[0]);
        exit(1);
    }

    fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    if (fd == -1) errExit("open");

    if (lseek(fd, MAP_LEN - 1, SEEK_SET) < 0) errExit("lseek");
    if (write(fd, "\0", 1) == -1) errExit("write");

    mp = mmap(NULL, MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mp == MAP_FAILED) errExit("mmap");

    close(fd);

    for (i = 0; ; ++i) {
        mp->id = i;
        sprintf(mp->name, "student-%d", i);
        mp->sex = (i % 2 == 0 ? 'm' : 'w');

        sleep(1);
    }
    munmap(mp, MAP_LEN);

    return 0;
}
