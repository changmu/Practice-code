#include <myLinux.h>

#define SERVER_IP "192.168.182.129"
#define SERVER_PORT 8000

int main(int argc, char *argv[])
{
    struct sockaddr_in serverAddr;
    int socketFd, len;
    char buf[1024] = "hello";

    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serverAddr, 0, sizeof (serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(SERVER_PORT);

    connect(socketFd, (struct sockaddr *) &serverAddr, sizeof (serverAddr));
    printf("TCP established.\n");

    while (gets(buf)) {
        write(socketFd, buf, strlen(buf));
        usleep(100);
        len = read(socketFd, buf, sizeof (buf));
        buf[len] = '\0';
        puts(buf);
    }
    return 0;
}
