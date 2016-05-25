#pragma once

#include "common.h"

typedef struct session {
        // control connection
        int ctrl_fd;
        char cmdline[MAX_COMMAND_LINE];
        char cmd[MAX_COMMAND];
        char arg[MAX_ARG];
        // parent and son's communication
        int parent_fd;
        int child_fd;
} session_t;

void begin_session(session_t *sess);