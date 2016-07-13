extern "C" {
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>    

#include "common.h"
}

#include <vector>
#include <algorithm>

char *ip_ptr = "0.0.0.0";
char *port_ptr = "8086";
char *root_ptr = "/home/";


ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while (1) {
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        if (ret == -1 && errno == EINTR) { continue; }
        return ret;
    }   
    return -1; 
}

ssize_t writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwritten;
    char *bufp = (char *) buf;

    while (nleft > 0) {
        nwritten = write(fd, bufp, nleft);
        if (nwritten < 0) {
            if (errno == EINTR) { continue; }
            return -1; 
        } else if (nwritten == 0) { continue; }

        bufp += nwritten;
        nleft -= nwritten;
    }   

    return count;
}

ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nread;
    char *bufp = (char *) buf;

    while (nleft > 0) {
        nread = read(fd, bufp, nleft);
        if (nread < 0) {
            if (errno == EINTR) { continue; }
            return -1;
        } else if (nread == 0) { return count - nleft; }

        bufp += nread;
        nleft -= nread;
    }

    return count;
}

ssize_t readline(int sockfd, void *buf, size_t maxlen)
{
    int ret;
    int nread;
    char *bufp = static_cast<char *>(buf);
    int nleft = maxlen;

    while (1) {
        ret = recv_peek(sockfd, bufp, nleft);
        if (ret <= 0) { return ret; }

        nread = ret;
        int i;
        for (i = 0; i < nread; ++i) {
            if (bufp[i] == '\n') {
                ret = readn(sockfd, bufp, i + 1);
                if (ret != i + 1) { exit(EXIT_FAILURE); }
                bufp[ret] = '\0';
                return ret;
            }
        }

        if (nread > nleft) { exit(EXIT_FAILURE); }

        nleft -= nread;
        ret = readn(sockfd, bufp, nread);
        if (ret != nread) { exit(EXIT_FAILURE); }

        bufp += nread;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    // getoptions();

    int listenfd;
    struct sockaddr_in servaddr;
    int ret;

    /* 配置serveraddr */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        ERR_EXIT("socket");

    ret = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons((unsigned short) atoi(port_ptr));
    servaddr.sin_addr.s_addr = inet_addr(ip_ptr);

    ret = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (ret < 0)
        ERR_EXIT("bind");

    ret = listen(listenfd, SOMAXCONN);
    if (ret < 0)
        ERR_EXIT("listen");



    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int conn;
    int nready;
    int count = 0;

    using EventList = std::vector<struct epoll_event>;

    int epollfd;
    struct epoll_event event;
    EventList events(16);
    std::vector<int> clients;

    epollfd = epoll_create1(EPOLL_CLOEXEC);

    event.data.fd = listenfd;
    event.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

    while (true) {
        nready = epoll_wait(epollfd, events.data(), events.size(), -1);
        if (nready == -1) {
            if (errno == EINTR)
                continue;
            else
                ERR_EXIT("epoll_wait");
        } else if (nready == 0)
            continue;

        if ((size_t) nready == events.size())
            events.resize(events.size() * 2);

        for (int i = 0; i < nready; ++i) {
            if (events[i].data.fd == listenfd) {
                peerlen = sizeof(peeraddr);
                conn = accept4(listenfd, (struct sockaddr *) &peeraddr, &peerlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
                if (conn == -1)
                    ERR_EXIT("accept4");

                printf("client from [%s:%hu]\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

                event.data.fd = conn;
                event.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, conn, &event);
                clients.push_back(conn);
            } else if (events[i].events & EPOLLIN) {
                conn = events[i].data.fd;

                static char recvbuf[1024];
                bzero(recvbuf, sizeof(recvbuf));

                ret = readline(conn, recvbuf, sizeof(recvbuf) - 1);
                if (ret == -1)
                    ERR_EXIT("readline");
                if (ret == 0) {
                    printf("client closed.\n");
                    close(conn);

                    event = events[i];
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, conn, &event);
                    clients.erase(std::remove(clients.begin(), clients.end(), conn), clients.end());
                } else {
                    fputs(recvbuf, stdout);
                }
            }
        }
    }

    return 0;
}