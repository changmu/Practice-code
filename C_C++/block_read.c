#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char buf[1024];
    ssize_t n;

    if ((n = read(STDIN_FILENO, buf, 10)) < 0) {
        perror("err");
        exit(-1);
    }

    write(STDOUT_FILENO, buf, n);

    return 0;
}
