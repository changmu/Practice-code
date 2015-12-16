#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fd[2];
    pid_t pid;
    char str[1024] = "hello world";
    char buf[1024];

    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    // 父写子读
    if (pid > 0) {
        // in parent, close parent read
        close(fd[0]);
        write(fd[1], str, strlen(str));
        wait(NULL);
    } else if (pid == 0) {
        // in child, close child write
        int len;
        close(fd[1]);
        len = read(fd[0], buf, sizeof(buf));
        // sprintf(str, "child %s", buf);
        write(STDOUT_FILENO, str, strlen(str));
    } else {
        perror("fork");
        exit(1);
    }
    return 0;
}
