#pragma once

#include "common.h"
#include "session.h"

#define WELCOME "220 (miniftpd 0.1)\r\n"
#define USER_RESPONSE "331 Please specify the password.\r\n"
#define PASS_RESPONSE "230 Login successful.\r\n"
#define LOGIN_INCORRECT "530 Login incorrect.\r\n"

void handle_child(session_t *sess);

int list_common(session_t *sess, int detail);
int get_transfer_fd(session_t *sess);
void upload_common(session_t *sess, int isappe);

void ftp_reply(session_t *sess, int status, const char *text);
void ftp_lreply(session_t *sess, int status, const char *text);
