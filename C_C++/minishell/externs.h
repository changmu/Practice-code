#pragma once

#include "def.h"
extern COMMAND cmd[PIPELINE];

extern char cmdline[MAXLINE + 1];
extern char avline[MAXLINE + 1];
extern char *lineptr;
extern char *avptr;

extern char infile[MAXNAME + 1];
extern char outfile[MAXNAME + 1];

extern int cmd_count;
extern int backgnd;
extern int append;
extern int lastpid;
