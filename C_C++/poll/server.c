#include "myLinux.h"

#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024

int main(int argc, char *argv[])
{
    int i, j, maxi, listenfd, connfd, sockfd, nready;
    ssize_t n;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    Listen(listenfd, 20);

    client[0].fd = listenfd;
    /* 监听普通读事件 */
    client[0].events = POLLRDNORM;

    for (i = 1; i < OPEN_MAX; ++i)
        client[i].fd = -1;
    /* client[]中有效元素的最大下标 */
    maxi = 0;

    while (1) {
        nready = poll(client, maxi + 1, -1); // 阻塞

        /* 有客户端连接请求 */
        if (client[0].revents & POLLRDNORM) {
            clilen = sizeof (clilen);
            connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
            printf("received from %s:%d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof (str)),
                    ntohs(cliaddr.sin_port));
            for (i = 1; i < OPEN_MAX; ++i)
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            if (i == OPEN_MAX)
                errExit("too many clients");
            client[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;

            /* 没有更多就绪事件时，继续回到poll阻塞 */
            if (!--nready)
                continue;
        }
        
        for (i = 1; i <= maxi; ++i) {
            if ((sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = Read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) { // 收到RST标志
                        printf("client[%d] aborted connection\n", i);
                        Close(sockfd);
                        client[i].fd = -1;
                    } else
                        errExit("read error");
                } else if (n == 0) {
                    printf("client[%d] closed connection\n", i);
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
                    for (j = 0; j < n; ++j)
                        buf[j] = toupper(buf[j]);
                    Writen(sockfd, buf, n);
                }
                if (!--nready)
                    break;
            }
        }
    }
    return 0;
}
