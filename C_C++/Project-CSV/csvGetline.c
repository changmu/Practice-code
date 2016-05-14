#include "csv.h"
/* csvGetline: get one line, grow as needed 
 * sample input: "LU", 86.25, "11/4/1998", "2:19PM", +4.0625
 * */


static int endOfLine(FILE *fin, int c);
static int split(void);
static char *advquoted(char *p);
static void reset(void);

char *csvGetline(FILE *fin) {
    int i, c;
    char *newl, *news;

    if (line == NULL) {         // allocate on first call
        maxLine = maxField = 1;
        line = (char *) malloc(maxLine);
        sLine = (char *) malloc(maxLine);
        field = (char **) malloc(maxField * sizeof(field[0]));

        if (line == NULL || sLine == NULL || field == NULL) {
            reset();
            return NULL;    // out of memory
        }
    }

    for (i = 0; (c = getc(fin)) != EOF && !endOfLine(fin, c); ++i) {
        if (i >= maxLine - 1) {     // grow line
            maxLine <<= 1;          // double current size
            newl = (char *) realloc(line, maxLine);
            news = (char *) realloc(sLine, maxLine);
            if (newl == NULL || news == NULL) {
                reset();
                return NULL;        // out of memory
            }
            line = newl;
            sLine = news;
        }
        line[i] = c;
    }
    line[i] = '\0';
    if (split() == NOMEM) {
        reset();
        return NULL;
    }

    return (c == EOF && i == 0) ? NULL : line;
}

// reset: set variables back to starting values
static void reset(void) {
    free(line); // free(NULL) permitted by ANSI C
    free(sLine);
    free(field);
    line = NULL;
    sLine = NULL;
    field = NULL;
    maxLine = maxField = nField = 0;
}

// endOfLine: check for and consume \r, \n, \r\n, or EOF
static int endOfLine(FILE *fin, int c) {
    int eol;

    eol = (c == '\r' || c == '\n');
    if (c == '\r') {
        c = getc(fin);
        if (c != '\n' && c != EOF)
            ungetc(c, fin);     // read too far, put c back
    }

    return eol;
}

// split: split line into fields
static int split(void) {
    char *p, **newf;
    char *sepp;     // pointer to temporary separator character
    int sepc;       // temporary separator character

    nField = 0;
    if (line[0] == '\0')
        return 0;
    strcpy(sLine, line);
    p = sLine;

    do {
        if (nField >= maxField) {
            maxField <<= 1;     // double current size
            newf = (char **) realloc(field, maxField * sizeof(field[0]));
            if (newf == NULL)
                return NOMEM;
            field = newf;
        }
        if (*p == '"')
            sepp = advquoted(++p);      // skip initial quote
        else
            sepp = p + strcspn(p, fieldsep);
        sepc = sepp[0];
        sepp[0] = '\0';     // terminate field
        field[nField++] = p;
        p = sepp + 1;
    } while (sepc == ',');

    return nField;
}

// advquoted: quoted field; return pointer to next separator
static char *advquoted(char *p) {
    int i, j;

    for (i = j = 0; p[j] != '\0'; ++i, ++j) {
        if (p[j] == '"' && p[++j] != '"') {
            // copy up to next separator or '\0'
            int k = strcspn(p + j, fieldsep);
            memmove(p + i, p + j, k);
            i += k;
            j += k;
            break;
        }
        p[i] = p[j];
    }
    p[i] = '\0';

    return p + j;
}
