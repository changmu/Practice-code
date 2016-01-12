/* select可以用来实现多路I/O复用，然而能支持的客户端连接数却不能超过1024，
 * 但是这个数字对于小型局域网是够用的。于是这个即时通讯软件就采用select来实现了。
 */

/* 需要的头文件都包裹在这个头文件里 */
#include "myLinux.h"
#include "common.h"

/* 设置能接受的应用层数据包最大长度 */
#define MAXLINE 1024
/* 服务器开放端口 */
#define SERV_PORT 8000

struct login_t {
    int fd;
    char *name;
} usr[1000];

char *getUsrList()
{
    static char buf[1024];
    char *str = buf;
    int i, n;

    str[0] = '\0';
    for (i = 0; i < 1000; ++i) {
        if (usr[i].fd > 0) {
            n = sprintf(str, "%s\n", usr[i].name);
            str += n;
        } else
            break;
    }

    return buf;
}

int findByName(const char *name)
{
    int i;
    for (i = 0; i < 1000; ++i) {
        if (usr[i].fd == 0) return -1;
        if (!strcmp(name, usr[i].name))
            return i;
    }
}

void signinName(const char *name, int fd)
{
    int i;
    for (i = 0; i < 1000; ++i) {
        if (usr[i].fd == 0) {
            usr[i].fd = fd;
            usr[i].name = malloc(strlen(name) + 1);
            strcpy(usr[i].name, name);
            return;
        }
    }
}

void parse_data(int sockfd, char *buf)
{
    char from[21], to[21];
    int kind, n, i;
    int fd;
    static char str[1024];
    static char str2[1024];
    sscanf(buf, "%d%n", &kind, &n);
    buf += n;

    switch (kind) {
    case SIGNIN:
        sscanf(buf, "%s", from);
        signinName(from, sockfd);
        break;
    case MSG:
        sscanf(buf, "%s %s %n", from, to, &n);
        fd = findByName(to);
        sprintf(str, "%s %s", from, buf + n);
        Write(fd, str, strlen(str) + 1);
        break;
    case LISTUSER:
        sprintf(str, "%s", getUsrList());
        Write(sockfd, str, strlen(str) + 1);
        break;
    }
}

int main(int argc, char *argv[])
{
    /* 下面这些变量的用途用的时候再解释 */
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    socklen_t cliaddr_len;
    struct sockaddr_in cliaddr, servaddr;

    /* 定义一个TCP/IPv4的socket, 自定义库函数对socket函数进行了一层包装，
     * 包含了常见的异常处理。
     */
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    /* 对服务器绑定的IP地址及端口号进行初始化 */
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET; // 设置地址族为IPv4
    servaddr.sin_port = htons(SERV_PORT); // 将本地指定端口号转换成网络序
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听本地所有网卡接口

    /* 绑定地址，端口到前面定义的socket上 */
    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    /* 设置三次握手排队队列为20，最大为128 */
    Listen(listenfd, 20);

    /* maxfd是当前进程已打开的文件描述符最大值，maxi是监听队列里最后一个有事件响应的文件描述符对应的下标，没有它
     * 程序也能运行，但是有的话跑得更快
     */
    maxfd = listenfd;
    maxi = -1;

    /* FD_SETSIZE是select能监听的最大文件描述符个数；初始化响应队列 */
    for (i = 0; i < FD_SETSIZE; ++i)
        client[i] = -1;

    /* 设置文件描述符集合为空 */
    FD_ZERO(&allset);
    /* 添加listenfd描述符到该集合 */
    FD_SET(listenfd, &allset);

    /* 服务器核心代码 */
    while (1) {
        /* 初始化描述符集合 */
        rset = allset;
        /* 正式阻塞监听所有文件描述符是否有事件发生，监听读事件，写、异常事件置空, 
         * 返回值为有事件发生的文件描述符个数
         */
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        /* 异常处理 */
        if (nready < 0)
            errExit("select error");
        /* 如果是listenfd描述符有读事件产生，说明有新的客户端来连接 */
        if (FD_ISSET(listenfd, &rset)) {
            /* 地址端口结构体的长度初始化 */
            cliaddr_len = sizeof (cliaddr);
            /* 产生新的文件描述符来处理客户端请求 */
            connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &cliaddr_len);

            /* 打印客户端的IP和端口 */
            printf("received from %s:%d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof (str)),
                    ntohs(cliaddr.sin_port));
            /* 将新的客户端描述符添加到处理队列 */
            for (i = 0; i < FD_SETSIZE; ++i)
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            /* 判断处理队列是否已满 */
            if (i == FD_SETSIZE)
                errExit("too many clients");
            
            /* 将新客户端添加到描述符集合 */
            FD_SET(connfd, &allset);
            /* 更新最大描述符值 */
            if (connfd > maxfd)
                maxfd = connfd;
            /* 更新当前处理到的最大下标，下面的for循环要用 */
            if (i > maxi)
                maxi = i;

            /* 事件数减一，若为0则跳过下边继续下次监听，否则到下面继续处理 */
            if (!--nready)
                continue;
        }

        /* maxi记录前边处理的最大下标 */
        for (i = 0; i <= maxi; ++i) {
            /* 当前位置没有文件描述符 */
            if ((sockfd = client[i]) < 0)
                continue;
            /* 查看该文件描述符是否产生读事件 */
            if (FD_ISSET(sockfd, &rset)) {
                /* 读取客户端发送过来的消息 */
                if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                    /* 当client关闭连接时，服务器也关闭对应连接 */
                    Close(sockfd);
                    /* 从描述符集合中删除该客户端 */
                    FD_CLR(sockfd, &allset);
                    /* 该位置腾空 */
                    client[i] = -1;
                } else {
                    /* 正常读取 */
                    int j;
                    /* 接下来解析该数据包 */
                    parse_data(sockfd, buf);
                    printf("test...\n");
                }
                if (!--nready)
                    break;
            }
        }
    }
    Close(listenfd);
    return 0;
}
