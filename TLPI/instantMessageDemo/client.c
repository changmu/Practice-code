#include "myLinux.h"
#include "common.h"

#define MAXLINE 80
#define SERV_PORT 8000
#define SERV_IP "192.168.182.129"

void printMenu()
{
    system("clear");
    printf("1. list users\n");
    printf("2. chat\n");
    printf("0. exit\n");
    printf("select: ");
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    char name[21];
    char name2[21];
    int sockfd, n, id;

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr);

    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    printf("Enter your name(20 Bytes at most): ");
    scanf("%s", name);
    sprintf(buf, "%d %s %s", SIGNIN, name, "\0");
    Write(sockfd, buf, strlen(buf) + 1);

    while (1) {
        printMenu();
        scanf("%d", &id);
        if (id == 0) {
            printf("Bye bye.\n");
            exit(0);
        } else if (id == 1) {
            sprintf(buf, "%d %s", LISTUSER, name);
            printf("buf1 is %s\n", buf);
            Write(sockfd, buf, strlen(buf) + 1);
            printf("buf2 is %s\n", buf);
            n = Read(sockfd, buf, sizeof (buf));
            if (n == 0)
                printf("the other side closed. id = 1\n");
            buf[n] = '\0';
            printf("buf is %s\n", buf);
            sleep(10);
        } else if (id == 2) {
            printf("enter the name to talk: ");
            scanf("%s", name2);
            printf("enter message to talk: \n");
            static char str[1024];
            while (fgets(str, sizeof (str), stdin)) {
                sprintf(buf, "%d %s %s %s", MSG, name, name2, str);
                Write(sockfd, buf, strlen(buf) + 1);
                n = Read(sockfd, buf, sizeof (buf));
                if (n == 0)
                    printf("the other side closed.\n");
                buf[n] = '\0';
                puts(buf);
            }
        }
    }
    /*
       while (fgets(buf, MAXLINE, stdin)) {
       Write(sockfd, buf, strlen(buf));
       n = Read(sockfd, buf, sizeof (buf));
       if (n == 0)
       printf("the other side closed.\n");
       else
       Write(STDOUT_FILENO, buf, n);
       }
       */
    Close(sockfd);
    return 0;
}
