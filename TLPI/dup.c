#include <myLinux.h>

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_CREAT | O_WRONLY, 0644);
    int saved_fd = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    write(STDOUT_FILENO, "123", 3);
    printf("456\n");
    fflush(stdout);

    close(STDOUT_FILENO);
    dup2(saved_fd, STDOUT_FILENO);

    printf("welcome back\n");
    return 0;
}
