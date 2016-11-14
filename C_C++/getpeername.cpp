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

#define BLOCK_SIZE 10240
char buf[BLOCK_SIZE];

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8086);
    addr.sin_addr.s_addr = inet_addr("115.159.54.250");

    socklen_t addr_len = sizeof(addr);
    bool isConnected = false;
    int ret;

    while (1) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        ret = connect(sock, (struct sockaddr *) &addr, addr_len);
        if (ret == -1) {
            fprintf(stderr, "connecting remote host...\n");
            sleep(3);
        } else {
            isConnected = true;
        }

        ret = getpeername(sock, (struct sockaddr *) &addr, &addr_len);
        printf("peer ip: %s\n", inet_ntoa(addr.sin_addr));
    }

    return 0;
}
