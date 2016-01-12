#include "myLinux.h"
#include "common.h"

#define MAXLINE 80
#define SERV_PORT 8000
// #define SERV_IP "192.168.182.129"
// #define SERV_IP "115.159.54.250"
#define SERV_IP "0.0.0.0"

/* 打印选项 */
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

    /* 连接到服务器 */
    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    printf("Enter your name(20 Bytes at most): ");
    scanf("%s", name);
    sprintf(buf, "%d %s %s", SIGNIN, name, "\0");
    Write(sockfd, buf, strlen(buf) + 1);

    while (1) {
        printMenu();
        scanf("%d", &id);
        /* 输入0则退出程序 */
        if (id == 0) {
            printf("Bye bye.\n");
            exit(0);
            /* 输入1则打印登录列表 */
        } else if (id == 1) {
            sprintf(buf, "%d %s", LISTUSER, name);
            Write(sockfd, buf, strlen(buf) + 1);
            n = Read(sockfd, buf, sizeof (buf));
            if (n == 0)
                printf("the other side closed. id = 1\n");
            buf[n] = '\0';
            printf("List:\n %s\n", buf);
            sleep(3);
            /* 开始聊天，ctrl-d退出聊天窗口 */
        } else if (id == 2) {
            printf("enter the name to talk: ");
            scanf("%s", name2);
            printf("enter message to talk: \n");
            static char str[1024];
            while (fgets(str, sizeof (str), stdin)) {
                if (*str == '\n') continue;
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
    Close(sockfd);
    return 0;
}
