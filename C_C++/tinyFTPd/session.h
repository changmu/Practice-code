#pragma once

#include "common.h"

typedef struct session {
        // control connection
        uid_t uid;
        int ctrl_fd;
        char cmdline[MAX_COMMAND_LINE];
        char cmd[MAX_COMMAND];
        char arg[MAX_ARG];
        // data connection
        struct sockaddr_in *port_addr;
        int pasv_listen_fd;
        int data_fd;
        // parent and son's communication
        int parent_fd;
        int child_fd;
        // FTP协议状态
        int is_ascii;
        long long restart_pos;
        char *rnfr_name;
} session_t;

void begin_session(session_t *sess);