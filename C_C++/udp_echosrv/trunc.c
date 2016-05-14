#include "myLinux.h"

#define SERVER_PORT 8086

void echo_srv(int sock)
{
        char recvbuf[1024];
        struct sockaddr_in peeraddr;
        socklen_t peerlen;
        int n;
        while (1) {
                peerlen = sizeof(peeraddr);
                memset(recvbuf, 0, sizeof(recvbuf));

                n = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *) &peeraddr, &peerlen);
                if (n == -1) {
                        if (errno == EINTR)
                                continue;
                        ERR_EXIT("recvfrom");
                } else if (n > 0) {
                        fputs(recvbuf, stdout);
                        sendto(sock, recvbuf, n, 0, (struct sockaddr *) &peeraddr, peerlen);

                }
        }

        close(sock);
}

int main(int argc, char *argv[])
{
        int ret;

        int sock;
        sock = socket(PF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
                ERR_EXIT("socket");

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        ret = bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));
        if (ret < 0)
                ERR_EXIT("bind");

        char recvbuf[1024] = "ABCD";
        sendto(sock, recvbuf, strlen(recvbuf), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));


        int i;
        for (i = 0; i < 4; ++i) {
                int n;
                n = recvfrom(sock, recvbuf, 1, 0, NULL, NULL);
                if (n == -1) {
                        if (errno == EINTR)
                                continue;
                        ERR_EXIT("recvfrom");
                } else if (n > 0) {
                        printf("n = %d, %c\n", n, recvbuf[0]);
                }
        }

        // echo_srv(sock);

        return 0;
}
