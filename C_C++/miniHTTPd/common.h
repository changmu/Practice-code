#pragma once

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

#define DEFAULT_IP "0.0.0.0"
#define DEFAULT_PORT "8086"
#define DEFAULT_LOG "/tmp/miniHTTPd.log"
#define DEFAULT_ROOT "/home"