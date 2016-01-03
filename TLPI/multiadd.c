#include <myLinux.h>

#define NUM 1000000000
typedef long long LL;

LL sum;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *thr_fn(void *arg)
{
    while (sum != NUM) {
        pthread_mutex_lock(&lock);
        ++sum;
        pthread_mutex_unlock(&lock);
    }

    return (void *) 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <num>\n", argv[0]);
        exit(1);
    }

    int thread_num = atoi(argv[1]);
    int i;
    int time0 = time(NULL), time1;
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    for (i = 1; i < thread_num; ++i) {
        pthread_create(&tid, &attr, thr_fn, NULL);
    }

    while (sum != NUM) {
        pthread_mutex_lock(&lock);
        ++sum;
        pthread_mutex_unlock(&lock);
    }
    time1 = time(NULL);
    printf("time used: %d\n", time1 - time0);

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&lock);
    return 0;
}
