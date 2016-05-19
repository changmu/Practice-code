#include "myLinux.h"

typedef struct stu {
        char name[32];
        int age;
} STU;

int main(int argc, char *argv[])
{
        int shmid;
        shmid = shm_open("/xyz", O_CREAT | O_RDWR, 0666);
        if (shmid < 0)
                ERR_EXIT("shm_open");

        printf("open succed\n");

        int ret = ftruncate(shmid, sizeof(STU));
        if (ret == -1)
                ERR_EXIT("ftruncate");

        close(shmid);

        return 0;
}
