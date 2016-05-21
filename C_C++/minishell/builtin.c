#include "builtin.h"
#include "parse.h"

typedef void (*CMD_HANDLER)();

typedef struct builtin_cmd {
        char *name;
        CMD_HANDLER handler;
} BUILTIN_CMD;

void do_exit();
void do_cd();
void do_type();

BUILTIN_CMD builtins[] = {
        {"exit", do_exit},
        {"cd", do_cd},
        {"type", do_type},
};

/**
 * 1 for builtin, 0 for not a builtin
 */
int builtin()
{
        int i = 0;
        int ret = 0;
        for ( ; i < ARRLEN(builtins); ++i) {
                if (check(builtins[i].name)) {
                        builtins[i].handler();
                        ret = 1;
                        break;
                }
        }
        return ret;
}

void do_exit()
{
        printf("exit...\n");
        exit(EXIT_SUCCESS);
}

void do_cd()
{
        printf("do_cd..\n");
}

void do_type()
{
        printf("do_type..\n");
}
