#include <myLinux.h>

#define NUM 5

/* implementing it with mutex */
pthread_mutex_t chopsticks_mutex[NUM];

void *thr_fn_mutex(void *arg)
{
    int id = (int) arg;

    int chop_num        = id;
    int chop_next_num   = (id + 1) % NUM;

    while (1) {
        pthread_mutex_lock(&chopsticks_mutex[chop_num]);
        printf("philosopher %d has got his chop, and now is waiting next one\n", id);

        if (pthread_mutex_trylock(&chopsticks_mutex[chop_next_num]) == EBUSY) {
            printf("philosopher %d couldn't get next chop, and put down the one he has\n", id);
            pthread_mutex_unlock(&chopsticks_mutex[chop_num]);
            sleep(rand() % 3);
        } else {
            printf("philosopher %d got both chops!!! and he has finished dining!!!!\n", id);
            pthread_mutex_unlock(&chopsticks_mutex[chop_num]);
            pthread_mutex_unlock(&chopsticks_mutex[chop_next_num]);

            pthread_exit(NULL);
        }
    }

    return arg;
}

void dinging_philosopher_mutex()
{
    int i;
    pthread_t tid[NUM];

    for (i = 0; i < NUM; ++i)
        pthread_mutex_init(&chopsticks_mutex[i], NULL);

    printf("dining starting!\n");
    for (i = 0; i < NUM; ++i)
        pthread_create(&tid[i], NULL, thr_fn_mutex, (void *) i);
    for (i = 0; i < NUM; ++i)
        pthread_join(tid[i], NULL);
    printf("dining finished!\n");

    for (i = 0; i < NUM; ++i)
        pthread_mutex_destroy(&chopsticks_mutex[i]);
}


/* implementing it with semaphore */
sem_t sem_chopsticks_num;
pthread_mutex_t sem_lock;

void *thr_fn_sem(void *arg)
{
    int id = (int) arg;

    while (1) {
        sem_wait(&sem_chopsticks_num);
        printf("philosopher %d has got his chop, and now is waiting next one\n", id);

        if (sem_trywait(&sem_chopsticks_num) == -1) {
            printf("philosopher %d couldn't get next chop, and put down the one he has\n", id);
            sem_post(&sem_chopsticks_num);
            sleep(rand() % 3);
        } else {
            printf("philosopher %d got both chops!!! and he has finished dining!!!!\n", id);
            sem_post(&sem_chopsticks_num);
            sem_post(&sem_chopsticks_num);

            pthread_exit(NULL);
        }
    }

    return arg;
}

void dinging_philosopher_sem()
{
    int i;
    pthread_t tid[NUM];

    sem_init(&sem_chopsticks_num, 0, NUM);
    pthread_mutex_init(&sem_lock, NULL);

    printf("sem dining starting!\n");
    for (i = 0; i < NUM; ++i)
        pthread_create(&tid[i], NULL, thr_fn_sem, (void *) i);
    for (i = 0; i < NUM; ++i)
        pthread_join(tid[i], NULL);
    printf("sem dining finished!\n");

    sem_destroy(&sem_chopsticks_num);
    pthread_mutex_destroy(&sem_lock);
}


/* implementing it with cond */
pthread_cond_t cond_chopstick = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cond_mutex;
int cond_chopsticks = NUM;

void *thr_fn_cond(void *arg)
{
    int id = (int) arg;
    int err;
    struct timespec outtime = {1, 0};

    while (1) {
        pthread_mutex_lock(&cond_mutex);

        if (cond_chopsticks == 0) {
            sleep(1);
            continue;
        }
        --cond_chopsticks;
        printf("philosopher %d has got his chop, and now is waiting next one\n", id);

        err = pthread_cond_timedwait(&cond_chopstick, &cond_mutex, &outtime);
        if (cond_chopsticks == 0 || err) {
            printf("philosopher %d couldn't get next chop, and put down the one he has\n", id);
            ++cond_chopsticks;
            pthread_cond_broadcast(&cond_chopstick);
            pthread_mutex_unlock(&cond_mutex);
        } else {
            --cond_chopsticks;
            printf("philosopher %d got both chops!!! and he has finished dining!!!!\n", id);
            ++cond_chopsticks;
            ++cond_chopsticks;
            pthread_cond_broadcast(&cond_chopstick);
            pthread_mutex_unlock(&cond_mutex);

            pthread_exit(NULL);
        }
    }

    return arg;
}

void dinging_philosopher_cond()
{
    // pthread_cond_init;
    pthread_t tid[NUM];
    int i;

    printf("cond dining starting!\n");
    for (i = 0; i < NUM; ++i)
        pthread_create(&tid[i], NULL, thr_fn_cond, (void *) i);
    pthread_cond_broadcast(&cond_chopstick);
    for (i = 0; i < NUM; ++i)
        pthread_join(tid[i], NULL);
    printf("cond dining finished!\n");

    pthread_cond_destroy(&cond_chopstick);
    pthread_mutex_destroy(&cond_mutex);
}

int main(int argc, char *argv[])
{
    // dinging_philosopher_mutex();
    // dinging_philosopher_sem();
    dinging_philosopher_cond();
    return 0;
}
