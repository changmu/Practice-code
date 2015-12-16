#ifndef CSV_H
#define CSV_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char *csvGetline(FILE *f);   // read next input line
extern char *csvField(int n);       // return field n
extern int csvNField(void);         // return number of fields

extern const int NOMEM;       // out of memory signal

extern char *line; // input chars
extern char *sLine; // line copy used by split
extern int maxLine;    // size of line[] and sLine[]
extern char **field; // field pointers
extern int maxField;    // size of field[]
extern int nField;    // number of fields in field[]

extern char fieldsep[];   // field separator chars

#endif
