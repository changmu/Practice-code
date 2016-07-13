extern "C" {
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // for bzero

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
}

#include <vector>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)


using EventList = std::vector<struct epoll_event>;

int main(int argc, char *argv[])
{
    int listenfd;
    struct sockaddr_in servaddr;
    int ret;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        ERR_EXIT("listenfd");

    int on = 1;
    ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret < 0)
        ERR_EXIT("setsockopt");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    ret = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (ret < 0)
        ERR_EXIT("bind");    

    ret = listen(listenfd, SOMAXCONN);
    if (ret < 0)
        ERR_EXIT("listen");


    int epollfd;
    struct epoll_event event;
    EventList events(16);

    struct sockaddr_in peeraddr;
    socklen_t socklen;
    int sock;
    int nready;


    epollfd = epoll_create1(EPOLL_CLOEXEC);

    event.data.fd = listenfd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);


    while (1) {
        nready = epoll_wait(epollfd, events.data(), static_cast<int>(events.size()) - 1, -1);
        if (nready == -1) {
            if (errno == EINTR)
                continue;
            else
                ERR_EXIT("epoll_wait");
        }

        if (nready == static_cast<int>(events.size()))
            events.resize(events.size() * 2);

        for (int i = 0; i < nready; ++i) {
            if (events[i].data.fd == listenfd) {
                socklen = sizeof(peeraddr);
                sock = accept4(listenfd, (struct sockaddr *) &peeraddr, &socklen, SOCK_NONBLOCK);
                if (sock == -1)
                    ERR_EXIT("sock");

                printf("Client from [%s:%hu]\n", inet_ntoa(peeraddr.sin_addr), 
                        ntohs(peeraddr.sin_port));

                event.data.fd = sock;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &event);
            } else if (events[i].events | EPOLLIN) {
                sock = events[i].data.fd;

                int var1, var2;
                char recvbuf[1024] = { 0 };
                ret = read(sock, recvbuf, sizeof(recvbuf));
                if (ret == -1)
                    ERR_EXIT("read");
                if (ret == 0) {
                    printf("client closed.\n");
                    close(sock);

                    event.data.fd = sock;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, sock, &event);

                    continue;
                }

                printf("receive:[%s]\n", recvbuf);

                sscanf(recvbuf, "%d%d", &var1, &var2);
                char sendbuf[1024] = { 0 };
                sprintf(sendbuf, "%d", var1 + var2);

                ret = write(sock, sendbuf, strlen(sendbuf));
                if (ret == -1)
                    ERR_EXIT("write");
            }
        }        
    }

    return 0;
}