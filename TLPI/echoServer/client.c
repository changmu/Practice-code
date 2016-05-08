#include <myLinux.h>

#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[])
{
    int ret;

    int sock;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) { ERR_EXIT("socket"); }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    ret = connect(sock, (struct sockaddr *) &servaddr, sizeof (servaddr));
    if (ret < 0) { ERR_EXIT("connect"); }

    char sendbuf[1024];
    char recvbuf[1024];
    while (fgets(sendbuf, sizeof (sendbuf), stdin)) {
        write(sock, sendbuf, strlen(sendbuf));
        read(sock, recvbuf, sizeof (recvbuf));

        printf("%s", recvbuf);
        memset(recvbuf, 0, sizeof (recvbuf));
        memset(sendbuf, 0, sizeof (sendbuf));
    }
    close(sock);

    return 0;
}
