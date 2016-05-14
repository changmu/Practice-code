#include "myLinux.h"

#define MAXLINE 80
#define SERV_PORT 8000

struct s_info {
    struct sockaddr_in cliaddr;
    int connfd;
};

void *do_work(void *arg)
{
    int i, n;
    struct s_info *ts = (struct s_info *) arg;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    pthread_detach(pthread_self());

    while (1) {
        n = Read(ts->connfd, buf, MAXLINE);
        if (n == 0) {
            printf("the other side has been closed.\n");
            break;
        }
        printf("received from %s:%d\n",
                inet_ntop(AF_INET, &ts->cliaddr.sin_addr, str, sizeof (str)),
                ntohs((*ts).cliaddr.sin_port)
                );

        for (i = 0; i < n; ++i)
            buf[i] = toupper(buf[i]);
        Write(ts->connfd, buf, n);
    }
    Close(ts->connfd);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    int i = 0;
    pthread_t tid;
    struct s_info ts[383];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    Listen(listenfd, 20);

    printf("Accepting connections ...\n");
    while (1) {
        cliaddr_len = sizeof (cliaddr);
        connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &cliaddr_len);
        ts[i].cliaddr = cliaddr;
        ts[i].connfd = connfd;
        pthread_create(&tid, NULL, do_work, (void *) &ts[i]);
    }
    return 0;
}
