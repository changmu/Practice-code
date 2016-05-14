#include "../my_unix.h"

int main()
{
    struct sembuf sops;
    int semid;

    // creat semaphore
    semid = semget(ftok("/home", 2), 1, IPC_CREAT);
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = SEM_UNDO;
    semop(semid, &sops, 1);
    
    // fetch product
    system("cp ./product.txt ./ship");
    return 0;
}
