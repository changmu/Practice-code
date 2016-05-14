#include "myLinux.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086

void echo_cli(int sock)
{
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

        connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));

        char sendbuf[1024] = {0};
        char recvbuf[1024] = {0};
        while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
                int ret = sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                if (ret < 0)
                        ERR_EXIT("sendto");

                ret = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
                if (ret < 0)
                        ERR_EXIT("recvfrom");
                
                fputs(recvbuf, stdout);
                memset(sendbuf, 0, sizeof(sendbuf));
                memset(recvbuf, 0, sizeof(recvbuf));
        }

        close(sock);
}


int main(int argc, char *argv[])
{
        int sock;
        sock = socket(PF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
                ERR_EXIT("socket");

        echo_cli(sock);

        return 0;
}
