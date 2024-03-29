#include "myLinux.h"

#define MAXLINE 80
// #define SERV_IP "192.168.182.129"
#define SERV_IP "115.159.54.250"
#define SERV_PORT 8000

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    int sockfd, n;

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    while (fgets(buf, sizeof (buf), stdin)) {
        Write(sockfd, buf, strlen(buf));
        n = Read(sockfd, buf, MAXLINE);
        if (n == 0)
            printf("the other side has been closed.\n");
        else
            Write(STDOUT_FILENO, buf, n);
    }
    Close(sockfd);
    return 0;
}
