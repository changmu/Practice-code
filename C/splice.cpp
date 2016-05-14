#include "myLinux.h"

int main(int argc, char *argv[])
{
    if (argc <= 2) {
        printf("usage: %s ip port\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    memset(&address, 0, sizeof (address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *) &address, sizeof (address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    int pipefd[2];
    ret = pipe(pipefd);
    assert(ret != -1);
    struct sockaddr_in client;

    while (true) {
        socklen_t client_addrlength = sizeof (client);
        int connfd = accept(sock, (struct sockaddr *) &client, &client_addrlength);
        if (connfd < 0) {
            printf("errno is: %d:%s\n", errno, strerror(errno));
        } else {
            while (true) {
                /* 将connfd上流入的客户数据定向到管道中 */
                ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
                assert(ret != -1);
                /* 将管道的输出定向到connfd客户连接文件描述符 */
                ret = splice(pipefd[0], NULL, connfd, NULL, 32768,
                        SPLICE_F_MORE | SPLICE_F_MOVE);
                assert(ret != -1);
            }
            close(connfd);
        }
    }
    close(sock);
    return 0;
}
