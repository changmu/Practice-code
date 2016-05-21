#include "execute.h"
#include "def.h"
#include "externs.h"
#include "parse.h"

void forkexec(int id);

void execute_disk_command()
{
        /* ls | grep init | wc -w */
        int flags;

        if (cmd_count == 0)
                return;

        if (infile[0] != '\0')
                cmd[0].infd = open(infile, O_RDONLY);

        if (outfile[0] != '\0') {
                flags = O_WRONLY | O_CREAT;
                if (append)
                        flags |= O_APPEND;
                else
                        flags |= O_TRUNC;
                cmd[cmd_count - 1].outfd = open(outfile, flags, 0666);
        }

        /* no need for calling wait() */
        if (backgnd == 1)
                signal(SIGCHLD, SIG_IGN);
        else
                signal(SIGCHLD, SIG_DFL);

        int i;
        int fd;
        int fds[2];
        for (i = 0; i < cmd_count; ++i) {
                if (i < cmd_count - 1) {
                        pipe(fds);
                        cmd[i].outfd = fds[1];
                        cmd[i + 1].infd = fds[0];
                }

                forkexec(i);

                fd = cmd[i].infd;
                if (fd != 0)
                        close(fd);
                fd = cmd[i].outfd;
                if (fd != 1)
                        close(fd);
        }

        if (backgnd == 0) {
                /* front job, wait last command to exit */
                while (wait(NULL) != lastpid)
                        ;
        }
}

void forkexec(int id)
{
        int i;
        pid_t pid = fork();
        if (pid == -1)
                ERR_EXIT("fork");

        if (pid > 0) {
                if (backgnd)
                        printf("[%d]\n", pid);
                lastpid = pid;
        } else {
                if (cmd[id].infd == 0 && backgnd)
                        cmd[id].infd = open("/dev/null", O_RDONLY);
                if (id == 0)
                        setpgid(0, 0);

                if (cmd[id].infd != 0) {
                        close(0);
                        dup(cmd[id].infd);
                }
                if (cmd[id].outfd != 1) {
                        close(1);
                        dup(cmd[id].outfd);
                }

                for (i = 3; i < NR_OPEN; ++i)
                        close(i);

                /* make front job able to accept the signals */
                if (backgnd == 0) {
                        signal(SIGINT, SIG_DFL);
                        signal(SIGQUIT, SIG_DFL);
                }

                execvp(cmd[id].args[0], cmd[id].args);
                exit(EXIT_FAILURE);
        }
}
