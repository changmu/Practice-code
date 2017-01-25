#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s peer_ip peer_port\n", argv[0]);
        exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);

    struct sockaddr_in peerAddr;
    bzero(&peerAddr, sizeof peerAddr);
    peerAddr.sin_family = AF_INET;
    peerAddr.sin_port = htons(atoi(argv[2]));
    peerAddr.sin_addr.s_addr = inet_addr(argv[1]);

    struct sockaddr_in localAddr;
    bzero(&localAddr, sizeof localAddr);
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(8000);
    localAddr.sin_addr.s_addr = inet_addr("10.105.25.168");
    // localAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    int r = bind(sockfd, (struct sockaddr *) &localAddr, sizeof localAddr);
    if (r < 0) {
        fprintf(stderr, "bind err, msg: %s\n", strerror(errno));
        exit(2);
    }

    struct sockaddr_in srcAddr;
    bzero(&srcAddr, sizeof srcAddr);
    socklen_t srcAddrLen = sizeof srcAddr;
    char buf[1024] = "hello world";

    while (true) {
        r = sendto(sockfd, buf, sizeof buf, 0, (struct sockaddr *) &peerAddr, sizeof peerAddr);
        if (r < 0) {
            fprintf(stderr, "sendto err, msg: %s\n", strerror(errno));
            exit(2);
        }

        sleep(1);

        // std::this_thread::sleep_for(1s);

        r = recvfrom(sockfd, buf, sizeof buf, 0, (struct sockaddr *) &srcAddr, &srcAddrLen);
        if (r < 0) {
            fprintf(stderr, "recvfrom err, msg: %s\n", strerror(errno));
            exit(2);
        }
        printf("recv: %s\n", buf);
    }

    return 0;
}
