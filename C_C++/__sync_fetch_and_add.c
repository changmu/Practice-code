#include "myLinux.h"

#define N 4
#define TAR_SUM 10000000000

// __thread int64_t sum;
int64_t sum;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_routine(void *arg)
{
        const int64_t count = (int64_t) arg;
        int i;
        for (i = 0; i < count; ++i) {
                // pthread_mutex_lock(&mutex);
                __sync_fetch_and_add(&sum, 1);
                // ++sum;
                // pthread_mutex_unlock(&mutex);
        }
        return NULL;
}

int main(int argc, char *argv[])
{
        pthread_t pid[N];
        int i;

        int64_t begin = timestamp_now();
        for (i = 0; i < N; ++i) {
                pthread_create(&pid[i], NULL, thread_routine, TAR_SUM / N);
        }
        for (i = 0; i < N; ++i) {
                pthread_join(pid[i], NULL);
        }
        int64_t end = timestamp_now();
        printf("sum=%" PRId64 "\ttime used: %lfs\n", sum, timestamp_difference(end, begin));

        pthread_mutex_destroy(&mutex);

        return 0;
}
