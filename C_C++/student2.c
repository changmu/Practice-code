#include "my_unix.h"

int main()
{
    key_t key = ftok("/home", 1);
    // 打开信号量集合
    int semid = semget(key, 1, IPC_CREAT);
    // 打开文件
    int fd = open("blackboard.txt", O_RDWR | O_APPEND);
    // 获取信号量
    int ret = semctl(semid, 0, GETVAL);
    printf("Initial sem val is %d.\n", ret);
    struct sembuf sops = {0, -1};
    semop(semid, &sops, 1);
    // 写入“英语课考试”
    write(fd, " english exam", 13);
    // 释放信号量
    sops.sem_op = 1;
    semop(semid, &sops, 1);
    // 关闭文件
    close(fd);
    return 0;
}
