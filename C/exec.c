#include <stdio.h>
#include <unistd.h>

int main()
{
    /*
    if (fork() > 0) {
        printf("I am parent.\n");
        // execlp("ps", "ps", "aux", NULL);
    } else {
        // execl("~/C/app", "app", NULL);
        printf("I am child.\n");
    }
    */
    int n = 10;
    while (n--) {
        if (fork() == 0) while (1);
    }
    return 0;
}
