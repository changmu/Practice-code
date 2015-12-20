#include <myLinux.h>

int main(int argc, char **argv)
{
    pid_t pid;
    const char *message;
    int n;
    
    pid = fork();
    if (pid < 0) errExit("fork");

    if (pid == 0) {
        message = "This is the child.\n";
        n = 6;
    } else {
        message = "This is the parent.\n";
        n = 3;
    }

    while (n--) {
        printf(message);
        sleep(1);
    }

    return 0;
}
