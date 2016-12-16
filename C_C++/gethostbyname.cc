#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libgen.h>

// extern int h_errno;

int getIpByHost(const char *host, struct in_addr *ip)
{
    struct hostent *hp = gethostbyname(host);
    if (hp == NULL)
        return -1;

    *ip = *(struct in_addr *) hp->h_addr;

    return 0;
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s qq.com\n", argv[0]);
        return 1;
    }
    struct in_addr ip;
    if (!getIpByHost(argv[1], &ip))
        printf("%s: %s\n", argv[1], inet_ntoa(ip));
    else
        printf("err.\n");
    return 0;
}
