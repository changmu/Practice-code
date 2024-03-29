#include "myLinux.h"

#define SERVER_IP "182.118.163.214"
#define SERVER_PORT 11111

void echo_cli(int sock)
{
        int ret;

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

        connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));

        char sendbuf[1024] = {0};
        char recvbuf[1024] = {0};
        // while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
                while (1) {
                // int ret = sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                // if (ret < 0)
                        // ERR_EXIT("sendto");
                
                
                for (ret = 0; ; ++ret) {
                        sprintf(sendbuf, "%d", ret);
                        printf("%s\n", sendbuf);
                        sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                }
/*
                ret = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
                if (ret < 0)
                        ERR_EXIT("recvfrom");
                
                fputs(recvbuf, stdout);
                */
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
