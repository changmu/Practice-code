#include <myLinux.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086

struct packet {
    int len;
    char buf[1024];
};

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

    ret = connect(sock, (struct sockaddr *) &servaddr, sizeof (servaddr));
    if (ret < 0) { ERR_EXIT("connect"); }

    struct packet sendbuf;
    struct packet recvbuf;
    memset(&sendbuf, 0, sizeof (sendbuf));
    memset(&recvbuf, 0, sizeof (recvbuf));
    int n;
    while (fgets(sendbuf.buf, sizeof (sendbuf.buf), stdin)) {
        n = strlen(sendbuf.buf);
        sendbuf.len = htonl(n);
        writen(sock, &sendbuf, 4 + n);


        ret = readn(sock, &recvbuf.len, sizeof(int));
        if (ret == -1) { ERR_EXIT("read"); }
        else if (ret < 4) {
            printf("client closed.\n");
            break;
        } 
        n = ntohl(recvbuf.len);
        ret = readn(sock, recvbuf.buf, n);
        if (ret == -1) { ERR_EXIT("read"); }
        else if (ret < n) {
            printf("client closed.\n");
            break;
        } 


        printf("%s", recvbuf.buf);
        memset(&recvbuf, 0, sizeof(recvbuf));
        memset(&sendbuf, 0, sizeof(sendbuf));
    }
    close(sock);

    return 0;
}
