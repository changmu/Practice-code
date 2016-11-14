#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <sys/epoll.h>

#define FILENAME "./ip.txt"
#define BLOCK_SIZE 10240
#define NO_EINTR(stmt) while ((stmt) == -1 && errno == EINTR);

int main(int argc, char *argv[])
{
    // daemon(1, 1);
    char buf[BLOCK_SIZE] = { 0 };
    
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenFd != -1);

    int flag = 1;
    int len = sizeof(flag);
    int ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &flag, len);
    assert(ret != -1);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(8086);
    socklen_t addr_len = sizeof(addr);

    ret = bind(listenFd, (struct sockaddr *) &addr, addr_len);
    assert(ret != -1);

    listen(listenFd, 5);

    int sock;
    sockaddr_in peerAddr;
    while (1) {
        sock = accept(listenFd, (struct sockaddr *) &addr, &addr_len);

        static __thread char buf[40];
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);

        int ret = getpeername(sock, (struct sockaddr *) &addr, &addr_len);
        inet_ntop(AF_INET, &addr.sin_addr, buf, addr_len);
        printf("client ip: %s\n", buf);
        sleep(1);
    }

    return 0;
}
