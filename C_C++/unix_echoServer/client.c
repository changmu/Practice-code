#include "myLinux.h"

#define SOCK_PATH "/tmp/test_socket"

void echo_cli(int sock)
{
        char sendbuf[1024];
        char recvbuf[1024];
        while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
                write(sock, sendbuf, strlen(sendbuf));
                read(sock, recvbuf, sizeof(recvbuf));
                fputs(recvbuf, stdout);
                memset(sendbuf, 0, sizeof(sendbuf));
                memset(recvbuf, 0, sizeof(recvbuf));
        }

        close(sock);
}

int main(int argc, char *argv[])
{
        int sock = socket(PF_UNIX, SOCK_STREAM, 0);
        if (sock < 0)
                ERR_EXIT("socket");

        struct sockaddr_un servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sun_family = AF_UNIX;
        strcpy(servaddr.sun_path,SOCK_PATH);

        int ret = connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));
        if (ret < 0)
                ERR_EXIT("connect");

        echo_cli(sock);
        return 0;
}
