#include "session.h"
#include "ftpproto.h"
#include "privaparent.h"

void begin_session(session_t *sess)
{
        int ret;
        int sockfds[2];
        ret = socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds);
        if (ret < 0)
                ERR_EXIT("socketpair");


        pid_t pid;
        pid = fork();
        if (pid < 0)
                ERR_EXIT("fork");

        if (pid == 0) {
                // ftp server process
                close(sockfds[0]);
                sess->child_fd = sockfds[1];
                handle_child(sess);
        } else {
                // nobody process
                struct passwd *pw = getpwnam("nobody");
                if (pw == NULL)
                        return;
                ret = setegid(pw->pw_gid);
                if (ret < 0)
                        ERR_EXIT("setegid");
                seteuid(pw->pw_uid);
                if (ret < 0)
                        ERR_EXIT("seteuid");


                close(sockfds[1]);
                sess->parent_fd = sockfds[0];
                handle_parent(sess);
        }
}