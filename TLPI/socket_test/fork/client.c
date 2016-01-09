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
    char ipstr[] = "0.0.0.0";
    char buf[1024];

    // 1. create a socket
    confd = socket(AF_INET, SOCK_STREAM, 0);
    // 2. initialize server ip
    bzero(&serveraddr, sizeof (serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    // 3. connect
    connect(confd, (struct sockaddr *) &serveraddr, sizeof (serveraddr));

    while (fgets(buf, sizeof (buf), stdin)) {
        write(confd, buf, strlen(buf));
        len = read(confd, buf, sizeof (buf));
        write(STDOUT_FILENO, buf, len);
    }

    // 4. close socket
    close(confd);

    return 0;
}
