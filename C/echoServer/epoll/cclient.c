#include <myLinux.h>

// #define SERVER_IP "127.0.0.1"
// #define SERVER_PORT 8086
#define SERVER_IP "115.159.41.73"
#define SERVER_PORT 80

int main(int argc, char *argv[])
{
        int ret;
        int sock;
        int count = 0;
        struct rlimit rl;
        socklen_t socklen;
/*
        int getrlimit(int resource, struct rlimit *rlim);
        int setrlimit(int resource, const struct rlimit *rlim);
*/
        getrlimit(RLIMIT_NOFILE, &rl);
        rl.rlim_max = 61234;
        rl.rlim_cur = rl.rlim_max - 1;
        ret = setrlimit(RLIMIT_NOFILE, &rl);
        if (ret == -1) { ERR_EXIT("setrlimit"); }
        getrlimit(RLIMIT_NOFILE, &rl);
        printf("s:%d h:%d\n", (int) rl.rlim_cur, (int) rl.rlim_max);
        sleep(1);

        while (1) {
                sock = socket(PF_INET, SOCK_STREAM, 0);
                if (sock < 0) { sleep(5); ERR_EXIT("socket"); }

                struct sockaddr_in servaddr;
                memset(&servaddr, 0, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                servaddr.sin_port = htons(SERVER_PORT);
                servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

                socklen = sizeof(servaddr);
                ret = connect(sock, (struct sockaddr *) &servaddr, socklen);
                if (ret < 0) { ERR_EXIT("connect"); }

                struct sockaddr_in localaddr;
                socklen_t addrlen = sizeof(localaddr);
                ret = getsockname(sock, (struct sockaddr *) &localaddr, &addrlen);
                if (ret < 0) { ERR_EXIT("getsockname"); }
                printf("Client: %s:%hu\ncount = %d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port), ++count);
                writen(sock, "hello\n", 6);
        }

        return 0;
}
