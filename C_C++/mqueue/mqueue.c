#include "myLinux.h"

int main(int argc, char *argv[])
{
        mqd_t mqid;
        mqid = mq_open("/abc", O_CREAT | O_RDWR, 0666, NULL);
        if (mqid == (mqd_t) -1)
                ERR_EXIT("mq_open");
        return 0;
}
