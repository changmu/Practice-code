#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp = fopen("/dev/shm/lb_transmit_node.pid", "w");
    if (!fp)
    {
        fprintf(stderr, "opening /dev/shm/lb_transmit_node.pid failed\n");
        return -1;
    }

    char buf[8] = {0};
    snprintf(buf, sizeof(buf), "%d", 10);
    fwrite(buf, sizeof(char), strlen(buf), fp);
    fclose(fp); 

    return 0;
}
