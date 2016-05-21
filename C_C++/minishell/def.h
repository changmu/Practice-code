#pragma once

#define MAXLINE 1024 // maxlen of input line
#define MAXARG  20      // command's num
#define PIPELINE 5      // commands' num in pipe
#define MAXNAME 100     // IO redirection's file name

typedef struct command {
        char *args[MAXARG + 1];
        int infd;
        int outfd;
} COMMAND;
