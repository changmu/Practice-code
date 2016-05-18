#include "myLinux.h"

struct msgbuf {
        long mtype;       /* message type, must be > 0 */
        char mtext[1];    /* message data */
};

int main(int argc, char *argv[])
{
        if (argc != 3) {
                fprintf(stderr, "Usage: %s <bytes> <type>\n", basename(argv[0]));
                exit(EXIT_FAILURE);
        }

        int len = atoi(argv[1]);
        int type = atoi(argv[2]);
        int msgid = msgget(1234, 0);
        if (msgid == -1)
                ERR_EXIT("msgget");

        struct msgbuf *ptr;
        ptr = malloc(sizeof(long) + len);
        ptr->mtype = type;
        msgsnd(msgid, ptr, len, 0);
        return 0;
}
