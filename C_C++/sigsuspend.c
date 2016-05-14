#include <myLinux.h>

void sig_alarm(int n)
{
    /* do nothing */
}

/* return the seconds unslept */
unsigned mySleep(unsigned nsecs)
{
    struct sigaction    newact, oldact;
    sigset_t            newmask, oldmask, suspmask;
    unsigned            unslept;

    /* set our handler, save previous information */
    newact.sa_handler = sig_alarm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);

    /* block SIGALRM and save current signal mask */
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    alarm(nsecs);

    suspmask = oldmask;
    sigdelset(&suspmask, SIGALRM);  /* delete SIGALRM */
    sigsuspend(&suspmask);          /* unblock SIGALRM, and wait for signal */

    /* some signal has been caught, SIGALRM is now blocked */

    unslept = alarm(0);
    sigaction(SIGALRM, &oldact, NULL);
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    return unslept;
}

int main(int argc, char *argv[])
{
    mySleep(5);
    return 0;
}
