#pragma once

#define MAXN 1024

enum {LOGIN, LOGOUT, CHAT, NOTONLINE};

struct Packet {
    int kind;
    int src;
    int dst;
    char *msg;
};

int parseBytesToPacket(const char *str, struct Packet *packet)
{
    static char buf[MAXN];
    char *cp;

    strcpy(buf, str);

    // read kind
    sscanf(buf, "%d", &packet->kind);
    cp = buf;

    // read src
    cp = strchr(buf, ' ') + 1;
    //! note not check cp's validity, if invalid, -1 should be returned
    sscanf(cp, "%d", &packet->src);
    // read dst
    cp = strchr(cp, ' ') + 1;
    sscanf(cp, "%d", &packet->dst);
    // read msg
    cp = strchr(cp, ' ') + 1;
    packet->msg = cp;

    return 0;
}
