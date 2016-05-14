/*
 * 没法用unlink创建临时文件
 * */
#include <myLinux.h>

char buf[10];

int main(int argc, char *argv[])
{
    // link(argv[1], argv[2]);
    // symlink(argv[1], argv[2]);

    // readlink(argv[1], buf, 10);
    // puts(buf);
    int fd = open(argv[1], O_CREAT | O_RDWR, 0664);
    write(fd, "hello\n", 6);
    unlink(argv[1]);
    read(fd, buf, 6);
    puts(buf);
    sleep(10);

    return 0;
}
