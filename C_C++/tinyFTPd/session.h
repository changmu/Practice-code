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
        int data_process;
        // 限速
        unsigned bw_upload_rate_max;
        unsigned bw_download_rate_max;
        long bw_transfer_start_sec;
        long bw_transfer_start_usec;
        
        // parent and son's communication
        int parent_fd;
        int child_fd;
        // FTP协议状态
        int is_ascii;
        long long restart_pos;
        char *rnfr_name;
        int abor_received;
} session_t;

void begin_session(session_t *sess);