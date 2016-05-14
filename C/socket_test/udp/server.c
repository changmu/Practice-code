#include <myLinux.h>

#define MAXLINE 1500

int main(int argc, char *argv[])
{
    int sockfd, i;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[MAXLINE];
    char ipstr[INET_ADDRSTRLEN];
    int len;
    socklen_t clientlen;

    /* 构造用于UDP通信的套接字 */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serveraddr, sizeof (serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(8000);

    bind(sockfd, (struct sockaddr *) &serveraddr, sizeof (serveraddr));

    while (1) {
        clientlen = sizeof (clientaddr);
        len = recvfrom(sockfd, buf, sizeof (buf), 0, (struct sockaddr *) &clientaddr, &clientlen);

        printf("client IP %s\tPORT%d\n", 
                inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipstr, sizeof (ipstr)),
                ntohs(clientaddr.sin_port));

        for (i = 0; i < len; ++i) {
            buf[i] = toupper(buf[i]);
        }
        buf[i] = '\0';

        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &clientaddr, sizeof (clientaddr));
    }

    close(sockfd);

    return 0;
}
