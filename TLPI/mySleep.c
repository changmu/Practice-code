#include <myLinux.h>

void capture_sig(int signum)
{
    /* nothing */
}

void mySleep(unsigned nsec)
{
    sigset_t            oldset, newset;
    struct sigaction    oldact, newact;

    /* set new mask */
    sigemptyset(&newset);
    sigaddset(&newset, SIGALRM);
    sigprocmask(SIG_SETMASK, &newset, &oldset);

    /* set new act */
    newact.sa_handler = capture_sig;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);

    alarm(nsec);
    sigdelset(&newset, SIGALRM);
    sigsuspend(&newset);

    /* resume mask and act */
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    sigaction(SIGALRM, &oldact, NULL);
}

int main(int argc, char *argv[])
{
    mySleep(5);
    return 0;
}
