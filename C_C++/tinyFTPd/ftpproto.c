#include "ftpproto.h"
#include "tunable.h"
#include "str.h"
#include "ftpcodes.h"
#include "privsock.h"


int port_active(session_t *sess);
int pasv_active(session_t *sess);
int get_port_fd(session_t *sess);
int get_pasv_fd(session_t *sess);
void handle_sigurg(int sig);
void check_abor(session_t* sess);

static void do_user(session_t *sess);
static void do_pass(session_t *sess);
static void do_cwd(session_t *sess);
static void do_cdup(session_t *sess);
static void do_quit(session_t *sess);
static void do_port(session_t *sess);
static void do_pasv(session_t *sess);
static void do_type(session_t *sess);
// static void do_stru(session_t *sess);
// static void do_mode(session_t *sess);
static void do_retr(session_t *sess);
static void do_stor(session_t *sess);
static void do_appe(session_t *sess);
static void do_list(session_t *sess);
static void do_nlst(session_t *sess);
static void do_rest(session_t *sess);
static void do_abor(session_t *sess);
static void do_pwd(session_t *sess);
static void do_mkd(session_t *sess);
static void do_rmd(session_t *sess);
static void do_dele(session_t *sess);
static void do_rnfr(session_t *sess);
static void do_rnto(session_t *sess);
static void do_site(session_t *sess);
static void do_syst(session_t *sess);
static void do_feat(session_t *sess);
static void do_size(session_t *sess);
static void do_stat(session_t *sess);
static void do_noop(session_t *sess);
static void do_help(session_t *sess);

static void do_site_chmod(session_t *sess, char *chmod_arg);
static void do_site_umask(session_t *sess, char *chmod_arg);

typedef struct ftpcmd {
        const char *cmd;
        void (*cmd_handler)(session_t *sess);
} ftpcmd_t;

static ftpcmd_t ctrl_cmds[] = {
        /* 访问控制命令 */
        { "USER", do_user },
        { "PASS", do_pass },
        { "CWD", do_cwd },
        { "XCWD", do_cwd },
        { "CDUP", do_cdup },
        { "XCUP", do_cdup },
        { "QUIT", do_quit },
        { "ACCT", NULL },
        { "SMNT", NULL },
        { "REIN", NULL },
        /* 传输参数命令 */
        { "PORT", do_port },
        { "PASV", do_pasv },
        { "TYPE", do_type },
        { "STRU", NULL/*do_stru*/ },
        { "MODE", NULL/*do_mode*/ },
        /* 服务命令 */
        { "RETR", do_retr },
        { "STOR", do_stor },
        { "APPE", do_appe },
        { "LIST", do_list },
        { "NLST", do_nlst },
        { "REST", do_rest },
        { "ABOR", do_abor },
        { "\377\364\377\362ABOR", do_abor},
        { "PWD", do_pwd },
        { "XPWD", do_pwd },
        { "MKD", do_mkd },
        { "XMKD", do_mkd },
        { "RMD", do_rmd },
        { "XRMD", do_rmd },
        { "DELE", do_dele },
        { "RNFR", do_rnfr },
        { "RNTO", do_rnto },
        { "SITE", do_site },
        { "SYST", do_syst },
        { "FEAT", do_feat },
        { "SIZE", do_size },
        { "STAT", do_stat },
        { "NOOP", do_noop },
        { "HELP", do_help },
        { "STOU", NULL },
        { "ALLO", NULL}
};

session_t *p_sess;

void start_data_alarm();
void handle_alarm_timeout(int sig)
{
        shutdown(p_sess->ctrl_fd, SHUT_RD);
        ftp_reply(p_sess, FTP_IDLE_TIMEOUT, "Timeout.");
        shutdown(p_sess->ctrl_fd, SHUT_WR);

        exit(EXIT_FAILURE);
}

void handle_sigalrm(int sig)
{
        if (p_sess->data_process == 0) {
                ftp_reply(p_sess, FTP_DATA_TIMEOUT, "Data timeout. Reconnect");
                exit(EXIT_FAILURE);  
        } else {
                p_sess->data_process = 0;
                start_data_alarm();
        }
}

void start_data_alarm()
{
        if (tunable_data_connection_timeout > 0) {
                signal(SIGALRM, handle_sigalrm);
                alarm(tunable_data_connection_timeout);
        } else if (tunable_idle_session_timeout > 0) {
                alarm(0);
        }
}

void handle_sigurg(int sig)
{
        if (p_sess->data_fd == -1)
                return;

        char cmdline[1024] = {0};
        int ret = readline(p_sess->ctrl_fd, cmdline, sizeof(cmdline));
        if (ret <= 0)
                ERR_EXIT("readline");
        str_trim_crlf(cmdline);
        if (!strcmp(cmdline, "ABOR")
            || !strcmp(cmdline, "\377\364\377\362ABOR")) {
                p_sess->abor_received = 1;
                shutdown(p_sess->data_fd, SHUT_RDWR);
        } else {
                ftp_reply(p_sess, FTP_BADCMD, "unknown command.");
        }
}

void check_abor(session_t* sess)
{
        if (sess->abor_received) {
                sess->abor_received = 0;
                ftp_reply(p_sess, FTP_ABOROK, "ABOR successful.");
        }
}

void start_cmdio_alarm()
{
        if (tunable_idle_session_timeout > 0) {
                signal(SIGALRM, handle_alarm_timeout);
                alarm(tunable_idle_session_timeout);
        }
}

void handle_child(session_t *sess)
{
        int ret;

        // writen(sess->ctrl_fd, WELCOME, strlen(WELCOME));
        ftp_reply(sess, FTP_GREET, "(miniFTPd 0.1)");
        while (1) {
                memset(sess->cmdline, 0, sizeof(sess->cmdline));
                memset(sess->cmd, 0, sizeof(sess->cmd));
                memset(sess->arg, 0, sizeof(sess->arg));
                
                start_cmdio_alarm();
                ret = readline(sess->ctrl_fd, sess->cmdline, MAX_COMMAND_LINE - 1);
                if (ret < 0)
                        ERR_EXIT("readline");
                else if (ret == 0)
                        exit(EXIT_FAILURE);

                // printf("cmdline = [%s]\n", sess->cmdline);

                // wipe off \r\n
                str_trim_crlf(sess->cmdline);
                printf("cmdline = [%s]\n", sess->cmdline);
                // parse command and args
                str_split(sess->cmdline, sess->cmd, sess->arg, ' ');
                printf("cmd = [%s] arg = [%s]\n", sess->cmd, sess->arg);
                str_upper(sess->cmd);
                // handle ftp command
                /*
                if (!strcmp("USER", sess->cmd)) {
                        do_user(sess);
                } else if (!strcmp("PASS", sess->cmd)) {
                        do_pass(sess);
                }*/
                int i;
                int size = ARRLEN(ctrl_cmds);
                for (i = 0; i < size; ++i) {
                        if (strcmp(ctrl_cmds[i].cmd, sess->cmd) == 0) {
                                if (NULL != ctrl_cmds[i].cmd_handler)
                                        ctrl_cmds[i].cmd_handler(sess);
                                else
                                        ftp_reply(sess, FTP_COMMANDNOTIMPL, "unimplement command.");
                                break;
                        }
                }
                if (i == size) {
                        ftp_reply(sess, FTP_BADCMD, "unknown command.");
                }
        }
}

void limit_rate(session_t *sess, int bytes_transfered, int is_upload)
{
        sess->data_process = 1;
        // 睡眠时间 = (当前传输速度 / 最大传输速度 - 1) * 当前传输时间
        long curr_sec = get_time_sec();
        long curr_usec = get_time_usec();
        double elapsed = curr_sec - sess->bw_transfer_start_sec;
        elapsed += (curr_usec - sess->bw_transfer_start_usec) / (double) 1000000;
        if (elapsed <= 0)
                elapsed = 0.01;

        unsigned bw_rate = (unsigned) (bytes_transfered / elapsed);

        double rate_ratio;
        if (is_upload) {
                if (bw_rate > sess->bw_upload_rate_max)
                        rate_ratio = bw_rate / sess->bw_upload_rate_max;
                else {
                        sess->bw_transfer_start_sec = get_time_sec();
                        sess->bw_transfer_start_usec = get_time_usec();
                        return;
                }
        } else {
                if (bw_rate > sess->bw_download_rate_max)
                        rate_ratio = bw_rate / sess->bw_download_rate_max;
                else {
                        sess->bw_transfer_start_sec = get_time_sec();
                        sess->bw_transfer_start_usec = get_time_usec();
                        return;
                }
        }

        double pause_time;
        pause_time = (rate_ratio - 1.0) * elapsed;

        nano_sleep(pause_time);

        sess->bw_transfer_start_sec = get_time_sec();
        sess->bw_transfer_start_usec = get_time_usec();
}

void upload_common(session_t *sess, int isappe)
{
        // 创建数据连接
        int ret = get_transfer_fd(sess);
        if (ret == 0)
                return;

        long long offset = sess->restart_pos;
        sess->restart_pos = 0;

        int fd = open(sess->arg, O_WRONLY | O_CREAT, 0666);
        if (fd == -1) {
                ftp_reply(sess, FTP_UPLOADFAIL, "Failed to create file.");
                return;
        }

        ret = lock_file_write(fd);
        if (ret == -1) {
                ftp_reply(sess, FTP_UPLOADFAIL, "Failed to lock file.");
                return;
        }

        if (!isappe) {
                if (!offset)
                        ftruncate(fd, 0);
                lseek(fd, offset, SEEK_SET);
        } else {
                lseek(fd, 0, SEEK_END);
        }

        

        // 150
        char text[1024] = {0};
        if (sess->is_ascii) {
                sprintf(text, "Open ASCII mode data connection for %s.", sess->arg);
        } else {
                sprintf(text, "Open BINARY mode data connection for %s.", sess->arg);
        }
        ftp_reply(sess, FTP_DATACONN, text);

        // 上传文件
        int flag = 0;
        char buf[4096];


        // 睡眠时间 = (当前传输速度 / 最大传输速度 - 1) * 当前传输时间
        sess->bw_transfer_start_sec = get_time_sec();
        sess->bw_transfer_start_usec = get_time_usec();
        while (1) {
                ret = read(sess->data_fd, buf, sizeof(buf));
                if (ret == -1) {
                        if (errno == EINTR)
                                continue;
                        else {
                                flag = 2;
                                break;
                        }
                } else if (ret == 0) {
                        flag = 0;
                        break;
                }

                limit_rate(sess, ret, 1);

                if (sess->abor_received) {
                        flag = 2;
                        break;
                }

                if (writen(fd, buf, ret) != ret) {
                        flag = 1;
                        break;
                }
        }
        


        // 关闭数据套接字
        close(sess->data_fd);
        sess->data_fd = -1;
        close(fd);
        if (flag == 0 && !sess->abor_received) {
                ftp_reply(sess, FTP_TRANSFEROK, "File send OK.");
        } else if (flag == 1) {
                ftp_reply(sess, FTP_BADSENDFILE, "Faile writting file.");
        } else if (flag == 2) {
                ftp_reply(sess, FTP_BADSENDNET, "Faile reading socket.");
        }

        check_abor(sess);

        start_cmdio_alarm();
}

int port_active(session_t *sess)
{
        if (sess->port_addr) {
                if (pasv_active(sess)) {
                        fprintf(stderr, "both PORT and PASV are actived.\n");
                        exit(EXIT_FAILURE);
                }
                return 1;
        }
        return 0;
}

int pasv_active(session_t *sess)
{
        /*if (sess->pasv_listen_fd != -1) {
                if (port_active(sess)) {
                        fprintf(stderr, "both PORT and PASV are actived.\n");
                        exit(EXIT_FAILURE);
                }
                return 1;
        }*/
        priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_PASV_ACTIVE);
        int active = priv_sock_get_int(sess->child_fd);
        if (active) {
                if (port_active(sess)) {
                        fprintf(stderr, "both PORT and PASV are actived.\n");
                        exit(EXIT_FAILURE);
                }
                return 1;
        }

        return 0;
}

int get_port_fd(session_t *sess)
{
        priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_GET_DATA_SOCK);
        unsigned short port = ntohs(sess->port_addr->sin_port);
        char *ip = inet_ntoa(sess->port_addr->sin_addr);
        priv_sock_send_int(sess->child_fd, (int) port);
        priv_sock_send_buf(sess->child_fd, ip, strlen(ip));
        char res = priv_sock_get_result(sess->child_fd);
        // printf("1aaaaaaaaaaa:%s:%hu\n", ip, port);
        // printf("1aaaaaaaaaaaaaaaaaa:%d\n", res);
        if (res == PRIV_SOCK_RESULT_BAD) {
                return 0;
        } else if (res == PRIV_SOCK_RESULT_OK) {
                sess->data_fd = priv_sock_recv_fd(sess->child_fd);
                // printf("aaaaaaaaaaaaaaaaaa:%d\n", sess->data_fd);
        }

        return 1;
}

int get_pasv_fd(session_t *sess)
{
        priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_PASV_ACCEPT);
        char res = priv_sock_get_result(sess->child_fd);
        if (res == PRIV_SOCK_RESULT_BAD) {
                return 0;
        } else if (res == PRIV_SOCK_RESULT_OK) {
                sess->data_fd = priv_sock_recv_fd(sess->child_fd);
        }
        return 1;
}

int get_transfer_fd(session_t *sess)
{
        int ret = 1;
        // 检测是否收到PORT或PASS命令
        if (!port_active(sess) && !pasv_active(sess)) {
                ftp_reply(sess, FTP_BADSENDCONN, "Use PORT or PASV first.");
                return 0;
        }

        if (port_active(sess)) {
                if (!get_port_fd(sess))
                        ret = 0;

                free(sess->port_addr);
                sess->port_addr = NULL;
        }

        if (pasv_active(sess)) {
                /*int fd = accept_timeout(sess->pasv_listen_fd, NULL, tunable_accept_timeout);
                if (fd == -1)
                        return 0;

                close(sess->pasv_listen_fd);
                sess->data_fd = fd;*/
                if (!get_pasv_fd(sess))
                        ret = 0;
        }

        if (ret) {
                start_data_alarm();
        }

        return ret;
}

static void do_user(session_t *sess)
{
        // USER qiu
        struct passwd *pw = getpwnam(sess->arg);
        if (pw == NULL) {
                // 用户不存在
                // writen(sess->ctrl_fd, LOGIN_INCORRECT, strlen(LOGIN_INCORRECT));
                ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
                return;
        }

        sess->uid = pw->pw_uid;
        ftp_reply(sess, FTP_GIVEPWORD, "Please specify the password.");
        // writen(sess->ctrl_fd, USER_RESPONSE, strlen(USER_RESPONSE));
}

static void do_pass(session_t *sess)
{
        struct passwd *pw = getpwuid(sess->uid);
        if (pw == NULL) {
                // 用户不存在
                ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
                return;
        }

        struct spwd *sp = getspnam(pw->pw_name);
        if (sp == NULL) {
                ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
                return;
        }

        char *encrypted_pass = crypt(sess->arg, sp->sp_pwdp);
        if (strcmp(encrypted_pass, sp->sp_pwdp) != 0) {
                ftp_reply(sess, FTP_LOGINERR, "Login incorrect.");
                return;
        }

        signal(SIGURG, handle_sigurg);
        setSigurg(sess->ctrl_fd);

        umask(tunable_local_umask);
        setegid(pw->pw_gid);
        seteuid(pw->pw_uid);
        chdir(pw->pw_dir);

        ftp_reply(sess, FTP_LOGINOK, "Login successful.");
}

void ftp_reply(session_t *sess, int status, const char *text)
{
        char buf[1024] = {0};
        sprintf(buf, "%d %s\r\n", status, text);
        writen(sess->ctrl_fd, buf, strlen(buf));
}

void ftp_lreply(session_t *sess, int status, const char *text)
{
        char buf[1024] = {0};
        sprintf(buf, "%d-%s\r\n", status, text);
        writen(sess->ctrl_fd, buf, strlen(buf));
}


static void do_cwd(session_t *sess)
{
        int ret = chdir(sess->arg);
        if (ret < 0) {
                ftp_reply(sess, FTP_FILEFAIL, "Failed to change directory.");
                return;
        }
        ftp_reply(sess, FTP_CWDOK, "Directory successfully changed.");
}

static void do_cdup(session_t *sess)
{
        int ret = chdir("..");
        if (ret < 0) {
                ftp_reply(sess, FTP_FILEFAIL, "Failed to change directory.");
                return;
        }
        ftp_reply(sess, FTP_CWDOK, "Directory successfully changed.");
}

static void do_quit(session_t *sess)
{
        ftp_reply(sess, FTP_GOODBYE, "Goodbye.");
        exit(EXIT_SUCCESS);
}

static void do_port(session_t *sess)
{
        // PORT 192,168,220,1,12,108
        unsigned v[6];
        sscanf(sess->arg, "%u,%u,%u,%u,%u,%u", v+2, v+3, v+4, v+5, v+0, v+1);
        sess->port_addr = malloc(sizeof(struct sockaddr_in));
        memset(sess->port_addr, 0, sizeof(struct sockaddr_in));     
        
        sess->port_addr->sin_family = AF_INET;
        unsigned char *p = (unsigned char *) &sess->port_addr->sin_port;
        p[0] = v[0];
        p[1] = v[1];
        p = (unsigned char *) &sess->port_addr->sin_addr;
        p[0] = v[2];
        p[1] = v[3];
        p[2] = v[4];
        p[3] = v[5];

        ftp_reply(sess, FTP_PORTOK, "PORT command successful, Consider using PASV");
}

static void do_pasv(session_t *sess)
{
        char ip[16] = "0.0.0.0";
        // char ip[16] = "192.168.220.129";
        // getlocalip(ip);
        /*printf("getip=[%s]\n", ip);
        sess->pasv_listen_fd = tcp_server(ip, 0);
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        int ret = getsockname(sess->pasv_listen_fd, (struct sockaddr *) &addr, &addrlen);
        if (ret < 0)
                ERR_EXIT("getsockname");

        unsigned short port = ntohs(addr.sin_port);*/

        priv_sock_send_cmd(sess->child_fd, PRIV_SOCK_PASV_LISTEN);
        unsigned short port = priv_sock_get_int(sess->child_fd);

        unsigned v[4];
        sscanf(ip, "%u.%u.%u.%u", v+0, v+1, v+2, v+3);
        char text[1024] = {0};
        sprintf(text, "Entering Passive Mode (%u,%u,%u,%u,%u,%u).",
                v[0], v[1], v[2], v[3], port >> 8, port & 0xff);

        ftp_reply(sess, FTP_PASVOK, text);
}

static void do_type(session_t *sess)
{
        if (!strcmp(sess->arg, "A")) {
                sess->is_ascii = 1;
                ftp_reply(sess, FTP_TYPEOK, "Switching to ASCII mode.");
        } else if (!strcmp(sess->arg, "I")) {
                sess->is_ascii = 0;
                ftp_reply(sess, FTP_TYPEOK, "Switching to Binary mode.");
        } else {
                ftp_reply(sess, FTP_BADCMD, "Unrecognized TYPE command.");
        }
}

/*
static void do_stru(session_t *sess)
{

}

static void do_mode(session_t *sess)
{

}
*/
static void do_retr(session_t *sess)
{
        // 创建数据连接
        int ret = get_transfer_fd(sess);
        if (ret == 0)
                return;

        long long offset = sess->restart_pos;
        sess->restart_pos = 0;

        int fd = open(sess->arg, O_RDONLY);
        if (fd == -1) {
                ftp_reply(sess, FTP_FILEFAIL, "Failed to open file.");
                return;
        }

        ret = lock_file_read(fd);
        if (ret == -1) {
                ftp_reply(sess, FTP_FILEFAIL, "Failed to lock file.");
                return;
        }

        struct stat sbuf;
        ret = fstat(fd, &sbuf);
        if (!S_ISREG(sbuf.st_mode)) {
                ftp_reply(sess, FTP_FILEFAIL, "Failed to stat file.");
                return;
        }

        if (offset) {
                ret = lseek(fd, offset, SEEK_SET);
                if (ret == -1) {
                        ftp_reply(sess, FTP_FILEFAIL, "Failed to open file.");
                        return;
                }
        }

        // 150
        char text[1024] = {0};
        if (sess->is_ascii) {
                sprintf(text, "Open ASCII mode data connection for %s (%lld bytes).", 
                        sess->arg, (long long) sbuf.st_size);
        } else {
                sprintf(text, "Open BINARY mode data connection for %s (%lld bytes).", 
                        sess->arg, (long long) sbuf.st_size);
        }
        ftp_reply(sess, FTP_DATACONN, text);

        // 下载文件
        int flag = 0;
        /*char buf[4096];
        while (1) {
                ret = read(fd, buf, sizeof(buf));
                if (ret == -1) {
                        if (errno == EINTR)
                                continue;
                        else {
                                flag = 1;
                                break;
                        }
                } else if (ret == 0) {
                        flag = 0;
                        break;
                }

                if (writen(sess->data_fd, buf, ret) != ret) {
                        flag = 2;
                        break;
                }
        }*/
        long long bytes_to_send = sbuf.st_size;
        if (offset > bytes_to_send) {
                bytes_to_send = 0;
        } else {
                bytes_to_send -= offset;
        }

        sess->bw_transfer_start_sec = get_time_sec();
        sess->bw_transfer_start_usec = get_time_usec();
        while (bytes_to_send) {
                int num_this_time = bytes_to_send > 4096 ? 4096 : bytes_to_send;
                ret = sendfile(sess->data_fd, fd, NULL, num_this_time);
                if (ret == -1) {
                        flag = 2;
                        break;
                }

                limit_rate(sess, ret, 0);

                if (sess->abor_received) {
                        flag = 2;
                        break;
                }

                bytes_to_send -= ret;
        }


        // 关闭数据套接字
        close(sess->data_fd);
        sess->data_fd = -1;
        close(fd);


        if (flag == 0 && !sess->abor_received) {
                ftp_reply(sess, FTP_TRANSFEROK, "File send OK.");
        } else if (flag == 1) {
                ftp_reply(sess, FTP_BADSENDFILE, "Faile reading file.");
        } else if (flag == 2) {
                ftp_reply(sess, FTP_BADSENDNET, "Faile writting to socket.");
        }

        check_abor(sess);

        start_cmdio_alarm();
}

static void do_stor(session_t *sess)
{
        upload_common(sess, 0);
}

static void do_appe(session_t *sess)
{
        upload_common(sess, 1);
}

static void do_list(session_t *sess)
{
        // 创建数据连接
        int ret = get_transfer_fd(sess);
        if (ret == 0)
                return;
        // 150
        ftp_reply(sess, FTP_DATACONN, "Here comes the directory listing.");
        // 传输列表
        list_common(sess, 1);
        // 关闭数据套接字
        close(sess->data_fd);
        sess->data_fd = -1;
        // 226
        ftp_reply(sess, FTP_TRANSFEROK, "Directory send OK.");
}

static void do_nlst(session_t *sess)
{
        // 创建数据连接
        int ret = get_transfer_fd(sess);
        if (ret == 0)
                return;
        // 150
        ftp_reply(sess, FTP_DATACONN, "Here comes the directory listing.");
        // 传输列表
        list_common(sess, 0);
        // 关闭数据套接字
        close(sess->data_fd);
        sess->data_fd = -1;
        // 226
        ftp_reply(sess, FTP_TRANSFEROK, "Directory send OK.");
}

static void do_rest(session_t *sess)
{
        sess->restart_pos = atoll(sess->arg);

        char text[1024] = {0};
        sprintf(text, "Restart position accepted (%s).", sess->arg);
        ftp_reply(sess, FTP_RESTOK, text);
}

static void do_abor(session_t *sess)
{
        ftp_reply(sess, FTP_ABOR_NOCONN, "No transfer to ABOR");
}

static void do_pwd(session_t *sess)
{
        char text[1024] = {0};
        char dir[1024 + 1] = {0};
        getcwd(dir, 1024);
        sprintf(text, "\"%s\"", dir);

        ftp_reply(sess, FTP_PWDOK, text);
}

static void do_mkd(session_t *sess)
{
        int ret = mkdir(sess->arg, 0777);
        if (ret < 0) {
                ftp_reply(sess, FTP_FILEFAIL, "Create directory operation failed.");
                return;
        }

        char text[4096] = {0};
        if (sess->arg[0] == '/') {
                sprintf(text, "%s created", sess->arg);
        } else { 
                char dir[4096 + 1] = {0};
                getcwd(dir, 4096);
                if (dir[strlen(dir) - 1] != '/')
                        strcat(dir, "/");
                sprintf(text, "%s%s created", dir, sess->arg);
        }
        ftp_reply(sess, FTP_MKDIROK, text);
}

static void do_rmd(session_t *sess)
{
        int ret = rmdir(sess->arg);
        if (ret < 0) {
                ftp_reply(sess, FTP_FILEFAIL, "Remove directory failed.");
                return;
        }
        ftp_reply(sess, FTP_RMDIROK, "Remove directory successful.");
}

static void do_dele(session_t *sess)
{
        int ret = unlink(sess->arg);
        if (ret < 0) {
                ftp_reply(sess, FTP_FILEFAIL, "Delete operation failed.");
                return;
        }
        ftp_reply(sess, FTP_DELEOK, "Delete operation successful.");
}

static void do_rnfr(session_t *sess)
{
        sess->rnfr_name = malloc(strlen(sess->arg) + 1);
        memset(sess->rnfr_name, 0, strlen(sess->rnfr_name) + 1);
        strcpy(sess->rnfr_name, sess->arg);
        ftp_reply(sess, FTP_RNFROK, "Ready for RNTO.");
}

static void do_rnto(session_t *sess)
{
        if (sess->rnfr_name == NULL) {
                ftp_reply(sess, FTP_NEEDRNFR, "RNFR required first.");
                return;
        }
        rename(sess->rnfr_name, sess->arg);
        ftp_reply(sess, FTP_RENAMEOK, "Rename successful");

        free(sess->rnfr_name);
        sess->rnfr_name = NULL;
}

static void do_site(session_t *sess)
{
    char cmd[100] = {0};
    char arg[100] = {0};

    str_split(sess->arg, cmd, arg, ' ');
    if (!strcmp(cmd, "CHMOD")) {
        printf("debug:arg[%s]\n", arg);
        do_site_chmod(sess, arg);
    } else if (!strcmp(cmd, "UMASK")) {
        do_site_umask(sess, arg);
    } else if (!strcmp(cmd, "HELP")) {
        ftp_reply(sess, FTP_SITEHELP, "CHMOD UMASK HELP");
    } else {
        ftp_reply(sess, FTP_BADCMD, "unknown SITE cmd.");
    }
}

static void do_syst(session_t *sess)
{
        ftp_reply(sess, FTP_SYSTOK, "UNIX Type L9");
}

static void do_feat(session_t *sess)
{
        ftp_lreply(sess, FTP_FEAT, "Features:");
        writen(sess->ctrl_fd, " EPRT\r\n", strlen(" EPRT\r\n"));
        writen(sess->ctrl_fd, " EPSV\r\n", strlen(" EPSV\r\n"));
        writen(sess->ctrl_fd, " MDTM\r\n", strlen(" MDTM\r\n"));
        writen(sess->ctrl_fd, " PASV\r\n", strlen(" PASV\r\n"));
        writen(sess->ctrl_fd, " REST STREAM\r\n", strlen(" REST STREAM\r\n"));
        writen(sess->ctrl_fd, " SIZE\r\n", strlen(" SIZE\r\n"));
        writen(sess->ctrl_fd, " TVFS\r\n", strlen(" TVFS\r\n"));
        writen(sess->ctrl_fd, " UTF8\r\n", strlen(" UTF8\r\n"));
        ftp_reply(sess, FTP_FEAT, "End");
}

static void do_size(session_t *sess)
{
        struct stat buf;
        if (stat(sess->arg, &buf) < 0) {
                ftp_reply(sess, FTP_FILEFAIL, "SIZE operation failed");
                return;
        }

        if (!S_ISREG(buf.st_mode)) {
                ftp_reply(sess, FTP_FILEFAIL, "Could not get file size.");
                return;
        }

        char text[1024] = {0};
        sprintf(text, "%lld", (long long) buf.st_size);
        ftp_reply(sess, FTP_SIZEOK, text);
}

static void do_stat(session_t *sess)
{
    ftp_lreply(sess, FTP_STATOK, "FTP server status:");
    writen(sess->ctrl_fd, "TYPE: ACSII...\r\n", strlen("TYPE: ACSII...\r\n"));
    ftp_reply(sess, FTP_STATOK, "End of status.");
}

static void do_noop(session_t *sess)
{
        ftp_reply(sess, FTP_NOOPOK, "NOOP ok.");
}

static void do_help(session_t *sess)
{
    ftp_lreply(sess, FTP_HELP, "The following commands are recognized.");
    writen(sess->ctrl_fd, "ABOR ACCT...\r\n", strlen("ABOR ACCT...\r\n"));
    ftp_reply(sess, FTP_HELP, "Help OK.");
}


int list_common(session_t *sess, int detail)
{
        DIR *dir = opendir(".");
        if (dir == NULL)
                return 0;

        struct dirent *dt;
        struct stat sbuf;
        while ((dt = readdir(dir)) != NULL) {
                if (lstat(dt->d_name, &sbuf) < 0)
                        continue;
                if (dt->d_name[0] == '.')
                        continue;

                char buf[1024] = {0};
                int off = 0;
                if (detail) {
                        char perms[] = "----------";
                        perms[0] = '?';

                        mode_t mode = sbuf.st_mode;
                        switch (mode & S_IFMT) {
                        case S_IFSOCK:
                                perms[0] = 's';
                                break;
                        case S_IFREG:
                                perms[0] = '-';
                                break;
                        case S_IFDIR:
                                perms[0] = 'd';
                                break;
                        case S_IFLNK:
                                perms[0] = 'l';
                                break;
                        case S_IFBLK:
                                perms[0] = 'b';
                                break;
                        case S_IFCHR:
                                perms[0] = 'c';
                                break;
                        case S_IFIFO:
                                perms[0] = 'p';
                                break;
                        }

                        if (mode & S_IRUSR)
                                perms[1] = 'r';
                        if (mode & S_IWUSR)
                                perms[2] = 'w';
                        if (mode & S_IXUSR)
                                perms[3] = 'x';

                        if (mode & S_IRGRP)
                                perms[4] = 'r';
                        if (mode & S_IWGRP)
                                perms[5] = 'w';
                        if (mode & S_IXGRP)
                                perms[6] = 'x';

                        if (mode & S_IROTH)
                                perms[7] = 'r';
                        if (mode & S_IWOTH)
                                perms[8] = 'w';
                        if (mode & S_IXOTH)
                                perms[9] = 'x';

                        if (mode & S_ISUID)
                                perms[3] = (perms[3] == 'x') ? 's' : 'S';
                        if (mode & S_ISGID)
                                perms[6] = (perms[6] == 'x') ? 's' : 'S';
                        if (mode & S_ISVTX)
                                perms[9] = (perms[9] == 'x') ? 't' : 'T';

                        off = sprintf(buf, "%s ", perms);
                        off += sprintf(buf + off, " %3d %-8d %-8d", (int) sbuf.st_nlink, (int) sbuf.st_uid, (int) sbuf.st_gid);
                        off += sprintf(buf + off, "%8lu ", (unsigned long) sbuf.st_size);

                        const char *p_date_format = "%b %e %H:%M";
                        struct timeval tv;
                        gettimeofday(&tv, NULL);
                        time_t local_time = tv.tv_sec;
                        if (local_time - sbuf.st_mtime > 182 * 24 * 60 * 60)
                                p_date_format = "%b %e  %Y";

                        char datebuf[64] = {0};
                        struct tm *p_tm = localtime(&local_time);
                        strftime(datebuf, sizeof(datebuf), p_date_format, p_tm);
                        off += sprintf(buf + off, "%s", datebuf);
                }

                if (!S_ISLNK(sbuf.st_mode))
                        off += sprintf(buf + off, " %s\r\n", dt->d_name);
                else {
                        char tmp[1024] = {0};
                        readlink(dt->d_name, tmp, sizeof(tmp));
                        off += sprintf(buf + off, " %s -> %s\r\n", dt->d_name, tmp);
                }


                // printf("%s", buf);
                writen(sess->data_fd, buf, strlen(buf));
        }

        closedir(dir);
        return 1;
}

static void do_site_chmod(session_t *sess, char *chmod_arg)
{
        if (!strlen(chmod_arg)) {
                ftp_reply(sess, FTP_BADCMD, "need 2 args");
                return;
        }

        char perm[100] = {0};
        char file[100] = {0};
        str_split(chmod_arg, perm, file, ' ');
        if (!strlen(file)) {
                ftp_reply(sess, FTP_BADCMD, "need 2 args");
                return;
        }

        unsigned mode = str_octal_to_uint(perm);
        if (chmod(file, mode) < 0) {
                ftp_reply(sess, FTP_BADCMD, "chmod failed");
        } else {
                ftp_reply(sess, FTP_CHMODOK, "chmod ok");
        }
}

static void do_site_umask(session_t *sess, char *chmod_arg)
{
        if (!strlen(chmod_arg)) {
                static char text[1024] = {0};
                sprintf(text, "umask is %#o", tunable_local_umask);
                ftp_reply(sess, FTP_UMASKOK, text);
        } else {
                unsigned um = str_octal_to_uint(chmod_arg);
                umask(um);
                static char text[1024] = {0};
                sprintf(text, "umask set to %#o", um);
                ftp_reply(sess, FTP_UMASKOK, text);
        }
}
