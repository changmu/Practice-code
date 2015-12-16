#include "my_unix.h"

int main()
{
    int pipefd[2];
    char buf[] = "hello";

    // 1. 创建管道
    pipe(pipefd);

    // 2. 创建子进程
    if (fork()) {
        // 父进程
        write(pipefd[1], buf, 6);
        // wait(NULL);
        close(pipefd[1]);
        exit(0);
    } else {
        // 子进程
        read(pipefd[0], buf, 6);
        printf("Child is %s.\n", buf);
        close(pipefd[0]);
        exit(0);
    }

    return 0;
}
