#include <myLinux.h>

int main(int argc, char *argv[])
{
        struct rlimit rl;

        if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
                ERR_EXIT("getrlimit");
        printf("%lu\n", rl.rlim_cur);
        printf("%lu\n", rl.rlim_max);

        rl.rlim_cur <<= 1;
        rl.rlim_max <<= 1;

        if (setrlimit(RLIMIT_NOFILE, &rl) < 0)
                ERR_EXIT("setrlimit");

        if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
                ERR_EXIT("getrlimit");
        printf("%lu\n", rl.rlim_cur);
        printf("%lu\n", rl.rlim_max);
        return 0;
}
