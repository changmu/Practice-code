#include <myLinux.h>

void *thr_fn(void *arg)
{
    int n = 3;

    while (n--) {
        printf("thread count %d\n", n);
        sleep(1);
    }

    return (void *) 1;
}

int main(int argc, char *argv[])
{
    pthread_t   tid;
    void        *tret;
    int         err;

    pthread_create(&tid, NULL, thr_fn, NULL);
    // pthread_detach(tid);

    while (1) {
        err = pthread_join(tid, &tret);
        if (err != 0)
            fprintf(stderr, "thread %s\n", strerror(err));
        else
            fprintf(stderr, "thread exit code %d\n", (int) tret);
        sleep(1);
    }
    return 0;
}
