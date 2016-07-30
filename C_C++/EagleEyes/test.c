#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //for exit(0);
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
 
int hostname_to_ip(char *  , char *);

static bool isPrivateIp(const char *ip)
{
    static const unsigned A_private = 0x0a000000;
    static const unsigned A_reserve = 0x7f000000;
    static const unsigned B_private = 0xac100000;
    static const unsigned B_reserve = 0xa9fe0000;
    static const unsigned C_private = 0xc0a80000;

    unsigned addr = ntohl((unsigned) inet_addr(ip));

    return (addr >> 24) == (A_private >> 24)
        || (addr >> 24) == (A_reserve >> 24)
        || (addr >> 20) == (B_private >> 20)
        || (addr >> 16) == (B_reserve >> 16)
        || (addr >> 16) == (C_private >> 16);
}

void getIpStat(char *ip)
{    
    struct ifaddrs *ifaStructPtr = NULL;
    struct ifaddrs *ifaPtr = NULL;
    struct in_addr addr;

    int ret = getifaddrs(&ifaStructPtr);

    int i = 0;
    for (ifaPtr = ifaStructPtr; ifaPtr != NULL; ifaPtr = ifaPtr->ifa_next) {
        if (!ifaPtr->ifa_addr || ifaPtr->ifa_addr->sa_family != AF_INET)
            continue;

        addr = ((struct sockaddr_in *) ifaPtr->ifa_addr)->sin_addr;
        strcpy(ip, inet_ntoa(addr));

        // strcpy(interfaceDevices[i].name, ifaPtr->ifa_name);

        // filter inner ip
        if (!isPrivateIp(ip)) {
            // printf("debug: ip %s\n", ip);
            break;
        }

    }

    freeifaddrs(ifaStructPtr);
}
 
int main(int argc , char *argv[])
{
    char ip[100];
    char hostname[1024] = { 0 };
    gethostname(hostname, sizeof(hostname));
    char *cp = strchr(hostname, '.');
    if (cp != NULL)
        *cp = '\0';
     
    // hostname_to_ip(hostname, ip);
    getIpStat(ip);
    printf("%s resolved to %s", hostname, ip);
     
    printf("\n");
     
}
/*
    Get ip from domain name
 */
 
int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}