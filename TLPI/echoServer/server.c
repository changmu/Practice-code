#include <myLinux.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086

/*
struct packet {
    int len;
    char buf[1024];
};
*/
void do_server(int sock)
{
    char recvbuf[1024];
    static int ret;
    while (1) {
        memset(recvbuf, 0, sizeof(recvbuf));
        ret = readline(sock, recvbuf, 1024);
        if (ret == -1) { ERR_EXIT("readline"); }
        if (ret == 0) {
            printf("client close\n");
            break;
        }
        printf("recv: %s", recvbuf);
        writen(sock, recvbuf, strlen(recvbuf));
    }
}

int main()
{
    int listenfd;
    struct sockaddr_in servaddr;
    int ret;

    if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        ERR_EXIT("socket");
    }

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    // inet_aton(SERVER_IP, &servaddr.sin_addr);

    int on = 1;
    ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret < 0) { ERR_EXIT("setsockopt"); }

    ret = bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    if (ret < 0) { ERR_EXIT("bind"); }

    ret = listen(listenfd, SOMAXCONN);
    if (ret < 0) { ERR_EXIT("listen"); }

    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof (peeraddr);
    int sock;

    pid_t pid;
    while (1) {
        sock = accept(listenfd, (struct sockaddr *) &peeraddr, &peerlen);
        if (sock < 0) { ERR_EXIT("accept"); }
        printf("Client: %s:%hu\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

        pid = fork();
        if (pid < 0) { ERR_EXIT("fork"); }

        if (pid == 0) { // child
            close(listenfd);
            do_server(sock);
            close(sock);
            exit(EXIT_SUCCESS);
        } else {
            close(sock);
        }
    }
    close(listenfd);

    return 0;
}
