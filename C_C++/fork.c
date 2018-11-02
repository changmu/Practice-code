// hello
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int main()
{
    if (fork()) {
        wait(NULL);
        printf("This is father.\n");
        exit(0);
    } else {
        execl("/bin/date", "date", NULL);
        printf("This is child.\n");
        exit(0);
    }
    return 0;
}
