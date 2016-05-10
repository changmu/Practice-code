#include <myLinux.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086

int main(int argc, char *argv[])
{
    int ret;

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) { ERR_EXIT("socket"); }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    ret = connect(sock, (struct sockaddr *) &servaddr, sizeof (servaddr));
    if (ret < 0) { ERR_EXIT("connect"); }

    pid_t pid = fork();
    if (pid < 0) { ERR_EXIT("fork"); }

    if (pid != 0) {
        char recvbuf[1024];
        while (1) {
            memset(recvbuf, 0, sizeof (recvbuf));
            ret = readn(sock, recvbuf, sizeof(recvbuf));
            if (ret == -1) { ERR_EXIT("read"); }
            else if (ret == 0) { 
                printf("peer closed\n");
                break;
            }
            fprintf(stdout, "recv %d:%s", (int) strlen(recvbuf), recvbuf);
        }
        // close(sock);
        exit(EXIT_SUCCESS);
    } else {
        char sendbuf[1024];
        while (fgets(sendbuf, sizeof(sendbuf), stdin)) {
            writen(sock, sendbuf, sizeof(sendbuf));
            memset(sendbuf, 0, sizeof(sendbuf));
        }
        // close(sock);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
