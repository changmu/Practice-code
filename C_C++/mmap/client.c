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
        fd = open(argv[1], O_RDWR);
        if (fd == -1)
                ERR_EXIT("open");

        STU *p;
        p = mmap(NULL, sizeof(STU) * 5, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (p == NULL)
                ERR_EXIT("mmap");

        int i;
        int n = 5;
        for (i = 0; i < n; ++i) {
                printf("name: %s\n", (p + i)->name);
                printf("age: %d\n", (p + i)->age);
        }

        sleep(5);
        munmap(p, sizeof(STU) * 5);
        printf("exit..\n");

        return 0;
}
