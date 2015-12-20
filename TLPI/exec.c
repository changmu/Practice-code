// #include <myLinux.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char * const ls_argv[] = {"ls", "-l", NULL};
    char * const ls_envp[] = {"PATH=/bin:/home", "TERM=console", NULL};

    execve("/bin/ls", ls_argv, ls_envp);
    return 0;
}
