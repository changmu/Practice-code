/* 3个线程不定时写同一全局资源，5个线程不定时读同一全局资源 */
#include <myLinux.h>

int counter;
pthread_rwlock_t rwlock;

void *thr_write(void *arg)
{
    int tmp;
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        tmp = counter;
        usleep(100);
        printf("write %x  :counter: %d\t++counter%d\n", (unsigned) pthread_self(), tmp, ++counter);
        pthread_rwlock_unlock(&rwlock);
        usleep(10);
    }
    return (void *) 1;
}

void *thr_read(void *arg)
{
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("read %x  :%d\n", (unsigned) pthread_self(), counter);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return (void *) 1;
}

int main(int argc, char *argv[])
{
    pthread_rwlock_init(&rwlock, NULL);
    int i;
    pthread_t tid[8];

    for (i = 0; i < 3; ++i)
        pthread_create(&tid[i], NULL, thr_write, NULL);
    for (i = 0; i < 5; ++i)
        pthread_create(&tid[3+i], NULL, thr_read, NULL);

    pthread_rwlock_destroy(&rwlock);
    for (i = 0; i < 8; ++i)
        pthread_join(tid[i], NULL);

    return 0;
}
