/*
 * This header file is for the functions commonly used.
 * */
#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <utime.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <poll.h>

#include <math.h>
#include "wrap_socket.h"

// macro function
#define errExit(msg) \
    do { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    } while (0)

// function prototype
void mySizeFormat(unsigned long sz, char *buf);
