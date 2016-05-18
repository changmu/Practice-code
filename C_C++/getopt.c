#include "myLinux.h"

int main(int argc, char *argv[])
{
        int opt;
        int n;
        while (1) {
                opt = getopt(argc, argv, "nt:s:");
                if (opt == -1)
                        break;

                switch (opt) {
                case 'n':
                        printf("-n\n");
                        break;
                case 't':
                        n = atoi(optarg);
                        printf("t = %d\n", n);
                        break;
                }
        }
        return 0;
}
