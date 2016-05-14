extern "C" {
#include <myLinux.h>
}

#include <vector>
#include <algorithm>

typedef std::vector<struct epoll_event> EventList;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8086
#define MAXN 2048

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
        struct rlimit rl;
        getrlimit(RLIMIT_NOFILE, &rl);
        rl.rlim_max = 61234;
        rl.rlim_cur = rl.rlim_max - 1;
        ret = setrlimit(RLIMIT_NOFILE, &rl);
        if (ret == -1) { ERR_EXIT("setrlimit"); }
        getrlimit(RLIMIT_NOFILE, &rl);
        printf("s:%d h:%d\n", (int) rl.rlim_cur, (int) rl.rlim_max);
        sleep(1);

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
        int count = 0;

        int epollfd;
        struct epoll_event event;
        EventList events(16);
        std::vector<int> clients;

        epollfd = epoll_create1(EPOLL_CLOEXEC);
        event.data.fd = listenfd;
        event.events = EPOLLIN | EPOLLET;

        epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

        while (1) {
                nready = epoll_wait(epollfd, &*events.begin(), static_cast<int>(events.size()) - 1, -1);
                if (nready == -1) {
                        if (errno == EINTR)
                                continue;
                        ERR_EXIT("epoll_wait");
                }
                if (nready == 0)
                        continue;
                if ((size_t) nready == events.size())
                        events.resize(events.size() * 2);

                for (i = 0; i < nready; ++i) {
                        if (events[i].data.fd == listenfd) {
                                peerlen = sizeof(peeraddr);
                                conn = accept(listenfd, (struct sockaddr *) &peeraddr, &peerlen);
                                if (conn == -1)
                                        ERR_EXIT("accept");

                                printf("client:%s:%hu\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
                                printf("count = %d\n", ++count);
                                clients.push_back(conn);

                                setNonblock(conn);

                                event.data.fd = conn;
                                event.events = EPOLLIN | EPOLLET;
                                epoll_ctl(epollfd, EPOLL_CTL_ADD, conn, &event);
                        } else if (events[i].events & EPOLLIN) {
                                conn = events[i].data.fd; 
                                if (conn < 0)
                                        continue;

                                char recvbuf[1024] = {0};
                                int ret = readline(conn, recvbuf, sizeof(recvbuf) - 1);
                                if (ret == -1)
                                        ERR_EXIT("readline");
                                if (ret == 0) {
                                        printf("client closed.\n");
                                        close(conn);

                                        event = events[i];
                                        epoll_ctl(epollfd, EPOLL_CTL_DEL, conn, &event);
                                        clients.erase(std::remove(clients.begin(), clients.end(), conn), clients.end());
                                }

                                fputs(recvbuf, stdout);
                                writen(conn, recvbuf, strlen(recvbuf));
                        }
                }
        }

        return 0;
}
