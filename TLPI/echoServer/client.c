#include <myLinux.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086

/*
struct packet {
    int len;
    char buf[1024];
};
*/

int main(int argc, char *argv[])
{
    int ret;

    int sock;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) { ERR_EXIT("socket"); }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    ret = connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (ret < 0) { ERR_EXIT("connect"); }

    struct sockaddr_in localaddr;
    socklen_t addrlen = sizeof(localaddr);
    ret = getsockname(sock, (struct sockaddr *) &localaddr, &addrlen);
    if (ret < 0) { ERR_EXIT("getsockname"); }
    printf("Client: %s:%hu\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));

    struct sockaddr_in peeraddr;
    addrlen = sizeof(peeraddr);
    ret = getpeername(sock, (struct sockaddr *) &peeraddr, &addrlen);
    if (ret < 0) { ERR_EXIT("getsockname"); }
    printf("Server: %s:%hu\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
        writen(sock, sendbuf, strlen(sendbuf));

        ret = readline(sock, recvbuf, sizeof(recvbuf));
        if (ret == -1) { ERR_EXIT("readline"); }
        else if (ret == 0) {
            printf("server closed.\n");
            break;
        } 

        printf("recv: %s", recvbuf);
        memset(recvbuf, 0, sizeof(recvbuf));
        memset(sendbuf, 0, sizeof(sendbuf));
    }
    close(sock);

    return 0;
}
