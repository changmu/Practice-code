#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_PORT 8000

int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr;
    int confd;
    int len;
    char ipstr[] = "192.168.182.129";
    char buf[1024];
    if (argc < 2) {
        printf("./client str\n");
        exit(1);
    }

    // 1. create a socket
    confd = socket(AF_INET, SOCK_DGRAM, 0);
    // 2. initialize server ip
    bzero(&serveraddr, sizeof (serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    // 3. connect
    sendto(confd, argv[1], strlen(argv[1]), 0, (struct sockaddr *) &serveraddr, sizeof (serveraddr));
    len = recvfrom(confd, buf, sizeof (buf), 0, NULL, 0);
    buf[len] = '\0';
    puts(buf);
    // 4. close socket
    close(confd);

    return 0;
}
