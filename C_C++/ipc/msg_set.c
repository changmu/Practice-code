#include "myLinux.h"

int main(int argc, char *argv[])
{
        int msgid = msgget(1234, 0666 | IPC_CREAT);
        // int msgid = msgget(1234, 0666 | IPC_CREAT | IPC_EXCL);
        // int msgid = msgget(1234, 0666);
        if (msgid == -1)
                ERR_EXIT("msgget");

        printf("msgget = %d\n", msgid);

        struct msqid_ds buf;
        msgctl(msgid, IPC_STAT, &buf);
        buf.msg_perm.mode = 0600;
        msgctl(msgid, IPC_SET, &buf);
        return 0;
}
