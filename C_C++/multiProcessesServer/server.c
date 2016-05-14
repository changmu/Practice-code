#include "myLinux.h"
#include "wrap_socket.h"

#define MAXLINE 80
#define SERV_PORT 8000

void do_sigchild(int num)
{
    waitpid(0, NULL, WNOHANG);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;
    pid_t pid;

    struct sigaction newact;
    newact.sa_handler = do_sigchild;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGCHLD, &newact, NULL);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    Listen(listenfd, 20);

    printf("Server %s:%d is already for connecting...\n",
            inet_ntop(AF_INET, &servaddr.sin_addr, str, sizeof (str)),
            ntohs(servaddr.sin_port)
          );
    while (1) {
        cliaddr_len = sizeof (cliaddr);
        connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &cliaddr_len);

        if (fork() == 0) {
            Close(listenfd);
            while (1) {
                n = Read(connfd, buf, MAXLINE);
                if (n == 0) {
                    printf("the other side has been closed.\n");
                }
                printf("Received from %s at port %d\n",
                        inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof (str)),
                        ntohs(cliaddr.sin_port)
                      );
                n = strlen(buf);
                for (i = 0; i < n; ++i)
                    buf[i] = toupper(buf[i]);
                Write(connfd, buf, n);
            }
            Close(connfd);
            return 0;
        }
        Close(connfd);
    }
    return 0;
}
