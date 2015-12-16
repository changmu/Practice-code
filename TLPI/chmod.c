#include <myLinux.h>

int my_atoi(const char *str)
{
    int ret = 0, i;

    for (i = 0; str[i] != '\0'; ++i) {
        ret = ret * 8 + str[i] - '0';
    }

    return ret;
}

int main(int args, char *argv[])
{
    if (args < 3) {
        fprintf(stderr, "%s <mode> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int i;
    mode_t mode = my_atoi(argv[1]);

    for (i = 2; i < args; ++i) {
        chmod(argv[i], mode);
    }
    return 0;
}
