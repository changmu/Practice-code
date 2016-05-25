#pragma once

#include "common.h"
#include "session.h"

#define WELCOME "220 (miniftpd 0.1)\r\n"

void handle_child(session_t *sess);
