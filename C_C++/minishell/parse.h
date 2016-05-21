#pragma once

#include "myLinux.h"

void shell_loop(void);
int read_command(void);
int parse_command(void);
int execute_command(void);
int check(const char *str);
