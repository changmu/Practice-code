#include <myLinux.h>

struct msg {
    struct msg  *next;
    int         num;
};
struct msg *head;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *p)
{
    struct msg *mp;

    while (1) {
        pthread_mutex_lock(&lock);
        while (head == NULL)
            pthread_cond_wait(&has_product, &lock);
        mp = head;
        head = head->next;
        pthread_mutex_unlock(&lock);

        printf("Consume %d\n", mp->num);
        free(mp);
        sleep(rand() % 2);
    }
}

void *producer(void *p)
{
    struct msg *mp;

    while (1) {
        mp = malloc(sizeof (struct msg));
        mp->num = rand() % 100 + 1;
        printf("Produce %d\n", mp->num);

        pthread_mutex_lock(&lock);
        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&lock);

        pthread_cond_signal(&has_product);
        sleep(rand() % 5);
    }
}

int main(int argc, char *argv[])
{
    pthread_t ptid, ctid;

    pthread_create(&ptid, NULL, producer, NULL);
    pthread_create(&ctid, NULL, consumer, NULL);

    pthread_join(ptid, NULL);
    pthread_join(ctid, NULL);

    return 0;
}
