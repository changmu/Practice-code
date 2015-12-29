#include <myLinux.h>

void daemonize()
{
    pid_t pid;

    /* to be a new session header */
    if ((pid = fork()) < 0)
        errExit("fork()");
    else if (pid != 0)
        exit(0);
    setsid();

    /* change current work directory */
    if (chdir("/") < 0)
        errExit("chdir");

    /* set umask */
    umask(0);

    /* redirect 0, 1, 2 file discriptor */
    close(0);
    open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);
}

int main(int argc, char *argv[])
{
    daemonize();
    while (1);
    return 0;
}
