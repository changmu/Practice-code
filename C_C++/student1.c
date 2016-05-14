#include "my_unix.h"

int main()
{
    // 创建键值
    key_t key = ftok("/home", 1);
    // 创建信号量
    int semid = semget(key, 1, IPC_CREAT);
    // 初始化信号量
    semctl(semid, 0, SETVAL, 1);
    printf("Init sem val is %d.\n", semctl(semid, 0, GETVAL));
    // 打开文件
    int fd = open("blackboard.txt", O_RDWR | O_APPEND);
    // 获取信号量
    struct sembuf sops = {0, -1};
    semop(semid, &sops, 1);
    // 向文件里写入“数学课”
    write(fd, "math class", 10);
    // 暂停
    sleep(5);
    // 继续写“取消”
    write(fd, " is canceld", 11);
    // 释放信号量
    sops.sem_op = 1;
    semop(semid, &sops, 1);
    close(fd);
    return 0;
}
