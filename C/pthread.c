#include <myLinux.h>

void *th_fun(void *arg)
{
    /*
    int *p = (int *) arg;

    printf("thread PID = %d\n", getpid());
    printf("thread ID = %lu\n", pthread_self());
    printf("thread *arg = %d\n", *p);
    */
    while (1) {
        sleep(19);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    int n = 10;
    int err;
    int cnt = 1;

    while (1) {
        err = pthread_create(&tid, NULL, th_fun, (void *) &n);
        if (err) {
            puts(strerror(err));
            break;
        }
        printf("main thread ID = %lu\n", pthread_self());
        printf("main child thread ID = %lu\n", tid);
        printf("main PID = %d\n", getpid());

        printf("cnt: %d\n", cnt++);
    }

    sleep(1);
    return 0;
}
