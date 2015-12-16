#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MSG_TRY "try again\n"

int main()
{
    char buf[1024];
    int fd, n;

    fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open /dev/tty");
        exit(1);
    }
tryagain:
    n = read(fd, buf, 10);
    if (n < 0) {
        if (errno == EAGAIN) {
            sleep(1);
            write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
            goto tryagain;
        }
        perror("read /dev/tty");
        exit(2);
    }

    write(STDOUT_FILENO, buf, n);
    close(fd);

    return 0;
}
