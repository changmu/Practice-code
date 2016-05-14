#include <myLinux.h>

void do_sig_child(int signo)
{
    int     status;
    pid_t   pid;

    while ((pid = waitpid(0, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status))
            printf("child %d exit %d\n", pid, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("child %d cancel signal %d\n", pid, WTERMSIG(status));
    }
}

int main(int argc, char *argv[])
{
    pid_t   pid;
    int     i;

    for (i = 0; i < 10; ++i) {
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
            errExit("fork()");
    }

    if (pid == 0) {
        int n = 8;
        while (n--) {
            printf("child ID %d\n", getpid());
            sleep(1);
        }
        return i;
    } else {
        struct sigaction act;

        act.sa_handler = do_sig_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, NULL);

        while (1) {
            printf("Parent ID %d\n", getpid());
            sleep(3);
        }
    }
    return 0;
}
