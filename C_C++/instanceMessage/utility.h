#pragma once

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xffff
#define SERVER_WELCOME "Welcome to join the chatting room! Your chat ID is: Client #%d"
#define SERVER_MESSAGE "ClientID %d say >> %s"
#define EXIT "EXIT"
#define CAUTION "There is only one in the chatting room!"

#define errExit(str) \
    do { \
        perror(str); \
        exit(-1); \
    } while (0)

// save all the clients' socket
list<int> clients_list;

int setNonblocking(int sockFd)
{
    fcntl(sockFd, F_SETFL, fcntl(sockFd, F_GETFL, 0) | O_NONBLOCK);
    return 0;
}

void addFd(int epollFd, int fd, bool enable_et)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (enable_et) {
        ev.events |= EPOLLET;
    }
    epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev);
    setNonblocking(fd);
    printf("fd added to epoll!\n\n");
}

// return len
int sendBroadcastMessage(int clientFd)
{
    // buf receive new message
    // message save format
    char buf[BUF_SIZE], message[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    memset(message, 0, BUF_SIZE);

    // receive message
    printf("read from client(clientID:%d)\n", clientFd);
    int len = recv(clientFd, buf, BUF_SIZE, 0);

    if (len == 0) {
        // client closed connection
        close(clientFd);
        clients_list.remove(clientFd);
        printf("ClientID:%d closed.\n now there are %d clients in the chatting room\n", clientFd, (int)clients_list.size());
    } else {
        // broadcast message
        if (clients_list.size() == 1) {
            send(clientFd, CAUTION, strlen(CAUTION), 0);
            return len;
        }
        // format message to broadcast
        sprintf(message, SERVER_MESSAGE, clientFd, buf);

        for (auto it = clients_list.begin(); it != clients_list.end(); ++it) {
            if (*it != clientFd) {
                if (send(*it, message, BUF_SIZE, 0) < 0) {
                    errExit("error");
                }
            }
        }
    }
    return len;
}
