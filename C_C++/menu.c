/*************************************************************
 * File name    :   menu.c
 * Author       :   Heurei
 * Date         :   2015.11.12
 * Description  :   This is a menu program
 *************************************************************/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_MAX_LEN 128
#define DESC_LEN    1024
#define CMD_NUM     10

int help();
int quit();

typedef struct DataNode_t {
    char    *cmd;
    char    *desc;
    int     (*handler)();
    struct  DataNode_t *next;
} DataNode_t;

const DataNode_t *findCmd(const DataNode_t *head, const char *cmd)
{
    if (NULL == head || NULL == cmd)
        return NULL;

    const DataNode_t *p = head;

    while (NULL != p) {
        if (!strcmp(p->cmd, cmd))
            return p;
        p = p->next;
    }

    return NULL;
}

int showAllCmd(const DataNode_t *head)
{
    printf("Menu List:\n");
    const DataNode_t *p = head;
    while (NULL != p) {
        printf("%s: %s\n", p->cmd, p->desc);
        p = p->next;
    }
    return 0;
}

/* menu program */
static DataNode_t head[] = {
    {"help", "this is help cmd!", help, head + 1},
    {"version", "menu program v1.0", NULL, head + 2},
    {"quit", "quit the program", quit, NULL}
};

int main()
{
    while (1) {
        char cmd[CMD_MAX_LEN];

        printf("Input a cmd> ");
        scanf("%s", cmd);
        const DataNode_t *p = findCmd(head, cmd);
        if (NULL == p) {
            printf("This is a wrong cmd!\n");
        } else {
            printf("%s: %s\n", p->cmd, p->desc);
            if (p->handler != NULL)
                p->handler();
        }
    }
    return 0;
}

int help()
{
    showAllCmd(head);
    return 0;
}

int quit()
{
    printf("Thanks for using.\n");
    exit(0);
}
