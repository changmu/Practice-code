#include "privaparent.h"
#include "tunable.h"
#include "privsock.h"

static void privop_pasv_get_data_sock(session_t *sess);
static void privop_pasv_get_active(session_t *sess);
static void privop_pasv_get_listen(session_t *sess);
static void privop_pasv_get_accept(session_t *sess);

static int capset(cap_user_header_t hdrp, const cap_user_data_t datap)
{
        return syscall(__NR_capset, hdrp, datap);
}

void minimize_privilege()
{
        struct passwd *pw = getpwnam("nobody");
        if (pw == NULL)
                exit(1);
        int ret = setegid(pw->pw_gid);
        if (ret < 0)
                ERR_EXIT("setegid");
        seteuid(pw->pw_uid);
        if (ret < 0)
                ERR_EXIT("seteuid");

        struct __user_cap_header_struct cap_header;
        struct __user_cap_data_struct cap_data;

        memset(&cap_header, 0, sizeof(cap_header));
        memset(&cap_data, 0, sizeof(cap_data));

        cap_header.version = _LINUX_CAPABILITY_VERSION_2;
        cap_header.pid = 0; // no need for capset()

        __u32 cap_mask = 0;
        cap_mask |= (1 << CAP_NET_BIND_SERVICE);
        cap_data.effective = cap_data.permitted = cap_mask;
        cap_data.inheritable = 0;

        capset(&cap_header, &cap_data);
}

void handle_parent(session_t *sess)
{
        // minimize_privilege();
        
        struct passwd *pw = getpwnam("nobody");
        if (pw == NULL)
                return;
        int ret = setegid(pw->pw_gid);
        if (ret < 0)
                ERR_EXIT("setegid");
        seteuid(pw->pw_uid);
        if (ret < 0)
                ERR_EXIT("seteuid");

        struct __user_cap_header_struct cap_header;
        struct __user_cap_data_struct cap_data;

        memset(&cap_header, 0, sizeof(cap_header));
        memset(&cap_data, 0, sizeof(cap_data));

        cap_header.version = _LINUX_CAPABILITY_VERSION_3;
        cap_header.pid = 0; // no need for capset()

        __u32 cap_mask = 0;
        cap_mask |= (1 << CAP_NET_BIND_SERVICE);
        cap_data.effective = cap_data.permitted = cap_mask;
        cap_data.inheritable = 0;

        capset(&cap_header, &cap_data);

        char cmd;
        while (1) {
                cmd = priv_sock_get_cmd(sess->parent_fd);
                // parse inner cmd
                switch (cmd) {
                case PRIV_SOCK_GET_DATA_SOCK:
                        privop_pasv_get_data_sock(sess);
                        break;
                case PRIV_SOCK_PASV_ACTIVE:
                        privop_pasv_get_active(sess);
                        break;
                case PRIV_SOCK_PASV_LISTEN:
                        privop_pasv_get_listen(sess);
                        break;
                case PRIV_SOCK_PASV_ACCEPT:
                        privop_pasv_get_accept(sess);
                        break;
                }
        }
}


static void privop_pasv_get_data_sock(session_t *sess)
{
        int ret;

        unsigned short port = (unsigned short) priv_sock_get_int(sess->parent_fd);
        char ip[16] = {0};
        priv_sock_recv_buf(sess->parent_fd, ip, sizeof(ip));

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);

        // printf("2aaaaaaaaaaa:%s:%hu\n", ip, port);

        int fd = tcp_client(20);
        // printf("3aaaaaaaaaaa:%d\n", fd);
        if (fd == -1) {
                // priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
                priv_sock_send_result(sess->parent_fd, 0);
                return;
        }
        ret = connect_timeout(fd, &addr, tunable_connect_timeout);
        // ret = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
        // printf("4aaaaaaaaaaa:%d\n", ret);
        if (ret < 0) {
                close(fd);
                priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
                return;
        }

        priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_OK);
        priv_sock_send_fd(sess->parent_fd, fd);
        close(fd);
}

static void privop_pasv_get_active(session_t *sess)
{
        int active = 0;
        if (sess->pasv_listen_fd != -1) {
                active = 1;
        }

        priv_sock_send_int(sess->parent_fd, active);
}

static void privop_pasv_get_listen(session_t *sess)
{
        char ip[16] = "0.0.0.0";

        sess->pasv_listen_fd = tcp_server(ip, 0);
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        int ret = getsockname(sess->pasv_listen_fd, (struct sockaddr *) &addr, &addrlen);
        if (ret < 0)
                ERR_EXIT("privop_pasv_get_listen:getsockname");

        unsigned short port = ntohs(addr.sin_port);

        priv_sock_send_int(sess->parent_fd, (int) port);
}

static void privop_pasv_get_accept(session_t *sess)
{
        int fd = accept_timeout(sess->pasv_listen_fd, NULL, tunable_accept_timeout);
        close(sess->pasv_listen_fd);
        sess->pasv_listen_fd = -1;

        if (fd == -1) {
                priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
                return;
        }
        priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_OK);
        priv_sock_send_fd(sess->parent_fd, fd);
        close(fd);
}
