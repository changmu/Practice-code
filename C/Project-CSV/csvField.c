#include "csv.h"
// csvField: return pointer to n-th field
char *csvField(int n) {
    if (n < 0 || n >= nField)
        return NULL;

    return field[n];
}
