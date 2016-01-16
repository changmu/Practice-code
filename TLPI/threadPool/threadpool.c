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
    pthread_cond_t queue_not_full;
    pthread_cond_t queue_not_empty;
    pthread_t *threads;                 /* 保存工作线程tid的数组 */
    pthread_t adjust_tid;               /* 管理线程tid */
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
        pool->threads = (pthread_t *) calloc(sizeof (pthread_t) * max_thr_num);
        if (pool->threads == NULL) {
            printf("malloc threads fail\n");
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
            printf("init the lock or cond fail");
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

int threadpool_add(threadpool_t *poll, void *(*function)(void *arg), void *arg)
{
    assert(pool != NULL);
    assert(function != NULL);
    assert(arg != NULL);

    pthread_mutex_lock(&pool->lock);

    while ((pool->queue_size == poll->queue_max_size) && !pool->shutdown) {
        pthread_cond_wait(&pool->queue_not_full, &pool->lock);
    }
    if (pool->shutdown)
        pthread_mutex_unlock(&pool->lock);

    /* add a task to queue */
    if (pool->task_queue[pool->queue_rear].arg != NULL) {
        free(pool->task_queue)
            // to do
    }
}

int is_thread_alive(pthread_t tid)
{
    int kill_rc = pthread_kill(tid, 0);
    return kill_rc != ESRCH;
}

#ifdef DEBUG
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
