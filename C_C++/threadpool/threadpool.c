#include "threadpool.h"

void *thread_routine(void * arg)
{
        printf("thread %#x is starting..\n", (int) pthread_self()); 

        struct timespec abstime;
        int timeout;
        threadpool_t *pool = (threadpool_t *) arg;
        while (1) {
                timeout = 0;
                condition_lock(&pool->ready);
                ++pool->idle;
                /* waiting for task or destroy event */
                while (pool->first == NULL && pool->quit == 0) {
                        printf("thread %#x is waiting..\n", (int) pthread_self()); 
                        // condition_wait(&pool->ready);

                        clock_gettime(CLOCK_REALTIME, &abstime);
                        abstime.tv_sec += 2;
                        int status = condition_timedwait(&pool->ready, &abstime);
                        if (status == ETIMEDOUT) {
                                printf("thread %#x timedout..\n", (int) pthread_self()); 
                                timeout = 1;
                                break;
                        }
                }
                /* got condition, ready to work or be destroyed */
                --pool->idle;

                if (pool->first != NULL) {
                        /* get task from list */
                        task_t *t = pool->first;
                        pool->first = t->next;
                        /* exec need time, so unlock for others to add new
                         * task to pool or get task from pool */
                        condition_unlock(&pool->ready);

                        t->run(t->arg);
                        free(t);

                        condition_lock(&pool->ready);
                }

                if (pool->quit && pool->first == NULL) {
                        if (!--pool->counter) {
                                condition_signal(&pool->ready);
                        }

                        condition_unlock(&pool->ready);
                        break;
                }

                if (timeout) {
                        // condition_lock(&pool->ready);
                        --pool->counter;
                        condition_unlock(&pool->ready);
                        break;
                }

                condition_unlock(&pool->ready);
        }
        printf("thread %#x is exiting..\n", (int) pthread_self()); 

        return NULL;
}

void threadpool_init(threadpool_t *pool, int threads)
{
        condition_init(&pool->ready);
        pool->first = NULL;
        pool->last = NULL;
        pool->counter = 0;
        pool->idle = 0;
        pool->max_threads = threads;
        pool->quit = 0;
}

void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
        task_t *newtask = malloc(sizeof(task_t));
        newtask->run = run;
        newtask->arg = arg;
        newtask->next = NULL;

        condition_lock(&pool->ready);

        if (pool->first == NULL)
                pool->first = newtask;
        else
                pool->last->next = newtask;
        pool->last = newtask;

        if (pool->idle > 0) {
                condition_signal(&pool->ready);
        } else if (pool->counter < pool->max_threads) {
                /* no idle thread and current threads not exceed max */
                pthread_t tid;
                pthread_create(&tid, NULL, thread_routine, pool);
                ++pool->counter;
        }

        condition_unlock(&pool->ready);
}

void threadpool_destroy(threadpool_t *pool)
{
        if (pool->quit) {
                condition_unlock(&pool->ready);
                return;
        }
        condition_lock(&pool->ready);

        pool->quit = 1;
        if (pool->counter > 0) {
                if (pool->idle > 0)
                        condition_broadcast(&pool->ready);
                /* executing threads couldn't recv broadcast, so pool has to
                 * be wait*/
                while (pool->counter > 0) {
                        condition_wait(&pool->ready);
                }
        }

        condition_unlock(&pool->ready);

        condition_destroy(&pool->ready);
}
