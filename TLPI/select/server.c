#include "myLinux.h"

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc, char *argv[])
{
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    socklen_t cliaddr_len;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    Listen(listenfd, 20);

    maxfd = listenfd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; ++i)
        client[i] = -1;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while (1) {
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0)
            errExit("select error");
        if (FD_ISSET(listenfd, &rset)) {
            cliaddr_len = sizeof (cliaddr);
            connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &cliaddr_len);

            printf("received from %s:%d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof (str)),
                    ntohs(cliaddr.sin_port));
            for (i = 0; i < FD_SETSIZE; ++i)
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            if (i == FD_SETSIZE)
                errExit("too many clients");
            
            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;

            if (!--nready)
                continue;
        }

        for (i = 0; i <= maxi; ++i) {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                    /* 当client关闭连接时，服务器也关闭对应连接 */
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    int j;
                    for (j = 0; j < n; ++j)
                        buf[j] = toupper(buf[j]);
                    Write(sockfd, buf, n);
                }
                if (!--nready)
                    break;
            }
        }
    }
    Close(listenfd);
    return 0;
}
