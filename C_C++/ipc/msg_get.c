#include "myLinux.h"

int main(int argc, char *argv[])
{
        // int msgid = msgget(1234, 0666 | IPC_CREAT);
        // int msgid = msgget(1234, 0666 | IPC_CREAT | IPC_EXCL);
        int msgid = msgget(1234, 0666);
        if (msgid == -1)
                ERR_EXIT("msgget");

        printf("msgget = %d\n", msgid);
        return 0;
}
