#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    struct option long_options[] = {
        { "host", 1, 0, 0 },
        { "port", 1, 0, 0 },
        { "dir", 1, 0, 0 },
        { "daemon", 0, 0, 0 }
    };

    int ret;
    int option_index = 132; // 传出参数，表示当前选项索引
    char *argp = NULL;  // 保存选项的参数
    int is_daemon = 0;  // 同上
    while (1) {
        /* 第三个开始参数分别为: 短参数列表(不能为null)，长参数列表，传出索引 */
        ret = getopt_long(argc, argv, "H:P:D:", long_options, &option_index);

        if (ret == '?')
            return 0;

        if (ret == -1)
            break;
        
        if (optarg)
            argp = strdup(optarg);

        if (ret == 0 && !strcasecmp(long_options[option_index].name, "host") || ret == 'H') {
            printf("host:%s\n", argp);
        } else if (ret == 0 && !strcasecmp(long_options[option_index].name, "port") || ret == 'P') {
            printf("port:%s\n", argp);
        } else if (ret == 0 && !strcasecmp(long_options[option_index].name, "dir") || ret == 'D') {
            printf("dir:%s\n", argp);
        } else if (ret == 0 && !strcasecmp(long_options[option_index].name, "daemon")) {
            printf("daemon\n");
        }

    }
    printf("over\n");
    return 0;
}