#include <myLinux.h>

#define NUM 10000000000
typedef long long LL;

LL sum;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *thr_fn(void *arg)
{
    LL end = (LL) arg, i, t = 0;

    for (i = 0; i < end; ++i)
        ++t;

    pthread_mutex_lock(&lock);
    sum += t;
    pthread_mutex_unlock(&lock);

    return arg;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <num>\n", argv[0]);
        exit(1);
    }

    int thread_num = atoi(argv[1]);
    int i;
    LL tar = NUM / thread_num;
    sum = NUM % thread_num;
    int time0 = time(NULL), time1;
    pthread_t tid[100];

    for (i = 0; i < thread_num; ++i) {
        pthread_create(&tid[i], NULL, thr_fn, (void *) tar);
    }
    for (i = 0; i < thread_num; ++i)
        pthread_join(tid[i], NULL);

    time1 = time(NULL);
    printf("result is %lld\ntime used: %d\n", sum, time1 - time0);

    pthread_mutex_destroy(&lock);
    return 0;
}
