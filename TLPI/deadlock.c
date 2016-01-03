#include <myLinux.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *thr_fn1(void *arg)
{
    pthread_mutex_lock(&mutex1);
        printf("thread1 acquired mutex1, and is waiting mutex2\n");
        sleep(1);
    pthread_mutex_lock(&mutex2);
        printf("thread1 got mutex2\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    exit(1);
    return (void *) 1;
}

void *thr_fn2(void *arg)
{
    pthread_mutex_lock(&mutex2);
        printf("thread2 acquired mutex2, and is waiting mutex1\n");
    pthread_mutex_lock(&mutex1);
        printf("thread2 got mutex1\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    exit(1);
    return (void *) 1;
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    pthread_create(&tid, NULL, thr_fn1, NULL);
    pthread_create(&tid, NULL, thr_fn2, NULL);
    pthread_exit(NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    sleep(3);
    return 0;
}
