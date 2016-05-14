#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1], "w+");
    char buf[1024] = "321";
    fwrite(buf, 1, 3, fp);
    buf[3] = '\0';
    // puts(buf);
    return 0;
}
