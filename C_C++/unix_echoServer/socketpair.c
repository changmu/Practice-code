#include "myLinux.h"

int main(int argc, char *argv[])
{
        int sockfds[2];
        int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds);
        if (ret < 0)
                ERR_EXIT("socketpair");

        pid_t pid = fork();
        if (pid == -1)
                ERR_EXIT("fork");

        if (pid > 0) {
                int val = 0;
                close(sockfds[1]);
                while (1) {
                        ++val;
                        printf("sending data: %d\n", val);
                        write(sockfds[0], &val, sizeof(val));
                        read(sockfds[0], &val, sizeof(val));
                        printf("data received: %d\n", val);
                        sleep(1);
                }
        } else {
                int val;
                close(sockfds[0]);
                while (1) {
                        read(sockfds[1], &val, sizeof(val));
                        ++val;
                        write(sockfds[1], &val, sizeof(val));
                }
        }

        return 0;
}
