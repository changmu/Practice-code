/*
 * 思路是将源文件分成多分，每一份用单独的进程去拷贝
 * 问题：无法复制上百兆的大文件
 * */
#include <myLinux.h>

// max bytes per process handles
#define MAX_LEN 4096

int subProcessCopy(const char *src, const char *tar, off_t src_pos)
{
    if (fork()) return 1;

    int src_fd = open(src, O_RDONLY);
    if (src_fd == -1) errExit("open");
    lseek(src_fd, src_pos, SEEK_SET);
    
    int tar_fd = open(tar, O_WRONLY);
    if (tar_fd == -1) errExit("open");
    lseek(tar_fd, src_pos, SEEK_SET);

    char buf[MAX_LEN];
    ssize_t sz = read(src_fd, buf, MAX_LEN);
    if (sz < 0) errExit("read");
    if (write(tar_fd, buf, sz) < 0) errExit("write");

    // close(src_fd);
    // close(tar_fd);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <src> <tar>\n", argv[0]);
        exit(1);
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) errExit("open");

    int tar_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0664);
    if (tar_fd == -1) errExit("open");

    // 获取src文件大小
    off_t src_len = lseek(src_fd, 0, SEEK_END);
    if (src_len == 0) return 0; // empty file

    /*
    // 扩展tar文件到src的大小
    lseek(tar_fd, src_len - 1, SEEK_SET);
    write(tar_fd, "E", 1); // the end of tar
*/

    // 开始分割复制
    off_t cur_len = 0;
    for ( ; cur_len < src_len; cur_len += MAX_LEN) {
        // lseek(src_fd, cur_len, SEEK_SET);
        if (!subProcessCopy(argv[1], argv[2], cur_len))
            return 1;
    }

    // while (2);
    waitpid(-1, NULL, 0);

    return 0;
}
