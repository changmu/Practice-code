#include "utility.h"

int main(int argc, char *argv[])
{
    // appoint server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { errExit("sock error"); }

    // connect to server
    int ret = connect(sock, (struct sockaddr *) &serverAddr, sizeof (serverAddr));
    if (ret < 0) { errExit("connect error"); }

    // create pipe, fd[0] for parent's reading, fd[1] for child's writing
    int pipe_fd[2];
    ret = pipe(pipe_fd);
    if (ret < 0) { errExit("pipe error"); }

    // create epoll
    int epFd = epoll_create(EPOLL_SIZE);
    if (epFd < 0) { errExit("epFd error"); }
    static struct epoll_event events[2];

    // put sock and fd[0] in kernel event list
    addFd(epFd, sock, true);
    addFd(epFd, pipe_fd[0], true);

    // check if client working normally
    bool isClientWork = true;
    // chatting info buffer
    char message[BUF_SIZE];

    int pid = fork();
    if (pid < 0) { errExit("fork error"); }
    else if (pid == 0) {
        // child responses for writting to pipe
        close(pipe_fd[0]);
        printf("Please input 'exit' to exit the char room\n");

        while (isClientWork) {
            memset(message, 0, BUF_SIZE);
            fgets(message, BUF_SIZE, stdin);

            if (!strncasecmp(message, EXIT, strlen(EXIT))) {
                // exit
                isClientWork = false;
            } else {
                // write to pipe
                ret = write(pipe_fd[1], message, strlen(message) - 1);
                if (ret < 0) { errExit("fork write error"); }
            }
        }
    } else {
        // parent responses for reading
        close(pipe_fd[1]);
        // main while
        while (isClientWork) {
            int epoll_events_count = epoll_wait(epFd, events, 2, -1);
            // handle readiness event
            for (int i = 0; i < epoll_events_count; ++i) {
                memset(message, 0, BUF_SIZE);

                if (events[i].data.fd == sock) {
                    // from server
                    ret = recv(sock, message, BUF_SIZE, 0);
                    if (ret == 0) {
                        printf("Server closed connection: %d\n", sock);
                        close(sock);
                        isClientWork = false;
                    } else { puts(message); }
                } else {
                    // from child
                    int ret = read(events[i].data.fd, message, BUF_SIZE);
                    if (ret == 0) { isClientWork = false; }
                    else { send(sock, message, BUF_SIZE, 0); }
                }
            } // for
        } // while
    }

    if (pid) {
        close(pipe_fd[0]);
        close(sock);
    } else { close(pipe_fd[1]); }

    return 0;
}
