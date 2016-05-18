#include "myLinux.h"
#include "pub.h"

int main(int argc, char *argv[])
{
        int shmid = shmget(1234, 0, 0);
        if (shmid == -1)
                ERR_EXIT("shmget");

        STU *p;
        p = shmat(shmid, NULL, 0);
        if (p == (void *) -1)
                ERR_EXIT("shmat");

        printf("name = %s age = %d\n", p->name, p->age);

        while (1) {
                if (memcmp(p, "quit", 4) == 0)
                        break;
        }

        shmdt(p);

        shmctl(shmid, IPC_RMID, NULL);

        return 0;
}
