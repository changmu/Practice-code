#include <myLinux.h>

int main(int argc, char **argv)
{
    int cpid;

    if ((cpid = fork()) < 0) {
        errExit("fork()");
    } else if (cpid == 0) {
        int i = 8;
        while (i--) {
            printf("This is the child.\n");
            sleep(1);
        }
        exit(3);                
    } else {
        int stat_val;
        waitpid(cpid, &stat_val, 0);
        if (WIFEXITED(stat_val))
            printf("Child exited with code %d.\n", WEXITSTATUS(stat_val));
        else if (WIFSIGNALED(stat_val))
            printf("Child terminated abnormally, signal %d\n", WTERMSIG(stat_val));
    }
    return 0;
}
