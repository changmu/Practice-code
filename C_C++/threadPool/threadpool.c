#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "threadpool.h"

#define DEFAULT_TIME 10     /* 10s检测一次 */
#define MIN_WAIT_TASK_NUM 10 /* 如果queue_size > MIN_WAIT_TASK_NUM 添加新的线程到线程池 */
#define DEFAULT_THREAD_VARY 10
#define true 1
#define false 0

#define errExit(str) \
    do { \
        perror(str); \
        exit(1); \
    } while (0)

typedef struct {
    void *(*function)(void *);
    void *arg;
} threadpool_task_t;

struct threadpool_t {
    pthread_mutex_t lock;               /* 用于锁住当前这个结构体 */
    pthread_mutex_t thread_counter;     /* 记录忙状态线程个数 */
    pthread_cond_t queue_not_full;      /* 任务队列不满 */
    pthread_cond_t queue_not_empty;     /* 任务队列不空 */
    pthread_t *threads;                 /* 保存工作线程tid的数组 */
    pthread_t adjust_tid;               /* 管理线程的tid */
    threadpool_task_t *task_queue;      /* 任务队列 */
    int min_thr_num;                    /* 线程组内默认最小线程数 */
    int max_thr_num;                    /* 线程组内默认最大线程数 */
    int live_thr_num;                   /* 当前存活线程个数 */
    int busy_thr_num;                   /* 忙状态线程个数 */
    int wait_exit_thr_num;              /* 要销毁的线程个数 */
    int queue_front;                    /* 队头索引下标 */
    int queue_rear;                     /* 队末索引下标 */
    int queue_size;                     /* 队中元素个数 */
    int queue_max_size;                 /* 队列容量 */
    int shutdown;                       /* 线程池使用状态 */
};

/* the worker thread*/
void *threadpool_thread(void *threadpool);
/* manager thread */
void *adjust_thread(void *threadpool);
/* check if a thread is alive */
int is_thread_alive(pthread_t tid);
int threadpool_free(threadpool_t *pool);

threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size)
{
    int i;
    threadpool_t *pool = NULL;

    do {
        if ((pool = (threadpool_t *) malloc(sizeof (threadpool_t))) == NULL) {
            printf("malloc threadpool fail");
            break;
        }

        pool->min_thr_num = min_thr_num;
        pool->max_thr_num = max_thr_num;
        pool->busy_thr_num = 0;
        pool->live_thr_num = min_thr_num;
        pool->queue_size = 0;
        pool->queue_max_size = queue_max_size;
        pool->queue_front = 0;
        pool->queue_rear = 0;
        pool->shutdown = false;
        pool->threads = (pthread_t *) calloc(1, sizeof (pthread_t) * max_thr_num);
        if (pool->threads == NULL) {
            printf("calloc threads fail\n");
            break;
        }

        pool->task_queue = (threadpool_task_t *) malloc(sizeof (threadpool_task_t ) * queue_max_size);
        if (pool->task_queue == NULL) {
            printf("malloc task_queue fail\n");
            break;
        }

        if (pthread_mutex_init(&pool->lock, NULL) != 0
                || pthread_mutex_init(&pool->thread_counter, NULL) != 0
                || pthread_cond_init(&pool->queue_not_empty, NULL) != 0
                || pthread_cond_init(&pool->queue_not_full, NULL) != 0) {
            printf("init the lock or cond fail\n");
            break;
        }

        /* 启动min_thr_num个work thread */
        for (i = 0; i < min_thr_num; ++i) {
            pthread_create(&pool->threads[i], NULL, threadpool_thread, (void *) pool);
            printf("start thread %#x...\n", (unsigned) pool->threads[i]);
        }
        pthread_create(&pool->adjust_tid, NULL, adjust_thread, (void *) pool);

        return pool;
    } while (0);

    /* 前边代码调用失败时调用 */
    threadpool_free(pool);
    return NULL;
}

int threadpool_add(threadpool_t *pool, void *(*function)(void *arg), void *arg)
{
    assert(pool != NULL);
    assert(function != NULL);
    assert(arg != NULL);

    pthread_mutex_lock(&pool->lock);

    while ((pool->queue_size == pool->queue_max_size) && !pool->shutdown) {
        pthread_cond_wait(&pool->queue_not_full, &pool->lock);
    }
    if (pool->shutdown)
        pthread_mutex_unlock(&pool->lock);

    /* add a task to queue */
    if (pool->task_queue[pool->queue_rear].arg != NULL) {
        free(pool->task_queue[pool->queue_rear].arg);
        pool->task_queue[pool->queue_rear].arg = NULL;
    }
    pool->task_queue[pool->queue_rear].function = function;
    pool->task_queue[pool->queue_rear].arg = arg;
    if (++pool->queue_rear == pool->queue_max_size)
        pool->queue_rear = 0;
    ++pool->queue_size;

    /* queue not empty */
    pthread_cond_signal(&pool->queue_not_empty);
    /* may unlock mutex twice, which may cause bug */
    pthread_mutex_unlock(&pool->lock);

    return 0;
}

void *threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *) threadpool;
    threadpool_task_t task;

    while (true) {
        /* lock must be taken to wait on conditional variable */
        /* 刚创建出线程，等待任务队列里有任务，否则阻塞等待 */
        pthread_mutex_lock(&pool->lock);
        while ((pool->queue_size == 0) && !pool->shutdown) {
            printf("thread %#x is waiting\n", (unsigned) pthread_self());
            /* 有可能被唤醒但是队列里并没有任务，而只是要删除线程 */
            pthread_cond_wait(&pool->queue_not_empty, &pool->lock);
            /* 清除指定数目的空闲进程，如果要结束的线程个数大于0，结束线程 */
            if (pool->wait_exit_thr_num > 0) {
                --pool->wait_exit_thr_num;
                /* 线程池里线程个数大于最小值时才结束当前进程 */
                if (pool->live_thr_num > pool->min_thr_num) {
                    printf("thread %#x is exiting\n", (unsigned) pthread_self());
                    --pool->live_thr_num;
                    pthread_mutex_unlock(&pool->lock);
                    pthread_exit(NULL);
                }
            }
        }
        /* 如果指定了true，要关闭线程池里的每个线程，自行退出处理 */
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            printf("thread %#x is exiting\n", (unsigned) pthread_self());
            pthread_exit(NULL);
        }

        /* 从任务队列里获得任务 */
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg = pool->task_queue[pool->queue_front].arg;
        if (++pool->queue_front == pool->queue_max_size)
            pool->queue_front = 0;
        --pool->queue_size;

        /* 通知可以有新的任务添加进来 */
        pthread_cond_broadcast(&pool->queue_not_full);
        pthread_mutex_unlock(&pool->lock);

        /* 执行任务 */
        printf("thread %#x start working\n", (unsigned) pthread_self());
        pthread_mutex_lock(&pool->thread_counter);
        ++pool->busy_thr_num;
        pthread_mutex_unlock(&pool->thread_counter);
        (*task.function)(task.arg);

        /* 任务结束处理 */
        printf("thread %#x end working\n", (unsigned) pthread_self());
        pthread_mutex_lock(&pool->thread_counter);
        --pool->busy_thr_num;
        pthread_mutex_unlock(&pool->thread_counter);
    }

    pthread_exit(NULL);
    return NULL;
}

void *adjust_thread(void *threadpool)
{
    int i;
    threadpool_t *pool = (threadpool_t *) threadpool;

    while (!pool->shutdown) {
        sleep(DEFAULT_TIME);

        pthread_mutex_lock(&pool->lock);
        int queue_size = pool->queue_size;
        int live_thr_num = pool->live_thr_num;
        pthread_mutex_unlock(&pool->lock);

        pthread_mutex_lock(&pool->thread_counter);
        int busy_thr_num = pool->busy_thr_num;
        pthread_mutex_unlock(&pool->thread_counter);

        /* 任务数大于最小线程个数并且存活的线程数少于最大线程个数时，创建新线程 */
        if (queue_size >= MIN_WAIT_TASK_NUM && live_thr_num < pool->max_thr_num) {
            pthread_mutex_lock(&pool->lock);
            int add = 0;
            /* 一次增加DEFAULT_THREAD_VARY个线程 */
            for (i = 0; i < pool->max_thr_num && add < DEFAULT_THREAD_VARY
                    && pool->live_thr_num < pool->max_thr_num; ++i) {
                if (pool->threads[i] == 0 || !is_thread_alive(pool->threads[i])) {
                    pthread_create(&pool->threads[i], NULL, threadpool_thread, (void *) pool);
                    ++add;
                    ++pool->live_thr_num;
                }
            }
            pthread_mutex_unlock(&pool->lock);
        }
        /* 销毁多余的空闲线程 */
        if (busy_thr_num * 2 < live_thr_num && live_thr_num > pool->min_thr_num) {
            /* 一次销毁DEFAULT_THREAD_VARY个线程 */
            pthread_mutex_lock(&pool->lock);
            pool->wait_exit_thr_num = DEFAULT_THREAD_VARY;
            pthread_mutex_unlock(&pool->lock);

            for (i = 0; i < DEFAULT_THREAD_VARY; ++i) {
                /* 通知处在空闲状态的线程 */
                pthread_cond_signal(&pool->queue_not_empty);
            }
        }
    }
    return NULL;
}

int threadpool_destroy(threadpool_t *pool)
{
    int i;
    if (pool == NULL)
        return -1;

    pool->shutdown = true;
    /* 先销毁管理线程 */
    pthread_join(pool->adjust_tid, NULL);
    /* 通知所有空闲线程 */
    pthread_cond_broadcast(&pool->queue_not_empty);
    for (i = 0; i < pool->min_thr_num; ++i)
        pthread_join(pool->threads[i], NULL);
    threadpool_free(pool);

    return 0;
}

int threadpool_free(threadpool_t *pool)
{
    if (pool == NULL)
        return -1;
    if (pool->task_queue)
        free(pool->task_queue);
    if (pool->threads) {
        free(pool->threads);
        pthread_mutex_lock(&pool->lock);
        pthread_mutex_destroy(&pool->lock);
        pthread_mutex_lock(&pool->thread_counter);
        pthread_mutex_destroy(&pool->thread_counter);
        pthread_cond_destroy(&pool->queue_not_empty);
        pthread_cond_destroy(&pool->queue_not_full);
    }
    free(pool);
    pool = NULL;

    return 0;
}

int threadpool_all_threadnum(threadpool_t *pool)
{
    int all_threadnum = -1;
    pthread_mutex_lock(&pool->lock);
    all_threadnum = pool->live_thr_num;
    pthread_mutex_unlock(&pool->lock);

    return all_threadnum;
}

int threadpool_busy_threadnum(threadpool_t *pool)
{
    int busy_threadnum = -1;
    pthread_mutex_lock(&pool->lock);
    busy_threadnum = pool->busy_thr_num;
    pthread_mutex_unlock(&pool->lock);

    return busy_threadnum;
}

int is_thread_alive(pthread_t tid)
{
    int kill_rc = pthread_kill(tid, 0);
    return kill_rc != ESRCH;
}

#if 1
void *process(void *arg)
{
    printf("thread %#x working on task %d\n", (unsigned) pthread_self(), *(int *) arg);
    sleep(1);
    printf("task %d is end\n", *(int *) arg);
    return NULL;
}

int main()
{
    /* 线程池里最小3个线程，最大100个，队列最大值12 */
    threadpool_t *thp = threadpool_create(3, 100, 12);
    printf("pool inited");

    int *num = (int *) malloc(sizeof (int) * 20);
    int i;

    for (i = 0; i < 10; ++i) {
        num[i] = i;
        printf("add task %d\n", i);
        threadpool_add(thp, process, (void *) &num[i]);
    }
    sleep(10);
    threadpool_destroy(thp);

    return 0;
}
#endif
