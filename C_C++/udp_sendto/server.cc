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

int main(int argc, char* argv[])
{
    int sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);

    struct sockaddr_in localAddr;
    bzero(&localAddr, sizeof localAddr);
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(8001);
    localAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    // localAddr.sin_addr.s_addr = inet_addr("10.105.25.168");

    int r = bind(sockfd, (struct sockaddr *) &localAddr, sizeof localAddr);
    if (r < 0) {
        fprintf(stderr, "bind err, msg: %s\n", strerror(errno));
        exit(2);
    }

    struct sockaddr_in srcAddr;
    bzero(&srcAddr, sizeof srcAddr);
    socklen_t srcAddrLen = sizeof srcAddr;
    char buf[1024];

    while (true) {
        r = recvfrom(sockfd, buf, sizeof buf, 0, (struct sockaddr *) &srcAddr, &srcAddrLen);
        if (r < 0) {
            fprintf(stderr, "recvfrom err, msg: %s\n", strerror(errno));
            exit(2);
        }
        printf("recv: %s\n", buf);

        printf("ip:%s, port:%hd\n", inet_ntoa(srcAddr.sin_addr), ntohs(srcAddr.sin_port));

        r = sendto(sockfd, buf, sizeof buf, 0, (struct sockaddr *) &srcAddr, srcAddrLen);
        if (r < 0) {
            fprintf(stderr, "sendto err, msg: %s\n", strerror(errno));
            exit(2);
        }

        // sleep(1);
    }

    return 0;
}
