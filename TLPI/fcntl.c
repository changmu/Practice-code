#include <myLinux.h>

#define MSG_TRY "try again\n"

int main()
{
    int n, flags;
    char buf[10];

    flags = fcntl(STDIN_FILENO, F_GETFL);
    flags |= O_NONBLOCK;

    if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(1);
    }

tryagain:
    n = read(STDIN_FILENO, buf, 10);
    if (n < 0) {
        if (errno == EAGAIN) {
            sleep(1);
            write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
            goto tryagain;
        }
        perror("read stdin");
        exit(2);
    }
    write(STDOUT_FILENO, buf, n);

    return 0;
}
