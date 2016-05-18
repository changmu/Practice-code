#include "myLinux.h"

typedef struct stu {
        char name[4];
        int age;
} STU;

int main(int argc, char *argv[])
{
        if (argc != 2) {
                fprintf(stderr, "Usage: %s <file>\n", basename(argv[0]));
                exit(EXIT_FAILURE);
        }

        int fd;
        fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0666);
        if (fd == -1)
                ERR_EXIT("open");
        lseek(fd, sizeof(STU) * 5 - 1, SEEK_SET);
        write(fd, "", 1);

        STU *p;
        p = mmap(NULL, sizeof(STU) * 5, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (p == NULL)
                ERR_EXIT("mmap");

        char ch = 'a';
        int i;
        int n = 5;
        for (i = 0; i < n; ++i) {
                memcpy((p + i)->name, &ch, 1);
                (p + i)->age = 20 + i;
                ++ch;
        }

        printf("initialize over\n");
        munmap(p, sizeof(STU) * 5);
        printf("exit..\n");

        return 0;
}
