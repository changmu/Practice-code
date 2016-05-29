#include "common.h"
#include "session.h"
#include "ftpproto.h"
#include "privaparent.h"
#include "str.h"
#include "parseconfig.h"
#include "tunable.h"

int main(int argc, char *argv[])
{
        signal(SIGCHLD, SIG_IGN);
        if (getuid() != 0) {
                fprintf(stderr, "miniftp: must be started as root\n");
                exit(EXIT_FAILURE);
        } 
        parseconf_load_file(MINIFTP_CONF_PATH);
        /*================testcode begin=============================*/
        /*char str1[] = "  a b";
        char str2[] = "             ";
        if (str_all_space(str1))
                printf("str_all_space\n");
        else
                printf("not str_all_space\n");
        if (str_all_space(str2))
                printf("str_all_space\n");
        else
                printf("not str_all_space\n");

        

        char str3[] = "abcDef";
        str_upper(str3);
        printf("str3=[%s]\n", str3);

        char str4[] = "1234567891012";
        printf("str4=%lld\n", str_to_longlong(str4));

        printf("str5=%u\n", str_octal_to_uint("0711"));*/
        /*================testcode end=============================*/
/*
extern int tunable_pasv_enable;
extern int tunable_port_enable;
extern unsigned tunable_listen_port;
extern unsigned tunable_max_clients;
extern unsigned tunable_max_per_ip;
extern unsigned tunable_accept_timeout;
extern unsigned tunable_connect_timeout;
extern unsigned tunable_idle_session_timeout;
extern unsigned tunable_data_connection_timeout;
extern unsigned tunable_local_umask;
extern unsigned tunable_upload_max_rate;
extern unsigned tunable_download_max_rate;
extern const char *tunable_listen_addr;
*/

printf("tunable_pasv_enable=%d\n", tunable_pasv_enable);
printf("tunable_port_enable=%d\n", tunable_port_enable);
printf("tunable_listen_port=%u\n", tunable_listen_port);
printf("tunable_max_clients=%u\n", tunable_max_clients);
printf("tunable_max_per_ip=%u\n", tunable_max_per_ip);
printf("tunable_accept_timeout=%u\n", tunable_accept_timeout);
printf("tunable_connect_timeout=%u\n", tunable_connect_timeout);
printf("tunable_idle_session_timeout=%u\n", tunable_idle_session_timeout);
printf("tunable_data_connection_timeout=%u\n", tunable_data_connection_timeout);
printf("tunable_local_umask=%#o\n", tunable_local_umask);
printf("tunable_upload_max_rate=%u\n", tunable_upload_max_rate);
printf("tunable_download_max_rate=%u\n", tunable_download_max_rate);

if (tunable_listen_addr == NULL)
        printf("tunable_download_max_rate=null\n");
else
        printf("tunable_listen_addr=%s\n", tunable_listen_addr);
        // list_common(sess);






        session_t sess = {
                -1, -1, 
                "", "", "",
                NULL, -1, -1,
                -1, -1, 0, 0, NULL
        };

        int listenfd = tcp_server(tunable_listen_addr, tunable_listen_port);
        int conn;
        pid_t pid;

        while (1) {
                conn = accept_timeout(listenfd, NULL, 0);
                if (conn == -1)
                        ERR_EXIT("accept_timeout");

                pid = fork();
                if (pid == -1)
                        ERR_EXIT("fork");

                if (pid == 0) {
                        close(listenfd);
                        sess.ctrl_fd = conn;
                        begin_session(&sess);
                } else {
                        close(conn);
                }
        }

        return 0;
}
