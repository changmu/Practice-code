#include "parse.h"
#include "externs.h"
#include "init.h"
#include "execute.h"
#include "builtin.h"

void get_command(int i);
int check(const char *str);
void getname(char *name);
void print_command();
void forkexec(int id);

/* shell main loop */
void shell_loop(void)
{
        int ret;
        while (1) {
                init();

                ret = read_command();
                if (ret == -1)
                        break;

                parse_command();

                // print_command();
                execute_command();
        }
        
        printf("\nexit..\n");
}

/* return 0 for success or -1 for failure or EOF */
int read_command(void)
{
        if (fgets(cmdline, 1024, stdin) == NULL)
                return -1;

        return 0;
}

/* return 0 for success or -1 for failure */
int parse_command(void)
{
        /* cat < test.txt | grep -n public > test2.txt & */
        if (check("\n"))
                return 0;
        if (builtin()) 
                return 0;
        /* 1. parse the first command */
        get_command(0);
        /* 2. check if there are IR(<) */
        if (check("<"))
                getname(infile);
        /* 3. check if there are pipe */
        int i;
        for (i = 1; i < PIPELINE; ++i) {
                if (check("|"))
                        get_command(i);
                else
                        break;
        }
        /* 4. check if there are OR(>/>>) */
        if (check(">")) {
                if (check(">"))
                        append = 1;
                else
                        append = 0;
                getname(outfile);
        }
        /* 5. check if there are background job */
        if (check("&"))
                backgnd = 1;
        /* 6. check end of commands '\n' */
        if (check("\n")) {
                cmd_count = i;
                return i;
        } else {
                fprintf(stderr, "Command line syntax error\n");
                return -1;
        }

        return 0;
}

/* return 0 for success or -1 for failure */
int execute_command(void)
{
        execute_disk_command();
        return 0;
}

void print_command()
{
        int i;
        int j;
        printf("cmd_count = %d\n", cmd_count);
        if (infile[0] != '\0')
                printf("infile = [%s]\n", infile);
        if (outfile[0] != '\0')
                printf("outfile = [%s]\n", outfile);

        for (i = 0; i < cmd_count; ++i) {
                j = 0;
                while (cmd[i].args[j] != NULL) {
                        printf("[%s] ", cmd[i].args[j]);
                        ++j;
                }
                printf("\n");
        }
}

/* parse simple command to cmd[i],
 * extract args to avline[],
 * point cmd.args[] to these strings
 */
void get_command(int i)
{
        /* cat < test.txt | grep -n public > test2.txt & */
        int j = 0;
        // int inword = 0;
        while (*lineptr != '\0') {
                while (*lineptr == ' ' || *lineptr == '\t')
                        ++lineptr;

                cmd[i].args[j] = avptr;
                while (*lineptr != '\0' && *lineptr != ' ' && *lineptr != '\t' && *lineptr != '\n'
                                && *lineptr != '<' && *lineptr != '>' && *lineptr != '|' && *lineptr != '&') {
                        *avptr++ = *lineptr++;
                        // inword = 1;
                }
                *avptr++ = '\0';

                switch (*lineptr) {
                        case ' ':
                        case '\t':
                                // inword = 0;
                                ++j;
                                break;
                        case '<':
                        case '>':
                        case '|':
                        case '&':
                        case '\n':
                                // if (inword == 0)
                                        // cmd[i].args[j] = NULL;
                                return;
                        default: /* for '\0' */
                                return;
                }
        }
}

/**
 * compare lineptr with str, 1 for success and lineptr move forward,
 * 0 for failure
 */
int check(const char *str)
{
        while (*lineptr == ' ' || *lineptr == '\t')
                ++lineptr;

        char *p = lineptr;
        while (*str != '\0' && *str == *p) {
                ++str;
                ++p;
        }
        if (*str == '\0') {
                lineptr = p;
                return 1;
        }

        /* lineptr not changed */
        return 0;
}

void getname(char *name)
{
        while (*lineptr == ' ' || *lineptr == '\t')
                ++lineptr;

        while (*lineptr != '\0' && *lineptr != ' ' && *lineptr != '\t' && *lineptr != '\n'
                        && *lineptr != '<' && *lineptr != '>' && *lineptr != '|' && *lineptr != '&') {
                *name++ = *lineptr++;
        }
        *name = '\0';
}
