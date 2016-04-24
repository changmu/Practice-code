#include "utility.h"

int main(int argc, char *argv[])
{
    // server ip + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = 0;

    // create socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) { errExit("listener"); }
    printf("listen socket created.\n");

    // bind address
    if (bind(listener, (struct sockaddr *) &serverAddr, sizeof (serverAddr)) < 0) {
        errExit("bind error");
    }

    // listen
    int ret = listen(listener, 5);
    if (ret < 0) { errExit("listen error"); }
    printf("Start to listen: %s\n", SERVER_IP);

    // create event list in kernel
    int epFd = epoll_create(EPOLL_SIZE);
    if (epFd < 0) { errExit("epFd error"); }
    printf("epoll created, epollFd = %d.\n", epFd);
    static struct epoll_event events[EPOLL_SIZE];
    // add events in the list
    addFd(epFd, listener, true);

    // main while
    while (1) {
        // epoll_events_count means readiness num
        int epoll_events_count = epoll_wait(epFd, events, EPOLL_SIZE, -1);
        if (epoll_events_count < 0) { errExit("epoll failure"); }
        printf("epoll_events_count = %d\n", epoll_events_count);
        // handle these epoll_events_count events
        for (int i = 0; i < epoll_events_count; ++i) {
            int sockFd = events[i].data.fd;
            // new client
            if (sockFd == listener) {
                struct sockaddr_in client_address;
                socklen_t client_addrLength = sizeof (client_address);
                int clientFd = accept(listener, (struct sockaddr *) &client_address, &client_addrLength);
                printf("client connection from %s:%d, clientFd = %d\n", 
                        inet_ntoa(client_address.sin_addr),
                        ntohs(client_address.sin_port),
                        clientFd
                      );
                addFd(epFd, clientFd, true);

                // save user's connection in list
                clients_list.push_back(clientFd);
                printf("Add new clientFd = %d to epoll\n", clientFd);
                printf("Now there are %d clients in the chatting room\n", (int) clients_list.size());

                // server sends welcome to new user
                printf("Welcome message\n");
                char message[BUF_SIZE];
                memset(message, 0, BUF_SIZE);
                sprintf(message, SERVER_WELCOME, clientFd);
                int ret = send(clientFd, message, BUF_SIZE, 0);
                if (ret < 0) { errExit("send error"); }
            } else {
                // handle message from user
                int ret = sendBroadcastMessage(sockFd);
                if (ret < 0) { errExit("error"); }
            }
        }
    }
    close(listener);
    close(epFd);
    return 0;
}
