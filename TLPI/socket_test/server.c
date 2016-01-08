#include <myLinux.h>

#define SERVER_PORT 8000

int main(int argc, char *argv[])
{
    struct sockaddr_in clientAddr, serverAddr;
    int i, listenFd, connectFd, len;
    socklen_t clientSocketLen;
    char strIp[INET_ADDRSTRLEN];
    char buf[1024];

    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serverAddr, 0, sizeof (serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);
    bind(listenFd, (struct sockaddr *) &serverAddr, sizeof (serverAddr));

    listen(listenFd, 20);
    printf("Server %s:%d\n", 
            inet_ntop(AF_INET, &serverAddr.sin_addr, strIp, sizeof (strIp)),
            ntohs(serverAddr.sin_port)
          );
    printf("Accepting connections...\n");

    while (1) {
        clientSocketLen = sizeof (clientAddr);
        connectFd = accept(listenFd, (struct sockaddr *) &clientAddr, &clientSocketLen);
        printf("client from %s:%d\n", inet_ntop(AF_INET, &clientAddr.sin_addr, strIp, sizeof (strIp)),
                ntohs(clientAddr.sin_port)
              );
        if (fork() == 0) {
            while (1) {
                len = read(connectFd, buf, sizeof (buf));
                buf[len] = '\0';
                for (i = 0; i < len; ++i)
                    buf[i] = toupper(buf[i]);
                write(connectFd, buf, len);
            }
        }
    }
    close(listenFd);

    return 0;
}
