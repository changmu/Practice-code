#include <myLinux.h>

#define MAX_LEN 1024

int main(int argc, char **argv)
{
    char buf[MAX_LEN];
    int i;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) errExit("pipe");

    if (fork()) {
        // parent close read fd[0];
        close(pipe_fd[0]);
        while (scanf("%s", buf) != EOF)
            write(pipe_fd[1], buf, strlen(buf) + 1);
        wait(NULL);
    } else {
        // child close write fd[1]
        close(pipe_fd[1]);
        while (read(pipe_fd[0], buf, sizeof(buf)), strcmp(buf, "end"))
            printf("child with pid: %d, output %s\n", getpid(), buf);
    }
    return 0;
}
