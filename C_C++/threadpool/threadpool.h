#pragma once

#include "condition.h"

/* task struct, put into threadpool to be handled */
typedef struct task {
        void *(*run)(void *arg);        // callback function
        void *arg;                      // arg to func above
        struct task *next;
} task_t;

/* threadpool struct */
typedef struct threadpool {
        condition_t   ready;          // task or destroy event
        task_t       *first;          // task queue head pointer
        task_t       *last;           // task queue tail pointer
        int           counter;        // current threads in the pool
        int           idle;           // idle threads' num
        int           max_threads;    // max limit num in the pool
        int           quit;           // set 1 to be destroyed
} threadpool_t;

void threadpool_init(threadpool_t *pool, int threads);
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg);
void threadpool_destroy(threadpool_t *pool);
