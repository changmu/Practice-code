#include <myLinux.h>

void *thr_fn(void *arg)
{
    while (1)
        sleep(100);
}

int main(int argc, char *argv[])
{
    pthread_t   tid;
    int         cnt = 1;
    int         err;

    while (1) {
        err = pthread_create(&tid, NULL, thr_fn, NULL);
        if (err) {
            printf("%s\n", strerror(err));
            break;
        }
        printf("%d\n", cnt++);
    }
    return 0;
}
