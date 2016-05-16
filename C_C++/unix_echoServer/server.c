#include "myLinux.h"

#define SOCK_PATH "/tmp/test_socket"

void echo_srv(int conn)
{
        char recvbuf[1024];
        int n;
        while (1) {
                memset(&recvbuf, 0, sizeof(recvbuf));
                n = read(conn, recvbuf, sizeof(recvbuf));
                if (n == -1) {
                        if (errno == EINTR)
                                continue;
                        ERR_EXIT("read");
                } else if (n == 0) {
                        printf("client closed.\n");
                        break;
                }

                fputs(recvbuf, stdout);
                write(conn, recvbuf, sizeof(recvbuf));
        }

        close(conn);
}

int main(int argc, char *argv[])
{
        int listenfd = socket(PF_UNIX, SOCK_STREAM, 0);
        if (listenfd < 0)
                ERR_EXIT("socket");

        unlink(SOCK_PATH);
        struct sockaddr_un servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sun_family = AF_UNIX;
        strcpy(servaddr.sun_path, SOCK_PATH);

        int ret = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
        if (ret < 0)
                ERR_EXIT("bind");

        ret = listen(listenfd, SOMAXCONN);
        if (ret < 0)
                ERR_EXIT("listen");

        int conn;
        pid_t pid;
        while (1) {
                conn =accept(listenfd, NULL, NULL);
                if (conn == -1) {
                        if (conn == EINTR)
                                continue;
                        ERR_EXIT("accept");
                }

                pid = fork();
                if (pid == -1)
                        ERR_EXIT("fork");

                if (pid == 0) {
                        close(listenfd);
                        echo_srv(conn);
                        exit(EXIT_SUCCESS);
                } else {
                        close(conn);
                }
        }

        return 0;
}
