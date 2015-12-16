#include <myLinux.h>

int main()
{
    // 先关闭stdout，释放其文件描述符
    close(STDOUT_FILENO);

    // 这个时候fd应该为1吧
    int fd = open("out", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    // 此时printf应该把内容输出到out文件中吧？
    printf("fd:%d\n", fd);
    fflush(stdout);

    close(fd);

    return 0;
}
