#include <myLinux.h>

int main(int argc, char *argv[])
{
    if (access(argv[1], F_OK) == 0)
        printf("File exists.\n");
    else 
        printf("No such a file.\n");
    return 0;
}
