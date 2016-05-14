#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 40

int main(int argc, char *argv[])
{
    FILE *in, *out;
    int ch;
    char name[LEN];

    if (argc < 2) {
        fprintf(stderr, "too few arguments!\n");
        exit(1);
    }

    if ((in = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "I couldn't open this file: %s\n", argv[1]);
        exit(2);
    }

    strcpy(name, argv[1]);
    strcat(name, ".bak");
    if ((out = fopen(name, "w")) == NULL) {
        fprintf(stderr, "Can't creat output file.\n");
        exit(3);
    }

    while ((ch = getc(in)) != EOF)
        putc(ch, out);

    if (fclose(in) || fclose(out))
        fprintf(stderr, "Error in closing files.\n");

    return 0;
}
