#include <myLinux.h>
#include "common.h"

// save user's id
int users = 0;
int shared_fifo_fd;
int users_fd[32]; // at most 31 users

int readFromSharedFifo(char *buf)
{
    int sz = read(shared_fifo_fd, buf, MAXN - 1);
    if (sz <= 0) return -1;

    buf[sz] = '\0';

    return 0;
}

void logIn(int uid)
{ 
    static char buf[MAXN];

    users |= (1 << uid); 
    // creat private fifo
    sprintf(buf, "private_fifo%d", uid);

    mkfifo(buf, 0664);
    users_fd[uid] = open(buf, O_WRONLY, 0664);
    if (users_fd[uid] == -1) errExit("server 27");
}

void logOut(int uid)
{ 
    users ^= (1 << uid); 
    close(users_fd[uid]);
}

int checkUser(int uid) 
{
    return users & (1 << uid);
}

void chat(int src, int dst, const char *msg)
{
    static char buf[MAXN];
    if (checkUser(dst) == 0) {
        sprintf(buf, "%d %d %d %s", NOTONLINE, 0, 0, "");
        write(users_fd[src], buf, strlen(buf));
    } else {
        sprintf(buf, "%d %d %d %s", CHAT, src, dst, msg);
        write(users_fd[dst], buf, strlen(buf));
    }
}

void handlePacket(struct Packet *packet)
{
    switch (packet->kind) {
        case LOGIN:     logIn(packet->src);     break;
        case LOGOUT:    logOut(packet->src);    break;
        case CHAT:      chat(packet->src, packet->dst, packet->msg);
                        break;
    }
}

int main(int argc, char **argv)
{
#ifdef DEBUG
    struct Packet p1 = {CHAT, 1, 2, "hello"}, p2;
    char buff[1024];

    sprintf(buff, "%d %d %d %s", p1.kind, p1.src, p1.dst, p1.msg);
    parseBytesToPacket(buff, &p2);
    printf("p2: %d %d %d %s\n", p2.kind, p2.src, p2.dst, p2.msg);
#endif

    char buf[MAXN];
    struct Packet packet;

    mkfifo("shared_fifo", 0664);
    shared_fifo_fd = open("shared_fifo", O_RDONLY | O_NONBLOCK);
    if (shared_fifo_fd == -1) errExit("open shared_fifo");

    while (1) {
        if (readFromSharedFifo(buf) != -1) {
            parseBytesToPacket(buf, &packet);
            handlePacket(&packet);
            continue;
        }
        sleep(1);
    }
    return 0;
}
