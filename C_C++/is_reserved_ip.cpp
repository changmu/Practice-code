#include "myLinux.h"

bool is_reserved_ip(const char *ip)
{
    static const char *address_blocks[] = {
        "0.0.0.0/8",
        "10.0.0.0/8",
        "100.64.0.0/10",
        "127.0.0.0/8",
        "169.254.0.0/16",
        "172.16.0.0/12",
        "192.0.0.0/24",
        "192.0.2.0/24",
        "192.88.99.0/24",
        "192.168.0.0/16",
        "198.18.0.0/15",
        "198.51.100.0/24",
        "203.0.113.0/24",
        "224.0.0.0/4",
        "240.0.0.0/4",
        "255.255.255.255/32"
    };

    static const int blk_nums = sizeof(address_blocks) / sizeof(*address_blocks);
    static char buf[40];

    for (int i = 0; i < blk_nums; ++i) {
        strncpy(buf, address_blocks[i], sizeof(buf));
        char *pos = strchr(buf, '/');
        assert(pos != NULL);
        *pos = '\0';
        uint32_t val = htonl(inet_addr(buf)); // big endian
        uint32_t msk = atoi(pos + 1);
        uint32_t offset = 32 - msk;

        uint32_t tar = htonl(inet_addr(ip));

        if ((val >> offset) == (tar >> offset))
            return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    const char *ip = "10.0.0.0";
    printf("be ip = %u\n", htonl(inet_addr(ip)));
    printf("le ip = %u\n", inet_addr(ip));
    char buf[40];

    while (~scanf("%s", buf)) {
        if (is_reserved_ip(buf))
            printf("%s is reserved ip.\n", buf); 
        else
            printf("public ip.\n");
    }
    return 0;
}
