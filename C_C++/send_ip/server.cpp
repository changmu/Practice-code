#include <stdio.h>
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
    
    int listenFd = socket(AF_INET, SOCK_DGRAM, 0);
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

    int sock;
    sockaddr_in peerAddr;
    while (1) {
        NO_EINTR(ret = read(listenFd, buf, sizeof buf));
        printf("msg: %s", buf);
    }

    return 0;
}
