#include "my_unix.h"

int main()
{
    int fd = open("myfifo", O_RDONLY);
    char buf[20];
    read(fd, buf, 11);
    buf[11] = '\0';
    puts(buf);
    close(fd);
    unlink("myfifo");
    return 0;
}
