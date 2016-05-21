#include "parse.h"
#include "init.h"
#include "def.h"

char cmdline[MAXLINE + 1];
char avline[MAXLINE + 1];
char *lineptr;
char *avptr;
char infile[MAXNAME + 1];
char outfile[MAXNAME + 1];
COMMAND cmd[PIPELINE];

int cmd_count;
int backgnd;
int append;
int lastpid;

int main(int argc, char *argv[])
{
        /* setup signals */
        setup();
        shell_loop();
        return 0;
}
