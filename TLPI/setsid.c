#include <myLinux.h>

int main(int argc, char *argv[])
{
    if (fork() == 0) {
        printf("child process PID is %d\n", getpid());
        printf("Group ID of child is %d\n", getpgid(0));
        printf("Session ID of child is %d\n", getsid(0));
        sleep(2);
        setsid();

        printf("After changed!\nchild process PID is %d\n", getpid());
        printf("Group ID of child is %d\n", getpgid(0));
        printf("Session ID of child is %d\n", getsid(0));
        sleep(20);
    }
    return 0;
}
