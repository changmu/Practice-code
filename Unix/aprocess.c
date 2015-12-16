#include "my_unix.h"

int main(int argc, char **argv)
{
    pid_t pid = atoi(argv[1]);
    kill(pid, SIGINT); 
    return 0;
}
