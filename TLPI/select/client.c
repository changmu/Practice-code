#include "myLinux.h"

#define MAXLINE 80
#define SERV_PORT 8000
#define SERV_IP "192.168.182.129"

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    int sockfd, n;

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr);

    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

    while (fgets(buf, MAXLINE, stdin)) {
        Write(sockfd, buf, strlen(buf));
        n = Read(sockfd, buf, sizeof (buf));
        if (n == 0)
            printf("the other side closed.\n");
        else
            Write(STDOUT_FILENO, buf, n);
    }
    Close(sockfd);
    return 0;
}
