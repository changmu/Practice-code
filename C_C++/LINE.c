#include "myLinux.h"

int main(int argc, char *argv[])
{
    printf("line: %d..%s..%s..%s..%d..\n", __LINE__, __FILE__, __func__, __TIME__); 
    printf("line: %d\n", __LINE__); 
    return 0;
}
