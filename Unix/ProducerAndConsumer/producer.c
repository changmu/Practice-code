#include "../my_unix.h"

int main()
{
    int fd;
    int semid;
    struct sembuf sops;

    // creat semaphore
    semid = semget(ftok("/home", 2), 1, IPC_CREAT);
    semctl(semid, 0, SETVAL, 0);

    // creat product
    fd = open("./product.txt", O_RDWR | O_CREAT, 0777);

    // reset
    sleep(10);

    // fill in the product
    write(fd, "the product is finished. ", 25);

    // close
    close(fd);

    // release semaphore
    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = SEM_UNDO;
    semop(semid, &sops, 1);
    return 0;
}
