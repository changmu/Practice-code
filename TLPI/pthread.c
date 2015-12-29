#include <myLinux.h>

void *th_fun(void *arg)
{
    int *p = (int *) arg;

    printf("thread PID = %d\n", getpid());
    printf("thread ID = %lu\n", pthread_self());
    printf("thread *arg = %d\n", *p);
    sleep(1);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    int n = 10;

    pthread_create(&tid, NULL, th_fun, (void *) &n);
    printf("main thread ID = %lu\n", pthread_self());
    printf("main child thread ID = %lu\n", tid);
    printf("main PID = %d\n", getpid());

    sleep(1);
    return 0;
}
