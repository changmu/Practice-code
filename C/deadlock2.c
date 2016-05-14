#include <myLinux.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thr_fn(void *arg)
{
    pthread_mutex_lock(&mutex);
    printf("Got it\n");
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);

    return (void *) 1;
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    pthread_create(&tid, NULL, thr_fn, NULL);
    pthread_join(tid, NULL);

    return 0;
}
