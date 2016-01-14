#include "myLinux.h"

#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024

int main(int argc, char *argv[])
{
    int i, j, maxi, listenfd, connfd, sockfd;
    int nready, efd, res;
    ssize_t n;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t clilen;
    int client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;
    struct epoll_event tep, ep[OPEN_MAX];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    Listen(listenfd, 20);

    for (i = 0; i < OPEN_MAX; ++i)
        client[i] = -1;
    maxi = -1;

    efd = epoll_create(OPEN_MAX);
    if (efd == -1)
        errExit("epoll_create");

    tep.events = EPOLLIN;
    tep.data.fd = listenfd;
    res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
    if (res == -1)
        errExit("epoll_ctl");

    for ( ; ; ) {
        /* 阻塞监听 */
        nready = epoll_wait(efd, ep, OPEN_MAX, -1);
        if (nready == -1)
            errExit("epoll_wait");
        for (i = 0; i < nready; ++i) {
            if (!(ep[i].events & EPOLLIN))
                continue;
            if (ep[i].data.fd == listenfd) {
                clilen = sizeof (cliaddr);
                connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
                printf("Received from %s:%d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof (str)),
                        ntohs(cliaddr.sin_port));
                for (j = 0; j < OPEN_MAX; ++j)
                    if (client[j] < 0) {
                        client[j] = connfd;
                        break;
                    }
                if (j == OPEN_MAX)
                    errExit("too many clients");

                if (j > maxi)
                    maxi = j;
                tep.events = EPOLLIN;
                tep.data.fd = connfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
                if (res == -1)
                    errExit("epoll_ctl");
            } else {
                sockfd = ep[i].data.fd;
                n = Read(sockfd, buf, MAXLINE);
                if (n == 0) {
                    for (j = 0; j <= maxi; ++j) {
                        if (client[j] == sockfd) {
                            client[j] = -1;
                            break;
                        }
                    }
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    if (res == -1)
                        errExit("epoll_ctl");
                    Close(sockfd);
                    printf("client[%d] closed connection\n", j);
                } else {
                    for (j = 0; j < n; ++j)
                        buf[j] = toupper(buf[j]);
                    Writen(sockfd, buf, n);
                }
            }
        }
    }

    close(listenfd);
    close(efd);
    return 0;
}
