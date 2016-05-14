#include "my_unix.h"

int main()
{
    // 创建fifo文件
    mkfifo("myfifo", 0777);

    // 打开fifo文件
    int fd = open("myfifo", O_WRONLY);

    // 写入数据到fifo文件
    write(fd, "hello fifo", 11);

    // 关闭fifo文件
    close(fd);
    return 0;
}
