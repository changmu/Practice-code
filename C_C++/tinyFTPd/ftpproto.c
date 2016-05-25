#include "ftpproto.h"
#include "str.h"

void handle_child(session_t *sess)
{
        int ret;

        writen(sess->ctrl_fd, WELCOME, strlen(WELCOME));
        while (1) {
                memset(sess->cmdline, 0, sizeof(sess->cmdline));
                memset(sess->cmd, 0, sizeof(sess->cmd));
                memset(sess->arg, 0, sizeof(sess->arg));
                ret = readline(sess->ctrl_fd, sess->cmdline, MAX_COMMAND_LINE - 1);
                if (ret < 0)
                        ERR_EXIT("readline");
                else if (ret == 0)
                        exit(EXIT_FAILURE);

                printf("cmdline = [%s]\n", sess->cmdline);

                // wipe off \r\n
                str_trim_crlf(sess->cmdline);
                printf("cmdline = [%s]\n", sess->cmdline);
                // parse command and args
                str_split(sess->cmdline, sess->cmd, sess->arg, ' ');
                printf("cmd = [%s] arg = [%s]\n", sess->cmd, sess->arg);
                str_upper(sess->cmd);
                // handle ftp command
        }
}
