#include "myLinux.h"
#include "pub.h"

int main(int argc, char *argv[])
{
        int shmid = shmget(1234, sizeof(STU), IPC_CREAT | 0666);
        if (shmid == -1)
                ERR_EXIT("shmget");

        STU *p;
        p = shmat(shmid, NULL, 0);
        if (p == (void *) -1)
                ERR_EXIT("shmat");

        strcpy(p->name, "lisi");
        p->age = 20;

        sleep(10);
        strcpy(p->name, "quit");
        shmdt(p);

        return 0;
}
