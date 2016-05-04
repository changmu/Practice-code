#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    char *file = "/bin/ls";
    pid_t pid = vfork();
    if (pid == 0) {
        char *argv[] = {"ls", "-l", "--color=auto", NULL};
        execve(file, argv, NULL);
        printf("hello..\n");
    }
    return 0;
}
