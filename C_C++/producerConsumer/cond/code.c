#include "myLinux.h"

#define CONSUMERS_COUNT 1
#define PRODUCERS_COUNT 5
#define BUFFIZE 10

int g_buffer[BUFFIZE];

unsigned in = 0;
unsigned out = 0;
unsigned produce_id = 0;
unsigned consume_id = 0;

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

int nready = 0;

void *consume(void *arg)
{
        int num = (int) arg;
        while (1) {
                pthread_mutex_lock(&g_mutex);
                while (nready == 0) {
                        printf("%d begin wait a condition..\n", num);
                        pthread_cond_wait(&g_cond, &g_mutex);
                }

                printf("%d end wait a condition...\n", num);
                printf("%d begin consume product..\n", num);
                --nready;
                printf("%d begin produce product..\n", num);

                pthread_mutex_unlock(&g_mutex);
                sleep(1);
        }

        return NULL;
}

void *produce(void *arg)
{
        int num = (int) arg;
        while (1) {
                pthread_mutex_lock(&g_mutex);

                printf("%d begin produce product...\n", num);
                ++nready;
                printf("%d end  produce product...\n", num);
                pthread_cond_signal(&g_cond);
                printf("%d signal...\n", num);

                pthread_mutex_unlock(&g_mutex);
                sleep(1);
        }

        return NULL;
}

int main(int argc, char *argv[])
{
        pthread_cond_init(&g_cond, NULL);
        pthread_mutex_init(&g_mutex, NULL);

        int i;
        for (i = 0; i < BUFFIZE; ++i)
                g_buffer[i] = -1;

        for (i = 0; i < CONSUMERS_COUNT; ++i)
                pthread_create(&g_thread[i], NULL, consume, (void *) i);
        for ( ; i < PRODUCERS_COUNT + CONSUMERS_COUNT; ++i)
                pthread_create(&g_thread[i], NULL, produce, (void *) i);

        for (i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; ++i)
                pthread_join(g_thread[i], NULL);

        pthread_mutex_destroy(&g_mutex);
        pthread_cond_destroy(&g_cond);

        return 0;
}
