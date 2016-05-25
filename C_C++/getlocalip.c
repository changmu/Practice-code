#include "myLinux.h"

int main(int argc, char *argv[])
{
        char ip[32];
        getlocalip(ip);
        printf("%s\n", ip);
        return 0;
}
