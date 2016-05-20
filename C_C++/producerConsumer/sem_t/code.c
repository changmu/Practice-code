#include "myLinux.h"

#define CONSUMERS_COUNT 1
#define PRODUCERS_COUNT 5
#define BUFFIZE 10

int g_buffer[BUFFIZE];

unsigned in = 0;
unsigned out = 0;
unsigned produce_id = 0;
unsigned consume_id = 0;

sem_t g_sem_full;
sem_t g_sem_empty;
pthread_mutex_t g_mutex;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

void *consume(void *arg)
{
        int num = (int) arg;
        while (1) {
                printf("%d wait to consume..\n", num);
                sem_wait(&g_sem_empty);
                pthread_mutex_lock(&g_mutex);

                int i;
                for (i = 0; i < BUFFIZE; ++i) {
                        printf("%02d ", i);
                        if (g_buffer[i] == -1)
                                printf("%s", "null");
                        else
                                printf("%d", g_buffer[i]);

                        if (i == out)
                                printf("\t<--consume");
                        printf("\n");
                }

                consume_id = g_buffer[out];
                printf("begin consume product %d\n", consume_id);
                g_buffer[out] = -1;
                if (++out == BUFFIZE)
                        out = 0;
                printf("end consume product %d\n", consume_id);

                pthread_mutex_unlock(&g_mutex);
                sem_post(&g_sem_full);
                sleep(1);
        }
        return NULL;
}

void *produce(void *arg)
{
        int num = (int) arg;
        int i;
        while (1) {
                printf("%d wait to produce..\n", num);
                sem_wait(&g_sem_full);
                pthread_mutex_lock(&g_mutex);

                for (i = 0; i < BUFFIZE; ++i) {
                        printf("%02d ", i);
                        if (g_buffer[i] == -1)
                                printf("%s", "null");
                        else
                                printf("%d", g_buffer[i]);

                        if (i == in)
                                printf("\t<--produce");
                        printf("\n");
                }

                printf("begin produce product %d\n", produce_id);
                g_buffer[in] = produce_id;
                if (++in == BUFFIZE)
                        in = 0;
                printf("end produce product %d\n", produce_id++);

                pthread_mutex_unlock(&g_mutex);
                sem_post(&g_sem_empty);

                sleep(1);
        }

        return NULL;
}

int main(int argc, char *argv[])
{
        sem_init(&g_sem_full, 0, BUFFIZE);
        sem_init(&g_sem_empty, 0, 0);

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

        sem_destroy(&g_sem_empty);
        sem_destroy(&g_sem_full);
        pthread_mutex_destroy(&g_mutex);

        return 0;
}
