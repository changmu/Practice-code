/* 每隔10s向/tmp/daemon.log文件写入当前时间 */
/*
   1. 创建子进程，父进程退出
   2. 在子进程中创建会话
   3. 改变当前目录
   4. 重设文件权限掩码
   5. 关闭文件描述符
   6. 进入核心工作
   7. 守护进程退出处理
*/

#include <myLinux.h>

void daemon_fn()
{
    int fd = open("/tmp/daemon.log", O_CREAT | O_TRUNC | O_WRONLY, 0664);
    if (fd == -1) errExit("open");

    time_t current_time;
    char *pc;

    while (1) {
        current_time = time(NULL);
        pc = ctime(&current_time);
        if ((write(fd, pc, strlen(pc))) == -1)
            errExit("write to file");

        sleep(10);
    }

    return;
}

int main(int argc, char *argv[])
{
    if (fork()) exit(0);

    /* preparation */
    setsid();
    if (chdir("/tmp") == -1)
        errExit("chdir");
    if (umask(0002) == -1)
        errExit("umask");
    close(0);
    open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);

    daemon_fn();

    return 0;
}
