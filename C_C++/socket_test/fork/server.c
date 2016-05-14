#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERBER_PORT 8000

int main(int argc, char *argv[])
{
    struct sockaddr_in serveraddr, clientaddr;
    int socketfd, addrlen, confd;
    char ipstr[128];
    char buf[128];
    int len, i;
    pid_t pid;

    /*
     * 1. socket
     * 2. bind
     * 3. listen
     * 4. accept
     */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serveraddr, sizeof (serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // "192.168.182.129";
    serveraddr.sin_port = htons(SERBER_PORT);
    bind(socketfd, (struct sockaddr *) &serveraddr, sizeof (serveraddr));

    listen(socketfd, 128);
    const char *tmpc = inet_ntop(AF_INET, &serveraddr.sin_addr.s_addr, ipstr, sizeof (ipstr));
    printf("server ip %s\tport %d\n", tmpc, ntohs(serveraddr.sin_port));

    while (1) {
        addrlen = sizeof (clientaddr);
        confd = accept(socketfd, (struct sockaddr *) &clientaddr, &addrlen);

        tmpc = inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipstr, sizeof (ipstr));
        printf("client ip %s\tport %d\n", tmpc, ntohs(clientaddr.sin_port));
        /* 多进程服务器 */
        pid = fork();
        if (pid == 0) {
            // 子进程
            close(socketfd);
            while (1) {
                len = read(confd, buf, sizeof (buf));
                for (i = 0; i < len; ++i)
                    buf[i] = toupper(buf[i]);
                sleep(5);
                write(confd, buf, len);
            }
            close(confd);
            return 0;
        } else if (pid > 0) {
            close(confd);
        } else {
        }

    }
    close(socketfd);

    return 0;
}
