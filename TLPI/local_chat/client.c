#include <myLinux.h>
#include "common.h"

int private_fifo_fd;
int shared_fifo_fd;

int readFromConsole(char *buf)
{
    int sz = read(STDIN_FILENO, buf, MAXN - 1);
    if (sz <= 0) return -1;

    buf[sz] = '\0';

    return 0;
}

void makePacket(char *buf, struct Packet *packet)
{
    static char buff[MAXN];
    packet->msg = buff;
    sscanf(buf, "%d %d %d %s", &packet->kind, &packet->src,
            &packet->dst, packet->msg);
}

void sendToServer(struct Packet *packet)
{
    static char buf[MAXN];

    sprintf(buf, "%d %d %d %s", packet->kind, packet->src,
            packet->dst, packet->msg);
    write(shared_fifo_fd, buf, strlen(buf));
}

int readFromPrivateFifo(char *buf)
{
    int sz = read(private_fifo_fd, buf, MAXN - 1);
    if (sz <= 0) return -1;
    buf[sz] = '\0';
    return 0;
}

void printPacket(struct Packet *packet)
{
    if (packet->kind == NOTONLINE) {
        printf("dst user is not online");
    } else {
        printf("Msg from %d: %s\n", packet->src, packet->msg);
    }
}

int main(int argc, char **argv)
{
    // initialize
    int uid;
    char buf[MAXN];
    printf("Enter your uid: ");
    scanf("%d", &uid);

    // login
    if ((shared_fifo_fd = open("shared_fifo", O_WRONLY)) == -1) {
        perror("cli shared");
    }
    struct Packet packet = {LOGIN, uid, 0, "\0"};
    sendToServer(&packet);
    sleep(1);

    // open private fifo and shared fifo
    sprintf(buf, "private_fifo%d", uid);
    if ((private_fifo_fd = open(buf, O_RDONLY | O_NONBLOCK)) == -1)
        perror("cli private");

    // set stdin as nonblock
    int flag = fcntl(STDIN_FILENO, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flag);

    // main work
    while (1) {
        if (readFromConsole(buf) != -1) {
            makePacket(buf, &packet);
            sendToServer(&packet);
        }
        if (readFromPrivateFifo(buf) != -1) {
            parseBytesToPacket(buf, &packet);
            printPacket(&packet);
            continue;
        }
        sleep(1);
    }
    return 0;
}
