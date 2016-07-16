#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(str) \
    do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    FILE *fp = popen("iostat", "r");
    FILE *txt = fopen("out.txt", "w");

    char buf[1024];

    while (fgets(buf, sizeof(buf), fp))
        fprintf(txt, "%s", buf);

    pclose(fp);
    fclose(txt);
    return 0;
}
