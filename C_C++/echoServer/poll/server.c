/*
 * implementing concurrence with 'single process and select'
 * author: qzg
 * complete time: 2016.05.12
 */
#include <myLinux.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086
#define MAXN 2048

/*
   struct packet {
   int len;
   char buf[1024];
   };
   */

void echo_server(int conn)
{
        char recvbuf[1024];
        static int ret;
        while (1) {
                memset(recvbuf, 0, sizeof(recvbuf));
                ret = readline(conn, recvbuf, 1024);
                if (ret == -1) { ERR_EXIT("readline"); }
                if (ret == 0) {
                        printf("client close\n");
                        break;
                }
                printf("recv: %s", recvbuf);
                writen(conn, recvbuf, strlen(recvbuf));
        }
}

void handle_sigchld(int sig)
{
        wait(NULL);
}

void handle_sigpipe(int sig)
{
        printf("recv signal: %d\n", sig);
}

int main()
{
        // signal(SIGPIPE, SIG_IGN);
        signal(SIGPIPE, handle_sigpipe);

        signal(SIGCHLD, SIG_IGN);
        // signal(SIGCHLD, handle_sigchld);
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
        socklen_t peerlen;
        int conn;
        int i;

        int nready;
        int maxfd = listenfd;
        int maxi = 0;   // the max not idle position
        fd_set rset;
        fd_set allset;

        FD_ZERO(&rset);
        FD_ZERO(&allset);
        FD_SET(listenfd, &allset);
        
        struct pollfd client[MAXN];
        int count = 0;

        client[0].fd = listenfd;
        client[0].events = POLLIN;
        for (i = 1; i < MAXN; ++i)
                client[i].fd = -1;

        while (1) {
                rset = allset;
                // nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
                nready = poll(client, maxfd + 1, -1);
                if (nready == -1) {
                        if (errno == EINTR) continue;

                        ERR_EXIT("poll");
                }
                if (nready == 0) continue;

                // if (FD_ISSET(listenfd, &rset)) {
                if (client[0].revents & POLLIN) {
                        peerlen = sizeof(peeraddr);
                        conn = accept(listenfd, (struct sockaddr *) &peeraddr, &peerlen);
                        if (conn < 0) { ERR_EXIT("accept"); }

                        maxfd = MAX(maxfd, conn);
                        FD_SET(conn, &allset);

                        for (i = 0; i < MAXN; ++i) {
                                if (client[i].fd < 0) {
                                        client[i].fd = conn;
                                        client[i].events = POLLIN;
                                        maxi = MAX(maxi, i);
                                        break;
                                }
                        }
                        if (i == MAXN) {
                                fprintf(stderr, "too many clients.\n");
                                exit(EXIT_FAILURE);
                        }

                        printf("Client: %s:%hu\ncount = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), ++count);
                        if (--nready <= 0) 
                                continue;
                }

                for (i = 1; i <= maxi; ++i) {
                        conn = client[i].fd;
                        if (conn == -1)
                                continue;

                        // if (FD_ISSET(conn, &rset)) {
                        if (client[i].revents & POLLIN) {
                                char recvbuf[1024] = {0};
                                ret = readline(conn, recvbuf, 1023);
                                if (ret == -1) { ERR_EXIT("readline"); }
                                if (ret == 0) {
                                        printf("client close\n");
                                        FD_CLR(conn, &allset);
                                        client[i].fd = -1;
                                        close(conn);
                                }
                                printf("recv: %s", recvbuf);
                                writen(conn, recvbuf, strlen(recvbuf));

                                if (--nready <= 0)
                                        break;
                        }
                }
        }
        return 0;
}
