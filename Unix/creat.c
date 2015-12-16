#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd = creat("/home/qiu/test.txt", 0621);
    if (fd < 0)
        printf("error\n");
    return 0;
}
