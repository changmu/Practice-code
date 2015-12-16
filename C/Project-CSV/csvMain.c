#include "csv.h"

const int NOMEM = -2;       // out of memory signal

char *line   = NULL; // input chars
char *sLine  = NULL; // line copy used by split
int maxLine  = 0;    // size of line[] and sLine[]
char **field = NULL; // field pointers
int maxField = 0;    // size of field[]
int nField   = 0;    // number of fields in field[]

char fieldsep[] = ",";   // field separator chars

int main(void) {
    int i;
    char *line;

    while ((line = csvGetline(stdin)) != NULL) {
        printf("line = '%s'\n", line);
        for (i = 0; i < csvNField(); ++i)
            printf("field[%d] = '%s'\n", i, csvField(i));
    }

    return 0;
}
