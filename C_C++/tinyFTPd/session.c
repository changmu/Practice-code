#include "session.h"
#include "ftpproto.h"
#include "privaparent.h"
#include "privsock.h"

void begin_session(session_t *sess)
{
        setOOB(sess->ctrl_fd);
        // int ret;
        /* int sockfds[2];
        ret = socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds);
        if (ret < 0)
                ERR_EXIT("socketpair"); */
        priv_sock_init(sess);


        pid_t pid;
        pid = fork();
        if (pid < 0)
                ERR_EXIT("fork");

        if (pid == 0) {
                // ftp server process
                /* close(sockfds[0]);
                sess->child_fd = sockfds[1]; */
                priv_sock_set_child_context(sess);
                handle_child(sess);
        } else {
                // nobody process 
                /* close(sockfds[1]);
                sess->parent_fd = sockfds[0]; */
                priv_sock_set_parent_context(sess);
                handle_parent(sess);
        }
}