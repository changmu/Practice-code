#include "myLinux.h"

int sem_create(key_t key)
{
        int semid;
        semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
        if (semid == -1)
                ERR_EXIT("semget");

        return semid;
}

int sem_open(key_t key)
{
        int semid;
        semid = semget(key, 1, 0);
        if (semid == -1)
                ERR_EXIT("semget");

        return semid;
}

int main(int argc, char *argv[])
{
        int semid;
        sem_create(1234);
        semid = sem_open(1234);
        return 0;
}
