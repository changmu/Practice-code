#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // for bzero

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    int var1, var2, sum;
    int sock;
    int ret;
    struct sockaddr_in servaddr;
    char buf[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    ret = connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (ret == -1)
        ERR_EXIT("connect");


    printf("input two integer:\n");
    while (scanf("%d%d", &var1, &var2) == 2) {
        sprintf(buf, "%d %d", var1, var2);

        ret = write(sock, buf, strlen(buf));
        if (ret == -1)
            ERR_EXIT("write");

        bzero(buf, sizeof(buf));
        ret = read(sock, buf, sizeof(buf));
        if (ret == -1)
            ERR_EXIT("read");

        sscanf(buf, "%d", &sum);
        printf("sum is: %d\n", sum);
    }

    return 0;
}