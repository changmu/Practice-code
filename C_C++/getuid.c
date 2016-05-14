#include <myLinux.h>

int main(int argc, char **argv)
{
    if (fork()) {
        printf("parent: uid %d, euid %d, pid %d, ppid %d\n", 
                getuid(), geteuid(), getpid(), getppid());
        printf("parent: gid %d, egid %d\n", getgid(), getegid());
    } else {
        printf("child: uid %d, euid %d, pid %d, ppid %d\n", 
                getuid(), geteuid(), getpid(), getppid());
        printf("child: gid %d, egid %d\n", getgid(), getegid());
    }
    return 0;
}
