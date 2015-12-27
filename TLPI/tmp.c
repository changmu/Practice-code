#include <myLinux.h>

struct main {
    int a;
    int b;
};

int main(int argc, char **argv)
{
    struct main t = {3, 4};
    printf("%d %d\n", t.a, t.b);
    return 0;
}
